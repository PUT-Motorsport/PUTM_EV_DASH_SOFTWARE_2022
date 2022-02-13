# Dash Software
The software for a Raspberry-Pi powered dash display. Written in Qt 6.2.3.
### Proposed GUI look
*Placeholder for an image*
### Features
The app will have a main menu displaying critical vehicle data: 
- speed
- rpm
- battery level and temperature
- icons to display errors of key components:
    1. CAN status
    2. Engine status
    3. Tractive system status
    4. Battery status
- icons to display Tractive System Modes???

And subwindows available when vehicle is stationary:
1. Raw CAN data
2. Logs
3. Error panel
4. Temperatures breakdown for all components

### XML parser data
Proposed format for parser data:
```xml
<?xml version="1.0" encoding="UTF-8"?>
<canFrames>
   <frame>
      <id>01</id>
      <type>Error</type>
      <sender>Engine</sender>
      <logger>Engine error reported</logger>
   </frame>
   <frame>
      <id>02</id>
      <type>Update</type>
      <sender>Resolver</sender>
      <data>Speed</data>
   </frame>
   <frame>
      <id>03</id>
      <type>Enumeric</type>
      <sender>Traction Control</sender>
      <state>01:State1</state>
      <state>02:State2</state>
      <state>03:State3</state>
      <logger>Traction Control State Change</logger>
   </frame>
</canFrames>
```
### Classes
![Classes diagram](https://i.ibb.co/f8dHRjV/Classes-Breakdown-dash.png)
The app will connect to CAN via system kernel and can-utils. A bash script will prepare the interface and launch the app on system startup.
### GUI functionality
The app will handle the GUI via MainWIndow class, which will provide a frontend to all display changes. The class will have member objects of other, smaller classes such as SpeedDial to handle specific, low-level graphical operations.
The MainMenu class will also have DialogWindow classes to handle subwindow display.
### Data and CAN logger
The vehicle status changes and raw CAN data will be logged to a .txt file to allow subwindows to display them later.
All raw CAN data will be logged. A vehicle status change will be logged if frame causing it has a <logger> attribute.
