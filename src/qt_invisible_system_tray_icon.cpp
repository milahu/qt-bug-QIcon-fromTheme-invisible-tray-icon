#include <QApplication>
#include <QMainWindow>
#include <QShortcut>
#include <QSystemTrayIcon>
#include <QIcon>
#include <QLabel>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    if (!QSystemTrayIcon::isSystemTrayAvailable()) {
        qWarning() << "System tray is not available!";
        return 1;
    }

    QIcon icon;
    if (argc > 1) { // just pass some argument to reach this branch
      qDebug() << "not using QIcon::fromTheme, this should work = the system tray icon should be **visible**";
      // Working version:
      icon = QIcon(":/resources/icons/koi_tray.png");
    }
    else {
      qDebug() << "using QIcon::fromTheme, this should break = the system tray icon should be **invisible**";
      qDebug() << "hint: call this executable with some argument to *not* use QIcon::fromTheme, that should work";
      // Failing version: shows transparent icon if theme lookup "succeeds" with a broken result
      // expected: QIcon::fromTheme should use the fallback icon
      QIcon fallbackIcon = QIcon(":/resources/icons/koi_tray.png");
      icon = QIcon::fromTheme("koi_tray", fallbackIcon);
    }

    QMainWindow window;
    window.setWindowTitle("QIcon::fromTheme test");
    window.setWindowIcon(icon);
    window.resize(400, 200);

    QLabel *iconLabel = new QLabel(&window);
    int size = 256;
    iconLabel->setPixmap(icon.pixmap(size, size));
    iconLabel->setAlignment(Qt::AlignCenter);
    window.setCentralWidget(iconLabel);

    QShortcut *quitShortcut = new QShortcut(QKeySequence("Ctrl+Q"), &window);
    // QObject::connect(quitShortcut, &QShortcut::activated, &window, &QMainWindow::close);
    QObject::connect(quitShortcut, &QShortcut::activated, &app, &QApplication::quit);

    window.show();

    QSystemTrayIcon *trayIcon = new QSystemTrayIcon(&window);
    trayIcon->setIcon(icon);
    trayIcon->setToolTip("QIcon::fromTheme test");
    trayIcon->setVisible(true);
    trayIcon->show();

    return app.exec();
}
