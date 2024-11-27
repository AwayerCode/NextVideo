#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // 注册 C++ 类型到 QML
    qmlRegisterType<MainWindow>("MyApp", 1, 0, "MainWindow");

    MainWindow mainWindow;
    engine.rootContext()->setContextProperty("mainWindow", &mainWindow);

    const QUrl url = QUrl::fromLocalFile(QDir::currentPath() + "/qml/MainWindow.qml");
    engine.load(url);

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
} 