#include <QGuiApplication>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "FileHandler.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    app.setApplicationName("PowerDelete");
    app.setApplicationVersion("1.0");
    
    QQmlApplicationEngine engine;

    FileHandler fileHandler;
    engine.rootContext()->setContextProperty("fileHandler", &fileHandler);

    const QUrl url(u"qrc:/qml/MainWindow.qml"_qs);
    engine.load(url);
    
    return app.exec();
} 