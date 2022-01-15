#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "chatserver.h"
#include "infodisplay.h"
#include <listconnexion.h>

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);



    ListConnexion* notSignedInUsers = new ListConnexion();
    ListConnexion* connectedusers = new ListConnexion();

    InfoDisplay serverInterface(notSignedInUsers, connectedusers);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("server", &serverInterface);
    engine.rootContext()->setContextProperty("NotSignedInUsers", notSignedInUsers);
    engine.rootContext()->setContextProperty("ConnectedUsers", connectedusers);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
