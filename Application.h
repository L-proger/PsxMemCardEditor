#pragma once

#include <QObject>
#include <QString>
#include <Memcard.h>
#include "SjisEncoding.h"
#include <iostream>
#include <AppMemcard.h>

class Application : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ name)
public:

    Q_INVOKABLE QObject* openMemcard() {
        VirtualMemcardIo::Image memcardImage;

        auto file = std::ifstream("D:/memcard/full.bin", std::ios::binary);
        file.read((char*)memcardImage.data(), memcardImage.size());
        file.close();

        auto memcard = std::make_shared<Memcard>(std::make_shared<VirtualMemcardIo>(memcardImage));

        return new AppMemcard(memcard);
    }

    explicit Application(QObject* parent = nullptr) : QObject(parent) {
        VirtualMemcardIo::Image memcardImage;

        auto file = std::ifstream("D:/memcard/full.bin", std::ios::binary);
        file.read((char*)memcardImage.data(), memcardImage.size());
        file.close();

        _mc = std::make_shared<Memcard>(std::make_shared<VirtualMemcardIo>(memcardImage));

        for(int i = 0; i < 15; ++i){
            auto& file = _mc->getFile(i);

            std::cout << "Memcard file id: " << i << std::endl;

            if(file.allocationState == Psx::MemCard::BlockAllocationState::FirstFileBlock){
                Memcard::FileHeader header;
                _mc->readFileHeader(&file, header);

                char str[64];
                sjisToAscii(header.title.data(), str);
                std::cout << str << std::endl;

                Memcard::RawIcon iconRaw;
                Memcard::Icon iconPaletted;
                for(int i = 0; i < header.iconFramesCount; ++i){

                    _mc->readFileIcon(&file, i, iconRaw);
                    Memcard::unpackRawIcon(header.colorPalette, iconRaw, iconPaletted);
                    std::cout << "Read icon: " << i << std::endl;

                    IconBgr24 icon24;
                    iconToBgr24(iconPaletted, icon24);

                    std::ofstream imgFile("D:/memcard/test.bmp", std::ios::binary);
                    LFramework::Image::Bmp::save24bitImage(16, 16, icon24.data(), imgFile);
                    imgFile.close();
                }
            }
        }
    }

    IconBgr24 getImage(int fileId, int iconId) {
        auto& file = _mc->getFile(fileId);

        Memcard::FileHeader header;
        _mc->readFileHeader(&file, header);

        Memcard::RawIcon iconRaw;
        Memcard::Icon iconPaletted;

        _mc->readFileIcon(&file, iconId % header.iconFramesCount, iconRaw);
        Memcard::unpackRawIcon(header.colorPalette, iconRaw, iconPaletted);

        IconBgr24 icon24;
        iconToRgb24(iconPaletted, icon24);
        return icon24;
    }

    QString name() {
        return "12345";
    }
private:
    std::shared_ptr<Memcard> _mc;
};
