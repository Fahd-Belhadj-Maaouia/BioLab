QT       += sql
QT += core gui sql
QT       += core gui widgets charts
QT += charts
QT += core gui network
QT += serialport

CONFIG   += c++17
CONFIG += console
SOURCES += \
    buttondelegate.cpp \
    connection.cpp \
    main.cpp \
    mainwindow.cpp \
    materialinputdialog.cpp \
    projetderecherche.cpp \
    serialmanager.cpp \
    todolist.cpp

HEADERS += \
    buttondelegate.h \
    connection.h \
    mainwindow.h \
    materialinputdialog.h \
    projetderecherche.h \
    serialmanager.h \
    todolist.h

FORMS += \
    mainwindow.ui

RESOURCES += \
    Resouces.qrc
