#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <PsxMemCard/PsxMemCard.h>
#include <fstream>
#include <vector>
#include <iostream>

int main(int argc, char *argv[]){

    std::vector<std::uint8_t> memcardImage;
    memcardImage.resize(1024*128);

    auto file = std::ifstream("D:/memcard/full.bin", std::ios::binary);
    file.read((char*)memcardImage.data(), memcardImage.size());
    file.close();

    bool blockValid = Psx::MemCard::HeaderFrame::isBlockValid(memcardImage.data());


    for(int i = 1; i < 16; ++i){
        auto dir = (Psx::MemCard::DirectoryFrame*)(memcardImage.data() + (128 * i));
        std::cout << "Allocation state: " << std::hex << "0x" << (int)dir->allocationState[0] << std::endl;
        std::cout << "File name: " << (char*)dir->name << std::endl;
        std::cout << "File size: " << std::dec << *(std::uint32_t*)dir->fileSize << std::endl;
    }




    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
