# Dash

The software for a raspberry-pi powered dash display.

### General project information

Written in Qt 6.2.3 on Ubuntu 21, targeted for Raspberry Pi OS 11.

##### External dependencies

The app uses an external library `libsocketcan`, which can be installed in a `libsocketcan-dev` package.

The app uses the [Nlohmanns's json library](https://github.com/nlohmann/json). Licensed under MIT, available in the project files in \Dash\lib.

The app uses the [d-bahr's crc c++ library](https://github.com/d-bahr/CRCpp).

## CAN bus connection

The app attempts to connect to the system CAN bus interface on startup. If the connection is successful, a `QObject connection` is created and a function `onFramesReceived()` is called every time a new frame is available. *See concurrency diagram below*.

### Data logging

Every incoming CAN frame (even these without a parser entry) is timestamped and logged to a `\*time\*-can.txt` file. 

If the app encounters an error, or an incoming frame has a `logger` parser attribute, it is timestamped and logged to the `\*time\*-log.txt` file.

The time is added to file names on app exit to simplify the code.

Both files have a header informing about the time of app launch.

### Program macros

The program behavior can be influenced with macros:

1) if `SEND_HEARTBEAT` is defined, the heartbeat frame will be sent @10Hz.
2) if `DATA_TIMEOUT_CHECK` is defined, the all synchronous frames are checked. If a frame has not been received for 10 times the expected interval, an error will be raised. 

## The GUI

### Main Window

The main window displays key vehicle data:

- speed
- RPM
- power
- can status
- battery level (SOC)

and lap times split into 3 sectors. Entering each sector is detected by the lap timer device and relayed over CAN.

Finishing each lap (entering the 1 sector) is relayed over CAN with measured total lap time. 

From the main window, two additional subwindows can be opened: ~~the driverless mission selection window~~ and the service mode window.

![Main Window](https://i.ibb.co/YLY0d7P/vision1.jpg)

##### Main Window navigation model

1. Pressing buttons 1 and 4 opens Driverless Selection
2. Pressing buttons 2 and 3 opens Service Mode
3. Pressing button 1 ends the current lap (until Lap Timer functionality is implemented)
4. Pressing button 2 resets the lap timer

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

![Service Mode](https://i.ibb.co/yfw9P51/version2service.jpg)


### Driverless mission selection window

The driverless mission selection window reads the missions' description, frame id and payload from a .xml file. All descriptions are then displayed and the driver can choose a mission using the "X" and "Y" buttons. Pressing "A" will accept a mission and a CAN frame with corresponding id and payload is relayed over the bus. A text "Sent!" will be displayed if the frame was sent successfully.

![Driverless mission selection](https://i.ibb.co/pW8ZNhq/Screenshot-from-2022-02-14-14-30-48.png)

##### Example xml file entry

```xml
<dvMissions>
    <mission description="Mission 1" frameID="101" framePayload="0000"></mission>
</dvMissions>
```

### Raw CAN data window

This window can display the CAN log while the app is running.

![Raw CAN data](https://i.ibb.co/0X1P8kq/Screenshot-from-2022-02-16-16-20-33.png)

### App and vehicle logs

This window can display the 'App and Vehicle log' while the app is running.

![App and vehicle logs](https://i.ibb.co/8P1zmdb/Screenshot-from-2022-02-16-16-11-58.png)

### Driving parameters selection window

The parameters that can be changed are:

- Traction Control (active/inactive)
- maximum slip ratio (0 - 100%)
- algorithm used in traction control (PI, PID, LQR, LQRI, SDRE, MPC)
- APPS curve (Linear, Wet, Acceleration)
- max power (0 - 100%)
- regen power (0 - 100%)
- traction control sensitivity (0 - 100%)

All values are loaded from a .csv file.

The current parameter is displayed in red and can be toggled with the "A" button. Pressing "X" changes the current value. The "Y" button confirms the change and sends a frame with corresponing id and payload. If the driver switches to another parameter or the window is closed before confirming the change, the settings' values are reset to display accurate data.

![Driving parameters selection](https://i.ibb.co/GCKG50j/Screenshot-from-2022-02-17-10-30-38.png)

##### Complete windows' schematic

![Complete windows' schematic](https://i.postimg.cc/3xMXQYy9/Windows-schematic.jpg)

### Project's class diagram

![Class diagram](https://i.ibb.co/ncWC3Zp/Screenshot-from-2022-02-22-10-28-02.png)

Every window class is inheriting an abstract class GUIComponent. The main window also inherits `QMainWindow` and every other window inherits `QDialog`. 
The logger class is a collection of log-related static function, that would be under a namespace logger, but a qt-related linker issue forced a static class as a workaround.

An incoming frame is first parsed in the `CanHandler` class and sent to the main window, where it is either processed there (if mainwindow is active) or sent to the current active subwindow via a pointer.

This design choice means that every window needs to exist at all times and be displayed asynchronously with `show()` rather than `exec()`.

This structure also means that some navigation data will need to be passed up to three times before affecting a window, but it allows for the navigation logic to be encapsulated within specific window (rather than having one class managing all windows and containing most of their logic). Moreover, it allows new features or windows to be added easily, as they contain their own logic.

### Incoming frame handling/Concurrent operation diagram

![Diagram](https://i.ibb.co/0BG4SZD/Dash-Concurrency-Diagram-Page-1-drawio.png)  

The shared memory is a collection of data structures present on the CAN bus. The main GUI thread reads from can and does a `memcpy` into the shared resources. (As a relatively low cost operation, it can be computed on the main GUI thread).

Then, every screen refresh cycle, that data is being interpreted on a secondary thread. Parameters are updated, errors checked and navigation processed.

Copies are made so that shared memory is locked for a short period of time (GUI calls take a long time to process). This way, if a 3rd process is needed (e.g. faster JSON updates), no major code changes are neccessary.