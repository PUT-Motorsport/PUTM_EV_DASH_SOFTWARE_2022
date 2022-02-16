#ifndef VEHICLE_H
#define VEHICLE_H
#include <QString>
enum Parameter{Speed, RPM, SOC, Power, BmshvVoltage, BmslvVoltage,
              InverterTemp, EngineTemp, CoolantTemp, BmshvTemp, BmslvTemp, APPS};
enum Status{Working, InError, Unresolved};
enum class Window{Main, CAN, DV, Driving, Logs, ServiceMode, ChangeConfirm};    //will be used to decide GUI calls redirection
enum Button{A, B, X, Y, LeftDial, RightDial};
enum FrameType{error, update, navigation};
struct Update {
    Parameter parameter;
    double value;
};
struct Error {
    QString part;
    int errorCode;
};

#endif // VEHICLE_H
