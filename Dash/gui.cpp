#include "gui.h"

GUI::GUI(QObject *parent)
    : QObject{parent}
{
    canHandler = new CanHandler();
    //connect all canhandler slots
    QObject::connect(canHandler, &CanHandler::updateGUI, this, &GUI::updateGUI);
    QObject::connect(canHandler, &CanHandler::navigation, this, &GUI::navigate);
    QObject::connect(canHandler, &CanHandler::raiseError, this, &GUI::raiseError);

    canHandler->connect();

    currentWindow = Window::Main;
    mainWindow = new MainWindow();
    logs = new Logs();
    canRaw = new CanRaw();
    dvSelect = new DvSelect();  //add connection with canHandler (or pass a pointer to canHandler???)
    drivingSelect = new DrivingSelect();
    serviceMode = new ServiceMode();
    changeConfirm = new ChangeConfirm();
    currentWindowPtr = nullptr;
    mainWindow->show();
//    changeWindow(Window::Driving);  //use this line for testing

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

void GUI::updateGUI(Parameter param, qreal newValue)
{
    if (onMainWindow.contains(param))
        mainWindow->updateData(param, newValue);
    else
        serviceMode->updateData(param, newValue);
    return;
}

void GUI::raiseError(int errorCode, QString errorString)
{
    return;
}

void GUI::navigate(Navigation button)
{
    return;
}

void GUI::changeWindow(Window newWindow)
{
    //to be rewritten (or at least rethought) after gui is confirmed
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
    case Window::ChangeConfirm:
        currentWindowPtr = changeConfirm;
        changeConfirm->show();
        break;
    default:
        Logger::add("Something went wrong in the changeWindow function");
    }
}
