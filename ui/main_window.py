import sys
import ctypes
import os
from PySide6.QtWidgets import QMainWindow, QWidget, QVBoxLayout, QHBoxLayout, QLabel, QPushButton, QFrame
from PySide6.QtCore import Qt, QTimer
from PySide6.QtGui import QFont, QColor, QIcon
from settings import SettingsPanel

# --- C API Mapping ---
class Config(ctypes.Structure):
    _fields_ = [
        ("mode", ctypes.c_int),
        ("scale_factor", ctypes.c_float),
        ("sharpness", ctypes.c_float),
        ("enable_temporal", ctypes.c_bool),
        ("show_fps", ctypes.c_bool)
    ]

class Stats(ctypes.Structure):
    _fields_ = [
        ("frame_time_ms", ctypes.c_float),
        ("capture_time_ms", ctypes.c_float),
        ("upscale_time_ms", ctypes.c_float),
        ("present_time_ms", ctypes.c_float),
        ("fps", ctypes.c_uint32)
    ]

def resource_path(relative_path):
    """ Get absolute path to resource, works for dev and for PyInstaller """
    try:
        # PyInstaller creates a temp folder and stores path in _MEIPASS
        base_path = sys._MEIPASS
    except Exception:
        base_path = os.path.abspath(".")
    return os.path.join(base_path, relative_path)

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("NovaScale v1.0")
        self.setMinimumSize(420, 680)
        self.setWindowIcon(QIcon(resource_path("app.ico")))
        self.is_running = False
        
        # Load C Core
        self.lib = None
        try:
            # Check locally, then in packed path
            dll_candidates = [
                os.path.abspath("ui/novascale.dll"),
                os.path.abspath("novascale.dll"),
                resource_path("novascale.dll")
            ]
            
            for dll_path in dll_candidates:
                if os.path.exists(dll_path):
                    self.lib = ctypes.CDLL(dll_path)
                    break
            
            if self.lib:
                self.lib.NovaScale_GetStats.restype = Stats
                self.lib.NovaScale_Initialize.restype = ctypes.c_bool
                self.lib.NovaScale_Start.argtypes = [Config]
                self.lib.NovaScale_Initialize()
        except Exception as e:
            print(f"Failed to load engine core: {e}")

        self.init_ui()
        self.apply_theme()

    def init_ui(self):
        central_widget = QWidget()
        self.setCentralWidget(central_widget)
        main_layout = QVBoxLayout(central_widget)
        main_layout.setContentsMargins(30, 30, 30, 30)
        main_layout.setSpacing(25)

        # Header with neon effect
        header_frame = QFrame()
        header_layout = QVBoxLayout(header_frame)
        self.header = QLabel("NOVASCALE")
        self.header.setFont(QFont("Outfit", 32, QFont.ExtraBold))
        self.header.setAlignment(Qt.AlignCenter)
        self.header.setStyleSheet("color: #00f2ff; letter-spacing: 4px;")
        
        self.subtitle = QLabel("AI-POWERED SPATIAL UPSCALER")
        self.subtitle.setFont(QFont("Inter", 9, QFont.Bold))
        self.subtitle.setAlignment(Qt.AlignCenter)
        self.subtitle.setStyleSheet("color: #555; letter-spacing: 2px;")
        
        header_layout.addWidget(self.header)
        header_layout.addWidget(self.subtitle)
        main_layout.addWidget(header_frame)

        # Main Settings
        self.settings = SettingsPanel()
        main_layout.addWidget(self.settings)

        # Status & Stats Display (Avant-Garde Grid)
        self.stats_box = QFrame()
        self.stats_box.setStyleSheet("background: rgba(255, 255, 255, 0.03); border: 1px solid rgba(255, 255, 255, 0.05); border-radius: 12px;")
        stats_layout = QVBoxLayout(self.stats_box)
        
        self.stats_grid = QHBoxLayout()
        self.fps_label = self.create_stat_widget("FPS", "--")
        self.lat_label = self.create_stat_widget("LATENCY", "-- ms")
        self.stats_grid.addWidget(self.fps_label)
        self.stats_grid.addWidget(self.lat_label)
        
        self.status_text = QLabel("STATUS: SYSTEM IDLE")
        self.status_text.setFont(QFont("Consolas", 9))
        self.status_text.setAlignment(Qt.AlignCenter)
        self.status_text.setStyleSheet("color: #444;")
        
        stats_layout.addLayout(self.stats_grid)
        stats_layout.addWidget(self.status_text)
        main_layout.addWidget(self.stats_box)

        # Controls
        self.start_btn = QPushButton("INITIALIZE ENGINE")
        self.start_btn.setFixedHeight(60)
        self.start_btn.setCursor(Qt.PointingHandCursor)
        self.start_btn.clicked.connect(self.toggle_engine)
        
        main_layout.addWidget(self.start_btn)

        # Stats Update Timer
        self.timer = QTimer()
        self.timer.timeout.connect(self.update_telemetry)
        self.timer.start(500)

    def create_stat_widget(self, title, value):
        container = QWidget()
        layout = QVBoxLayout(container)
        t = QLabel(title)
        t.setFont(QFont("Inter", 8, QFont.Bold))
        t.setStyleSheet("color: #666;")
        v = QLabel(value)
        v.setFont(QFont("Outfit", 18, QFont.Bold))
        v.setStyleSheet("color: #eee;")
        layout.addWidget(t)
        layout.addWidget(v)
        container.value_label = v
        return container

    def apply_theme(self):
        self.setStyleSheet("""
            QMainWindow { background-color: #08080a; }
            QLabel { color: #ccc; }
            QPushButton { 
                background: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #00f2ff, stop:1 #0072ff);
                color: #000; border: none; border-radius: 8px; font-weight: 800; font-size: 14px; 
            }
            QPushButton:hover { background: #00f2ff; }
            QPushButton:pressed { background: #00c2cc; }
            QPushButton#stop { background: #1a1a1f; color: #ff4444; border: 1px solid #333; }
        """)

    def toggle_engine(self):
        self.is_running = not self.is_running
        
        if self.is_running:
            cfg = Config()
            cfg.mode = self.settings.mode_combo.currentIndex()
            cfg.scale_factor = self.settings.scale_slider.value() / 10.0
            cfg.sharpness = self.settings.sharp_slider.value() / 100.0
            
            if self.lib:
                self.lib.NovaScale_Start(cfg)
            
            self.start_btn.setText("STOP ENGINE")
            self.start_btn.setObjectName("stop")
            self.status_text.setText("STATUS: ENGINE ACTIVE")
            self.status_text.setStyleSheet("color: #00f2ff;")
        else:
            if self.lib:
                self.lib.NovaScale_Stop()
            
            self.start_btn.setText("INITIALIZE ENGINE")
            self.start_btn.setObjectName("")
            self.status_text.setText("STATUS: SYSTEM IDLE")
            self.status_text.setStyleSheet("color: #444;")
        
        self.apply_theme() # Refresh dynamic styles

    def update_telemetry(self):
        if self.is_running and self.lib:
            stats = self.lib.NovaScale_GetStats()
            self.fps_label.value_label.setText(str(stats.fps))
            self.lat_label.value_label.setText(f"{stats.frame_time_ms:.1f}ms")
