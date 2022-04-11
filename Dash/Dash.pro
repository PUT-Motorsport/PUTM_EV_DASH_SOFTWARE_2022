QT       += core gui
QT += xml
QT += serialbus
QT += concurrent
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    canhandler.cpp \
    canraw.cpp \
    changeconfirm.cpp \
    drivingselect.cpp \
    dvselect.cpp \
    guihandler.cpp \
    logger.cpp \
    logs.cpp \
    main.cpp \
    mainwindow.cpp \
    servicemode.cpp

HEADERS += \
    CanHeaders/PM08-CANBUS-APPS.hpp \
    CanHeaders/PM08-CANBUS-AQ_CARD.hpp \
    CanHeaders/PM08-CANBUS-BMS_HV.hpp \
    CanHeaders/PM08-CANBUS-BMS_LV.hpp \
    CanHeaders/PM08-CANBUS-DASH.hpp \
    CanHeaders/PM08-CANBUS-LAP_TIMER.hpp \
    CanHeaders/PM08-CANBUS-SF.hpp \
    CanHeaders/PM08-CANBUS-STEERING_WHEEL.hpp \
    CanHeaders/PM08-CANBUS-TC.hpp \
    CanHeaders/PM08-CANBUS-TELEMETRY.hpp \
    canhandler.h \
    canraw.h \
    changeconfirm.h \
    drivingselect.h \
    dvselect.h \
    guicomponent.h \
    guihandler.h \
    lib/json.hpp \
    logger.h \
    logs.h \
    mainwindow.h \
    servicemode.h \
    vehicle.h

FORMS += \
    canraw.ui \
    changeconfirm.ui \
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
