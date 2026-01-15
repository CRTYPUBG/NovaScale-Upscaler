import sys
import os
import traceback
import datetime
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

# --- Logging Setup ---
LOG_FILE = os.path.join(os.path.abspath("."), "novascale_error.log")

class Logger(object):
    def __init__(self, original_stream):
        self.terminal = original_stream
        try:
            self.log = open(LOG_FILE, "a", encoding="utf-8")
        except Exception:
            self.log = None

    def write(self, message):
        if self.terminal:
            try:
                self.terminal.write(message)
            except:
                pass
        if self.log:
            try:
                self.log.write(message)
                self.log.flush()
            except:
                pass

    def flush(self):
        if self.terminal:
            try:
                self.terminal.flush()
            except:
                pass
        if self.log:
            try:
                self.log.flush()
            except:
                pass

sys.stdout = Logger(sys.stdout)
sys.stderr = Logger(sys.stderr)

def handle_exception(exc_type, exc_value, exc_traceback):
    if issubclass(exc_type, KeyboardInterrupt):
        sys.__excepthook__(exc_type, exc_value, exc_traceback)
        return
    print("FATAL UNCAUGHT EXCEPTION:")
    traceback.print_exception(exc_type, exc_value, exc_traceback)
    
    # Try to open the log file even if main() wasn't the one that failed
    try:
        os.startfile(LOG_FILE)
    except:
        pass

sys.excepthook = handle_exception

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
    try:
        main()
    except Exception:
        with open(LOG_FILE, "a", encoding="utf-8") as f:
            f.write("\n" + "="*50 + "\n")
            f.write("CRASH DETECTED AT: " + str(datetime.datetime.now()) + "\n")
            traceback.print_exc(file=f)
        
        # Open log file for user
        os.startfile(LOG_FILE)
