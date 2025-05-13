TEMPLATE = app
CONFIG += console c++17
QT += quick  quickcontrols2
include($$PWD/Dependencies/Dependencies.pri)

SOURCES += \
    SjisEncoding.cpp \
    main.cpp

RESOURCES += qml.qrc

HEADERS += \
    AppMemcard.h \
    Application.h \
    ImageBmp.h \
    Memcard.h \
    MemcardImageProvider.h \
    SjisEncoding.h
