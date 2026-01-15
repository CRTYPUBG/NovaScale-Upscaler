import sys
import os
from PySide6.QtWidgets import QApplication, QSplashScreen
from PySide6.QtGui import QPixmap, QScreen
from PySide6.QtCore import Qt, QTimer
from main_window import MainWindow

def resource_path(relative_path):
    try:
        base_path = sys._MEIPASS
    except Exception:
        base_path = os.path.abspath(".")
    return os.path.join(base_path, relative_path)

def main():
    app = QApplication(sys.argv)
    
    # Splash Screen
    splash_pix = QPixmap(resource_path("slpsh_screen.png"))
    splash = QSplashScreen(splash_pix, Qt.WindowStaysOnTopHint)
    splash.show()
    
    # Move splash to center of primary screen
    primary_screen = app.primaryScreen()
    if primary_screen:
        screen_geometry = primary_screen.geometry()
        splash.move(screen_geometry.center() - splash.rect().center())

    # Simulate some loading or just show for a bit
    app.processEvents()
    
    window = MainWindow()
    
    # Fade out or just close splash
    QTimer.singleShot(2000, lambda: (window.show(), splash.finish(window)))
    
    sys.exit(app.exec())

if __name__ == "__main__":
    main()
