# Dash Software
The software for a Raspberry-Pi powered dash display. (Will be) Written in Qt 6.2.3.
### Proposed GUI look
![Main screen](https://i.ibb.co/T18HSnz/Screenshot-from-2022-02-14-14-29-39.png)
![Raw CAN data](https://i.ibb.co/Lr7LKHX/Screenshot-from-2022-02-14-14-29-27.png)
![DV mission select](https://i.ibb.co/pW8ZNhq/Screenshot-from-2022-02-14-14-30-48.png)
![App and vehicle logs](https://i.ibb.co/s36yyMX/Screenshot-from-2022-02-14-14-30-03.png)
![Driving parameters select](https://i.ibb.co/NxBqchD/Screenshot-from-2022-02-14-14-29-48.png)
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
The xml is the proposed format for storing the CAN parser data. Every parameter will have a numerical prefix easily interpretable in the code, e.g. sender="03:Engine", type="01:Update". An example table connecting numerical prefixes and components can be seen below. The string after **:** will be disregarded by the parser and can be appended solely to improve human readability.
To be included in the logs, a frame needs to have a logger attribute, which text will be timestamped and sent to logs
##### Example table with parts and their prefixes

| Part  | Prefix |
| ------------- | ------------- |
| 01  | Engine  |
| 02  | Tractive System  |
| 03 | Battery |
| 03 | Steering wheel |
##### Example XML document
```xml
<?xml version="1.0" encoding="UTF-8"?>
<canFrames>
   <frame id="01" type="01:Error" sender="01:Engine" logger="Engine error reported"></frame>
   <frame id="02" type="02:Update" sender="05:Resolver" data="07:Speed" format="02:Unsigned Int"><frame>
   <frame id="56" type="02:Update" sender="06:Battery" data="03:Charge level" format="04:float" logger="Charge level decreased"><frame>
</canFrames>
```
### Steering wheel buttons functionality
The app behavior will be controlled via the steering wheel. A steering model is not set yet.
### Service Mode panel
The service mode panel will display:
- BMSHV temperature and voltage
- BMSLV temperature and voltage
- inverter, coolant and engine temperature
- APPS value

And will be a gateway to access driving parameters, log and can data panel.
### Driverless mission select
A window with driverless mission select will be shown after (the condition is not yet set). It will have a simple mission list and will send a CAN frame after selection.
The mission frames will be stored in a xml file??
### Data and CAN logger
The vehicle status changes and raw CAN data will be logged to a .txt file to allow subwindows to display them later.
All raw CAN data will be logged. A vehicle status change will be logged if frame causing it has a <logger> attribute.
### Driving parameters select
This window will allow a selection of driving parameters, such as energy recovery level, APPS curves and traction control.
### Classes

![Classes diagram](https://i.ibb.co/dG70Szd/Screenshot-from-2022-02-14-14-37-06.png)
The app will connect to CAN via system kernel and can-utils. A bash script will prepare the interface and launch the app on system startup.
### GUI functionality
*Will be rewritten after a GUI-schematic is confirmed*