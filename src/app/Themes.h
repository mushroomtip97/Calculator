#ifndef THEMES_H
#define THEMES_H

#include <QString>
#include <QApplication>

namespace Themes{
inline const QString light = R"qss(
            QWidget { background-color: #f5f5f5; color: #1c1c1c; }
            QMainWindow, QDialog { background-color: #f5f5f5; }
            QLabel { background: transparent; color: #1c1c1c; }
            QLineEdit, QComboBox, QDateEdit, QSpinBox, QListWidget {
                background-color: #ffffff;
                color: #1c1c1c;
                border: 1px solid #b8b8b8;
                border-radius: 4px;
                padding: 4px 6px;
                selection-background-color: #1f6fd6;
                selection-color: #ffffff;
            }
            QPushButton {
                background-color: #ffffff;
                color: #1c1c1c;
                border: 1px solid #b8b8b8;
                border-radius: 4px;
                padding: 5px 10px;
            }
            QPushButton:hover { background-color: #e8e8e8; }
            QPushButton:pressed { background-color: #d5d5d5; }
            QGraphicsView { background-color: #ffffff; border: none; }
            QMenuBar, QMenu { background-color: #f5f5f5; color: #1c1c1c; }
            QStatusBar { background-color: #f5f5f5; color: #555555; }
            QSplitter::handle { background-color: #c8c8c8; }
            QComboBox QAbstractItemView {
                background-color: #ffffff;
                color: #1c1c1c;
                selection-background-color: #1f6fd6;
                selection-color: #ffffff;
            }
            QCalendarWidget QWidget { background-color: #ffffff; color: #1c1c1c; }
        )qss";

inline const QString dark = R"qss(
            QWidget { background-color: #1e1e1e; color: #eaeaea; }
            QMainWindow, QDialog { background-color: #1e1e1e; }
            QLabel { background: transparent; color: #eaeaea; }
            QLineEdit, QComboBox, QDateEdit, QSpinBox, QListWidget {
                background-color: #2b2b2b;
                color: #eaeaea;
                border: 1px solid #555555;
                border-radius: 4px;
                padding: 4px 6px;
                selection-background-color: #1f6fd6;
                selection-color: #ffffff;
            }
            QPushButton {
                background-color: #2d2d2d;
                color: #eaeaea;
                border: 1px solid #555555;
                border-radius: 4px;
                padding: 5px 10px;
            }
            QPushButton:hover { background-color: #3a3a3a; }
            QPushButton:pressed { background-color: #4a4a4a; }
            QGraphicsView { background-color: #1e1e1e; border: none; }
            QMenuBar, QMenu { background-color: #1e1e1e; color: #eaeaea; }
            QStatusBar { background-color: #1e1e1e; color: #9a9a9a; }
            QSplitter::handle { background-color: #555555; }
            QComboBox QAbstractItemView {
                background-color: #2b2b2b;
                color: #eaeaea;
                selection-background-color: #1f6fd6;
                selection-color: #ffffff;
            }
            QCalendarWidget QWidget { background-color: #2b2b2b; color: #eaeaea; }
        )qss";

inline const QString blue = R"qss(
            QWidget { background-color: #eef4fc; color: #10243e; }
            QMainWindow, QDialog { background-color: #eef4fc; }
            QLabel { background: transparent; color: #10243e; }
            QLineEdit, QComboBox, QDateEdit, QSpinBox, QListWidget {
                background-color: #ffffff;
                color: #10243e;
                border: 1px solid #9db8d9;
                border-radius: 4px;
                padding: 4px 6px;
                selection-background-color: #1f6fd6;
                selection-color: #ffffff;
            }
            QPushButton {
                background-color: #1f6fd6;
                color: #ffffff;
                border: 1px solid #1557a8;
                border-radius: 4px;
                padding: 5px 10px;
            }
            QPushButton:hover { background-color: #3b82e0; }
            QPushButton:pressed { background-color: #1557a8; }
            QGraphicsView { background-color: #ffffff; border: none; }
            QMenuBar, QMenu { background-color: #eef4fc; color: #10243e; }
            QStatusBar { background-color: #eef4fc; color: #4a6a8a; }
            QSplitter::handle { background-color: #9db8d9; }
            QComboBox QAbstractItemView {
                background-color: #ffffff;
                color: #10243e;
                selection-background-color: #1f6fd6;
                selection-color: #ffffff;
            }
            QCalendarWidget QWidget { background-color: #ffffff; color: #10243e; }
        )qss";

inline const QString metallicSilver = R"qss(
            QWidget {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #f7f7f7, stop:0.5 #dedede, stop:1 #c2c2c2);
                color: #1f1f1f;
            }
            QMainWindow, QDialog {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #f7f7f7, stop:0.5 #dedede, stop:1 #c2c2c2);
            }
            QLabel { background: transparent; color: #1f1f1f; }
            QLineEdit, QComboBox, QDateEdit, QSpinBox, QListWidget {
                background-color: #ffffff;
                color: #1f1f1f;
                border: 1px solid #a8a8a8;
                border-radius: 4px;
                padding: 4px 6px;
                selection-background-color: #1f6fd6;
                selection-color: #ffffff;
            }
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #fdfdfd, stop:0.5 #d6d6d6, stop:1 #b5b5b5);
                color: #1f1f1f;
                border: 1px solid #9a9a9a;
                border-radius: 4px;
                padding: 5px 10px;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #ffffff, stop:0.5 #e4e4e4, stop:1 #c6c6c6);
            }
            QPushButton:pressed { background-color: #c0c0c0; }
            QGraphicsView { background-color: #ffffff; border: none; }
            QMenuBar, QMenu { background-color: #e8e8e8; color: #1f1f1f; }
            QStatusBar { background-color: #d8d8d8; color: #444444; }
            QSplitter::handle { background-color: #9e9e9e; }
            QComboBox QAbstractItemView {
                background-color: #ffffff;
                color: #1f1f1f;
                selection-background-color: #1f6fd6;
                selection-color: #ffffff;
            }
            QCalendarWidget QWidget { background-color: #ffffff; color: #1f1f1f; }
        )qss";

inline const QString metallicGold = R"qss(
            QWidget {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #fbf0c4, stop:0.5 #e8c86a, stop:1 #d3ad42);
                color: #3a2a00;
            }
            QMainWindow, QDialog {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #fbf0c4, stop:0.5 #e8c86a, stop:1 #d3ad42);
            }
            QLabel { background: transparent; color: #3a2a00; }
            QLineEdit, QComboBox, QDateEdit, QSpinBox, QListWidget {
                background-color: #fff9e2;
                color: #3a2a00;
                border: 1px solid #b8933a;
                border-radius: 4px;
                padding: 4px 6px;
                selection-background-color: #b8860b;
                selection-color: #ffffff;
            }
            QPushButton {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #fff1b8, stop:0.5 #e9c96e, stop:1 #cda33a);
                color: #3a2a00;
                border: 1px solid #b88a1f;
                border-radius: 4px;
                padding: 5px 10px;
            }
            QPushButton:hover {
                background: qlineargradient(x1:0, y1:0, x2:0, y2:1,
                    stop:0 #fff7cc, stop:0.5 #f2d982, stop:1 #dcb44d);
            }
            QPushButton:pressed { background-color: #d9b64e; }
            QGraphicsView { background-color: #fff9e2; border: none; }
            QMenuBar, QMenu { background-color: #f2d982; color: #3a2a00; }
            QStatusBar { background-color: #e8c86a; color: #5a4400; }
            QSplitter::handle { background-color: #b8933a; }
            QComboBox QAbstractItemView {
                background-color: #fff9e2;
                color: #3a2a00;
                selection-background-color: #b8860b;
                selection-color: #ffffff;
            }
            QCalendarWidget QWidget { background-color: #fff9e2; color: #3a2a00; }
        )qss";

inline const QString highContrast = R"qss(
            QWidget { background-color: #000000; color: #ffffff; }
            QMainWindow, QDialog { background-color: #000000; }
            QLabel { background: transparent; color: #ffff00; }
            QLineEdit, QComboBox, QDateEdit, QSpinBox, QListWidget {
                background-color: #000000;
                color: #ffffff;
                border: 2px solid #ffff00;
                border-radius: 2px;
                padding: 4px 6px;
                selection-background-color: #ffff00;
                selection-color: #000000;
            }
            QPushButton {
                background-color: #000000;
                color: #ffff00;
                border: 2px solid #ffff00;
                border-radius: 2px;
                padding: 5px 10px;
            }
            QPushButton:hover { background-color: #ffff00; color: #000000; }
            QPushButton:pressed { background-color: #ccff00; color: #000000; }
            QGraphicsView { background-color: #000000; border: 2px solid #ffff00; }
            QMenuBar, QMenu { background-color: #000000; color: #ffff00; }
            QStatusBar { background-color: #000000; color: #ffff00; }
            QSplitter::handle { background-color: #ffff00; }
            QComboBox QAbstractItemView {
                background-color: #000000;
                color: #ffffff;
                selection-background-color: #ffff00;
                selection-color: #000000;
            }
            QCalendarWidget QWidget { background-color: #000000; color: #ffff00; }
        )qss";

inline QString inkLight = R"qss(
    QWidget {
        background-color: #fbfaf7; /* Soft paper white */
        color: #111111;            /* Deep charcoal ink */
    }
    QMainWindow, QDialog {
        background-color: #fbfaf7;
    }
    QLabel {
        background: transparent;
        color: #0f111a;
    }
    QLineEdit, QComboBox, QDateEdit, QSpinBox, QListWidget {
        background-color: #ffffff;
        color: #111111;
        border: 2px solid #2c3e50; /* Pen outline */
        border-radius: 2px;
        padding: 4px 6px;
        selection-background-color: #111111;
        selection-color: #ffffff;
    }
    QPushButton {
        background-color: #ffffff;
        color: #111111;
        border: 2px solid #111111;
        border-radius: 2px;
        padding: 5px 10px;
        font-weight: bold;
    }
    QPushButton:hover {
        background-color: #111111;
        color: #fbfaf7;
    }
    QPushButton:pressed {
        background-color: #333333;
        color: #fbfaf7;
    }
    QGraphicsView {
        background-color: #ffffff;
        border: 2px solid #2c3e50;
    }
    QMenuBar, QMenu {
        background-color: #fbfaf7;
        color: #111111;
    }
    QStatusBar {
        background-color: #fbfaf7;
        color: #555555;
    }
    QSplitter::handle {
        background-color: #2c3e50;
    }
    QComboBox QAbstractItemView {
        background-color: #ffffff;
        color: #111111;
        selection-background-color: #111111;
        selection-color: #ffffff;
    }
    QCalendarWidget QWidget {
        background-color: #fbfaf7;
        color: #111111;
    }
)qss";

inline QString inkBlue = R"qss(
    QWidget {
        background-color: #0a0f1d; /* Deep blue-black ink tank */
        color: #e3e6eb;            /* Soft paper-white text */
    }
    QMainWindow, QDialog {
        background-color: #0a0f1d;
    }
    QLabel {
        background: transparent;
        color: #74b9ff; /* Blueprint / Ink Blue accent */
    }
    QLineEdit, QComboBox, QDateEdit, QSpinBox, QListWidget {
        background-color: #111827;
        color: #ffffff;
        border: 2px solid #00a8ff;
        border-radius: 2px;
        padding: 4px 6px;
        selection-background-color: #00a8ff;
        selection-color: #0a0f1d;
    }
    QPushButton {
        background-color: #0a0f1d;
        color: #00a8ff;
        border: 2px solid #00a8ff;
        border-radius: 2px;
        padding: 5px 10px;
    }
    QPushButton:hover {
        background-color: #00a8ff;
        color: #0a0f1d;
    }
    QPushButton:pressed {
        background-color: #0097e6;
        color: #0a0f1d;
    }
    QGraphicsView {
        background-color: #111827;
        border: 2px solid #00a8ff;
    }
    QMenuBar, QMenu {
        background-color: #0a0f1d;
        color: #00a8ff;
    }
    QStatusBar {
        background-color: #0a0f1d;
        color: #74b9ff;
    }
    QSplitter::handle {
        background-color: #00a8ff;
    }
    QComboBox QAbstractItemView {
        background-color: #111827;
        color: #ffffff;
        selection-background-color: #00a8ff;
        selection-color: #0a0f1d;
    }
    QCalendarWidget QWidget {
        background-color: #0a0f1d;
        color: #00a8ff;
    }
)qss";

inline QString pureInk = R"qss(
    QWidget {
        background-color: #0d0d0d; /* Solid black ink */
        color: #f5f5f5;            /* White paper text */
    }
    QMainWindow, QDialog {
        background-color: #0d0d0d;
    }
    QLabel {
        background: transparent;
        color: #ffffff;
    }
    QLineEdit, QComboBox, QDateEdit, QSpinBox, QListWidget {
        background-color: #161616;
        color: #ffffff;
        border: 2px solid #ffffff;
        border-radius: 0px;
        padding: 4px 6px;
        selection-background-color: #ffffff;
        selection-color: #000000;
    }
    QPushButton {
        background-color: #0d0d0d;
        color: #ffffff;
        border: 2px solid #ffffff;
        border-radius: 0px;
        padding: 5px 10px;
        font-weight: bold;
    }
    QPushButton:hover {
        background-color: #ffffff;
        color: #0d0d0d;
    }
    QPushButton:pressed {
        background-color: #cccccc;
        color: #000000;
    }
    QGraphicsView {
        background-color: #161616;
        border: 2px solid #ffffff;
    }
    QMenuBar, QMenu {
        background-color: #0d0d0d;
        color: #ffffff;
        border: 1px solid #ffffff;
    }
    QMenuBar::item:selected, QMenu::item:selected {
        background-color: #ffffff;
        color: #0d0d0d;
    }
    QStatusBar {
        background-color: #0d0d0d;
        color: #ffffff;
        border-top: 1px solid #ffffff;
    }
    QSplitter::handle {
        background-color: #ffffff;
    }
    QComboBox QAbstractItemView {
        background-color: #161616;
        color: #ffffff;
        border: 1px solid #ffffff;
        selection-background-color: #ffffff;
        selection-color: #000000;
    }
    QCalendarWidget QWidget {
        background-color: #161616;
        color: #ffffff;
    }
)qss";
}





#endif // THEMES_H

