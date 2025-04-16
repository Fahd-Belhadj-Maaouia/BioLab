QT       += sql
QT += core gui sql
QT       += core gui widgets charts
QT += charts
QT += core gui network
CONFIG   += c++17
CONFIG += console
SOURCES += \
    buttondelegate.cpp \
    connection.cpp \
    main.cpp \
    mainwindow.cpp \
    projetderecherche.cpp \
    todolist.cpp

HEADERS += \
    buttondelegate.h \
    connection.h \
    mainwindow.h \
    projetderecherche.h \
    todolist.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    Resouces.qrc
