#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include "videocontroller.h"

int main(int argc, char *argv[])
{
    qputenv("QT_QPA_PLATFORM", "xcb");
    
    QGuiApplication app(argc, argv);

    qmlRegisterType<VideoController>("VideoAnalyzer", 1, 0, "VideoController");

    QQmlApplicationEngine engine;

    engine.addImportPath("qrc:/");
    
    const QUrl url(u"qrc:/VideoAnalyzer.qml"_qs);
    engine.load(url);

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
} 