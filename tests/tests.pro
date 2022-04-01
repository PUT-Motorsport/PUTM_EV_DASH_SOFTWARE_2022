include(gtest_dependency.pri)

QT       += core gui
QT += xml
QT += serialbus
QT += concurrent
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11


SOURCES += \
        main.cpp         tst_test.cpp
