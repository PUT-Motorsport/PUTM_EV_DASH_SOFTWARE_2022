QT       += core gui
QT += xml
QT += serialbus
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    canhandler.cpp \
    canraw.cpp \
    drivingselect.cpp \
    dvselect.cpp \
    gui.cpp \
    logger.cpp \
    logs.cpp \
    main.cpp \
    mainwindow.cpp \
    servicemode.cpp

HEADERS += \
    canhandler.h \
    canraw.h \
    drivingselect.h \
    dvselect.h \
    gui.h \
    logger.h \
    logs.h \
    mainwindow.h \
    servicemode.h \
    vehicle.h

FORMS += \
    canraw.ui \
    drivingselect.ui \
    dvselect.ui \
    logs.ui \
    mainwindow.ui \
    servicemode.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
