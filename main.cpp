#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "chatcontroller.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    
    ChatController chatController;
    engine.rootContext()->setContextProperty("chatController", &chatController);

    const QUrl url(u"qrc:/main.qml"_qs);
    engine.load(url);

    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
} 