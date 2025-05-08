QT       += sql
QT += serialport
QT += core gui serialport
QT += core gui serialport widgets
QT   += charts
QT += core gui sql charts
QT += charts widgets
QT += network
QT += core gui network
QT += core gui sql charts
QT += widgets sql
QT += core gui widgets sql charts  # Example: Add 'charts' if needed
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
QT += printsupport
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    arduinomanager.cpp \
    connection.cpp \
    connectionh.cpp \
    connectionn.cpp \
    main.cpp \
    mainwindow.cpp \
    messagesmanager.cpp \
    patients.cpp \
    personnelmanager.cpp \
    researchermanager.cpp \
    toolsmanager.cpp \
    vaccinmanager.cpp \
    buttondelegate.cpp \
    projetderecherche.cpp \
    todolist.cpp \
    materialinputdialog.cpp \
    serialmanager.cpp \
    qrcodegen.cpp

HEADERS += \
    arduinomanager.h \
    connection.h \
    connectionh.h \
    connectionn.h \
    mainwindow.h \
    messagesmanager.h \
    patients.h \
    personnelmanager.h \
    researchermanager.h \
    toolsmanager.h \
    vaccinmanager.h \
    buttondelegate.h \
    projetderecherche.h \
    todolist.h \
    materialinputdialog.h \
    serialmanager.h \
    qrcodegen.hpp

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Resources.qrc
