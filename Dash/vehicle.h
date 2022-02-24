#ifndef VEHICLE_H
#define VEHICLE_H
#include <QString>
enum class Parameter{Speed, RPM, SOC, Power, BmshvVoltage, BmslvVoltage,
              InverterTemp, EngineTemp, CoolantTemp, BmshvTemp, BmslvTemp, APPS};
enum class Status{Working, InError, Unresolved};    //probably not needed
enum class Window{Main, CAN, DV, Driving, Logs, ServiceMode, ChangeConfirm};    //will be used to decide GUI calls redirection
enum class Navigation{A, B, X, Y, LeftDial, RightDial};
enum class FrameType{error, update, navigation, confirmation};

#endif // VEHICLE_H
