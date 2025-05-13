#pragma once

#include <QObject>
#include <QString>
#include <Memcard.h>
#include "SjisEncoding.h"
#include <iostream>
#include <memory>

class AppMemcard : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString name READ name)
public:
    ~AppMemcard(){
        std::cout << "~AppMemcard" << std::endl;
    }
    AppMemcard(std::shared_ptr<Memcard> memcard) : _memcard(memcard){
        std::cout << "AppMemcard" << std::endl;
    }
    QString name() {
        return "MC0";
    }
private:
    std::shared_ptr<Memcard> _memcard;
};
