#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QDir>
#include "backend.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    // 注册后端类
    Backend backend;
    engine.rootContext()->setContextProperty("backend", &backend);

    const QUrl url = QUrl::fromLocalFile(QDir::currentPath() + "/qml/MainWindow.qml");
    engine.load(url);




    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
} 