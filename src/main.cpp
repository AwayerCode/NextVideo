#include <QApplication>
#include <QGuiApplication>
#include <QMessageBox>
#include <QDebug>
#include <QScreen>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    try {
        // 设置环境变量
        qputenv("QT_QPA_PLATFORM", "xcb");
        qputenv("QT_QPA_PLATFORMTHEME", "gtk3");
        qputenv("DISPLAY", ":0");
        qputenv("LC_ALL", "C");
        qputenv("LANG", "C");
        
        // 创建应用程序实例
        QApplication app(argc, argv);
        
        // 检查是否成功创建应用程序实例
        if (!QGuiApplication::primaryScreen()) {
            qDebug() << "Failed to initialize screen";
            return 1;
        }
        
        // 创建主窗口
        MainWindow window;
        
        // 设置窗口大小和位置
        window.resize(1280, 720);
        window.move(
            QGuiApplication::primaryScreen()->geometry().center() - window.rect().center()
        );
        
        window.show();
        
        return app.exec();
    } catch (const std::exception& e) {
        qDebug() << "Exception caught:" << e.what();
        return 1;
    } catch (...) {
        qDebug() << "Unknown exception caught";
        return 1;
    }
} 