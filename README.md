# Dash Software
The software for a Raspberry-Pi powered dash display. (Will be) Written in Qt 6.2.3 on Ubuntu 21, targeted for Raspberry Pi OS 11.
### Dependencies
This app uses the external library `libsocketcan`; please ensure that the `libsocketcan-dev` package is installed.

### Proposed GUI look
![Main screen](https://i.ibb.co/T18HSnz/Screenshot-from-2022-02-14-14-29-39.png)
![Raw CAN data](https://i.ibb.co/0X1P8kq/Screenshot-from-2022-02-16-16-20-33.png)
![DV mission select](https://i.ibb.co/pW8ZNhq/Screenshot-from-2022-02-14-14-30-48.png)
![App and vehicle logs](https://i.ibb.co/8P1zmdb/Screenshot-from-2022-02-16-16-11-58.png)
![Driving parameters select](https://i.ibb.co/GCKG50j/Screenshot-from-2022-02-17-10-30-38.png)
![Service Mode](https://i.ibb.co/M9Fy0jH/Screenshot-from-2022-02-15-15-23-28.png)

### Features
The app will have a main menu displaying critical vehicle data: 
- speed
- rpm
- power supplied
- SOC and *range?*
- coolant temperature
- lap timer

And subwindows available when vehicle is stationary:
1. Raw CAN data
2. Logs
3. DV mode mission select
4. Driving parameters select
5. Service mode

### XML parser data

Parser data will be stored in a xml file. Every parameter will have a numerical prefix easily interpretable in the code, e.g. parameter="00:Speed", type="01:Update". The string after **:** will be disregarded by the parser and can be appended solely to improve human readability.
To be included in the logs, a frame needs to have a logger attribute, which text will be timestamped and sent to logs.

**A table with numerical prefixes for parts is appended to the repo as input.ods**

An error frame will have a type, format and logger attribute; the payload is the error code.
An update frame will have a type, parameter, format and logger attributes; the payload is the updated data.


##### Example XML document
```xml
<?xml version="1.0" encoding="UTF-8"?>
<canFrames>
   <frame id="01" type="01:Error" logger="Engine error reported"></frame>
   <frame id="02" type="02:Update" parameter="07:Speed" format="02:Unsigned Int"><frame>
   <frame id="56" type="02:Update" data="03:Charge level" format="04:float" logger="Charge level decreased"><frame>
   <frame id="127" type="03:Navigation" button="01:A">
</canFrames>
```

**A xml parser data file is in the repo (although it's a work in progress).**

### Steering wheel buttons functionality

(A complete steering model will be added)

Entering the service mode will require all buttons to be pressed.
Entering the dv mission select will require both upper buttons to be pressed.
Exiting a window will always be done with one and the same button.
Steering hints will appear on all screens except main.

Multiple button presses will be registered on the stm side.

### Service Mode panel

The service mode panel will display:
- BMSHV temperature and voltage
- BMSLV temperature and voltage
- inverter, coolant and engine temperature
- APPS value

And will be a gateway to access driving parameters, log and can data panel.

### Driverless mission select

A window with driverless mission select will be shown after upper buttons are pressed on both sides of the steering wheel. It will have a simple mission list and will send a CAN frame after selection.
The mission frames will be stored in a xml file.

```xml
<dvMissions>
    <mission description="Mission 1" frameID="101" framePayload="0000"></mission>
    <mission description="Mission 2" frameID="102" framePayload="0000"></mission>
    <mission description="Mission 3" frameID="103" framePayload="0000"></mission>
    <mission description="Mission 4" frameID="104" framePayload="0000"></mission>
    <mission description="Mission 5" frameID="105" framePayload="0000"></mission>
</dvMissions>
```

After mission selection, a frame with corresponding ID and payload will be relayed over CAN.

### Data and CAN logger

The vehicle status changes and raw CAN data will be logged to a .txt file to allow subwindows to display them later.
All raw CAN data will be logged. A vehicle status change will be logged if frame causing it has a <logger> attribute.

### Driving parameters select

This window will allow a selection of driving parameters. After accepting a new value, a frame will be sent to the bus.
List of all parameters that can be changed within this window:
1. APPS Curve
2. Traction Control
3. Energy regain
4. Fan setting
5. Max power


**Please do note that some parameters can be changed in motion via the dials and therefore do not require entering this mode. See below.**

### Parameters change confirmation window

**This mode is being reworked. Left only for reference**

![Driving parameters change confimration](https://i.ibb.co/Q6Smkvd/Screenshot-from-2022-02-16-16-14-44.png)


If a dial is turned, a confirmation window will appear. The driver can choose to accept or reject a change. Only after accepting a frame is sent to the bus causing the actual change.

- The left dial changes the ?.
- The right dial changes the ?.

### Classes

![Classes diagram](https://i.ibb.co/dG70Szd/Screenshot-from-2022-02-14-14-37-06.png)
*Will probably be changed*

The app will connect to CAN via system kernel and can-utils. A bash script will prepare the interface and launch the app on system startup.

### Incoming frame handling algorithm

![Incoming frame handling algorithm](https://i.ibb.co/SBYKQHN/Frame-Handling.jpg)

### Window choice/navigation algorithm

*Work in progress*