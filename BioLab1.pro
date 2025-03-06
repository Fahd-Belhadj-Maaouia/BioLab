QT       += sql
QT += core gui sql
QT       += core gui widgets charts
CONFIG   += c++17
CONFIG += console
SOURCES += \
    connection.cpp \
    main.cpp \
    mainwindow.cpp \
    projetderecherche.cpp

HEADERS += \
    connection.h \
    mainwindow.h \
    projetderecherche.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    Resouces.qrc
