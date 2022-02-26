# Dash

The software for a raspberry-pi powered dash display.

### General project information

Written in Qt 6.2.3 on Ubuntu 21, targeted for Raspberry Pi OS 11.

##### External dependencies

The app uses an external library called `libsocketcan`, which can be installed in a `libsocketcan-dev` package.
The app connects to the system CAN interface. A script will open the interface and launch the app on raspberry pi system startup.

## CAN bus connection

The app attempts to connect to the system CAN bus interface on startup. If the connection is successful, a `QObject connection` is created and a function `onFramesReceived()` is called every time a new frame is available. The frame is then parsed into one of three types: Update, Error and Navigation and a signal is sent to the GUI to inform about required changes. 

### Data logging

Every incoming CAN frame (even these without a parser entry) is timestamped and logged to a `\*time\*-can.txt` file. 

If the app encounters an error, or an incoming frame has a `logger` parser attribute, it is timestamped and logged to the `\*time\*-log.txt` file.

The time is added to file names on app exit to simplify the code.

Both files have a header informing about the time of app launch.

## The GUI

### Main Window

The main window displays key vehicle data:

- speed
- RPM
- power
- can status
- battery level (SOC)

and has a lap timer. The timer starts automatically as soon as vehicle movement is detected. The driver can end the lap, using the "X" button on the steering wheel. The current time is then cleared and the best time can be updated. After pressing the "Y" button, the timer can be hard-reset. If the vehicle is in motion, the hard-reset will clear the best timer and restart the current timer. If the vehicle is stopped, both timers will be cleared, and the app will wait until the vehicle starts moving again to start counting.

From the main window, two additional subwindows can be opened: the driverless mission select window and the service mode window.

![Main Window](https://i.ibb.co/T18HSnz/Screenshot-from-2022-02-14-14-29-39.png)

### Driverless mission selection window

The driverless mission selection window reads the missions' description, frame id and payload from a .xml file. All descriptions are then displayed and the driver can choose a mission using the "X" and "Y" buttons. Pressing "A" will accept a mission and a CAN frame with corresponding id and payload is relayed over the bus. A text "Sent!" will be displayed if the frame was sent successfully.

![Driverless mission selection](https://i.ibb.co/pW8ZNhq/Screenshot-from-2022-02-14-14-30-48.png)

##### Example xml file entry

```xml
<dvMissions>
    <mission description="Mission 1" frameID="101" framePayload="0000"></mission>
</dvMissions>
```

### Service mode window

The service mode window displays additional vehicle parameters:

- BMSHV and BMSLV voltage
- BMSHV and BMSLV temperature
- coolant, engine and inverter temperature
- apps value

From the service mode window, the driver can access three additional windows:

- Raw CAN data window
- App and vehicle log window
- Driving parameters selection window

![Service Mode](https://i.ibb.co/M9Fy0jH/Screenshot-from-2022-02-15-15-23-28.png)

### Raw CAN data window

This window can display the CAN log while the app is running.

**Right now, the app is able only to sniff the data from the static class logger. A way to load data from the files will be found**

![Raw CAN data](https://i.ibb.co/0X1P8kq/Screenshot-from-2022-02-16-16-20-33.png)

### App and vehicle logs

This window can display the 'App and Vehicle log' while the app is running.

![App and vehicle logs](https://i.ibb.co/8P1zmdb/Screenshot-from-2022-02-16-16-11-58.png)

### Driving parameters selection window

The parameters that can be changed here are:

- energy regain
- traction control
- max power
- fan setting
- APPS curve

All values, frame ids and payloads are loaded from a .csv file.

The current parameter is displayed in red and can be toggled with the "A" button. Pressing "X" changes the current value. The "Y" button confirms the change and sends a frame with corresponing id and payload. If the driver switches to another parameter or the window is closed before confirming the change, the settings' values are reset to display accurate data.

![Driving parameters selection](https://i.ibb.co/GCKG50j/Screenshot-from-2022-02-17-10-30-38.png)

### Driving parameters change confirmation window

If the driver changes the value of one of the steering wheel dials, this window will appear. It will load all parameters from a .xml file:
- the description of setting changed
- the id of the confirmation frame

This type of frames has a new type "03:Confirmation".

The payload of these frames is treated as a new value of the setting and is set to the outgoing frame.

If the driver accepts or rejects the change, this window is closed immediately.

If a frame could not be sent, this window stays open for as long as the driver rejects the change or the frame is finally sent.

##### Complete windows' schematic

![Complete windows' schematic](https://i.postimg.cc/3xMXQYy9/Windows-schematic.jpg)

### Project's class diagram

![Class diagram](https://i.ibb.co/ncWC3Zp/Screenshot-from-2022-02-22-10-28-02.png)

Every window class is inheriting an abstract class GUIComponent. The main window also inherits `QMainWindow` and every other window inherits `QDialog`. 
The logger class is a collection of log-related static function, that would be under a namespace logger, but a qt-related linker issue forced a static class as a workaround.

An incoming frame is first parsed in the `CanHandler` class and sent to the main window, where it is either processed there (if mainwindow is active) or sent to the current active subwindow via a pointer.

This design choice means that every window needs to exist at all times and be displayed asynchronously with `show()` rather than `exec()`.

This structure also means that some navigation data will need to be passed up to three times before affecting a window, but it allows for the navigation logic to be encapsulated within specific window (rather than having one class managing all windows and containing most of their logic). Moreover, it allows new features or windows to be added easily, as they contain their own logic.

### Incoming frame handling algorithm  
  
![Incoming frame handling algorithm](https://i.ibb.co/XX954xT/frame-algorithm.jpg)