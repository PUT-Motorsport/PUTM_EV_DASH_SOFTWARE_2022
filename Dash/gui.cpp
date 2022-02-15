#include "gui.h"

GUI::GUI(QObject *parent)
    : QObject{parent}
{
    canHandler = new CanHandler();
    canHandler->connect();
    currentWindow = Window::Main;
    mainWindow = new MainWindow();
    logs = new Logs();
    canRaw = new CanRaw();
    dvSelect = new DvSelect();  //add connection with canHandler (or pass a pointer to canHandler???)
    drivingSelect = new DrivingSelect();
    serviceMode = new ServiceMode();
    currentWindowPtr = nullptr;
    mainWindow->show();
    changeWindow(Window::CAN);
    changeWindow(Window::ServiceMode);
}

GUI::~GUI()
{
    delete canHandler;
    delete mainWindow;
    delete logs;
    delete canRaw;
    delete dvSelect;
    delete drivingSelect;
    delete serviceMode;
}

void GUI::updateGUI(Update update)
{
    return;
}

void GUI::raiseError(Error error)
{
    return;
}

void GUI::navigate(Button button)
{
    return;
}

void GUI::changeWindow(Window newWindow)
{
    if (currentWindowPtr == nullptr) {
        mainWindow->hide();
    }
    else {
        currentWindowPtr->hide();   //hides every window inheriting QDialog
    }

    currentWindow = newWindow;
    switch (currentWindow) {    //replace with showFullScreen for pi
    case Window::Main:
        currentWindowPtr = nullptr;
        mainWindow->show();
        break;
    case Window::CAN:
        currentWindowPtr = canRaw;
        canRaw->show();
        break;
    case Window::DV:
        currentWindowPtr = dvSelect;
        dvSelect->show();
        break;
    case Window::Logs:
        currentWindowPtr = logs;
        logs->show();
        break;
    case Window::Driving:
        currentWindowPtr = drivingSelect;
        drivingSelect->show();
        break;
    case Window::ServiceMode:
        currentWindowPtr = serviceMode;
        serviceMode->show();
        break;
    default:
        Logger::add("Something went wrong in the changeWindow function");
    }
}
