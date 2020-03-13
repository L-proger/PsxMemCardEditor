TEMPLATE = app
CONFIG += console c++17
QT += quick
include($$PWD/Dependencies/Dependencies.pri)

SOURCES += \
    main.cpp

RESOURCES += qml.qrc
