#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <PsxMemCard/PsxMemCard.h>
#include <fstream>
#include <vector>
#include <iostream>
#include <array>
#include <vector>
#include <string>
#include <memory>
#include <ImageBmp.h>
#include <Application.h>
#include <MemcardImageProvider.h>
#include <QQuickStyle>

int main(int argc, char *argv[]){
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    Application application;
    engine.addImageProvider("memcard", new ColorImageProvider(&application));
    engine.rootContext()->setContextProperty("app", &application);

    QQuickStyle::setStyle("Material");

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
    //return 0;
}
