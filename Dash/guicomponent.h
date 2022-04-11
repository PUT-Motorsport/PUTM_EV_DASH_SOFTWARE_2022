#ifndef GUICOMPONENT_H
#define GUICOMPONENT_H

#include "vehicle.h"
#include "logger.h"
#include "canhandler.h"
extern Logger logger;
extern CanHandler canHandler;

class GUIComponent
{
public:
    virtual void navigate(buttonStates navigation) = 0;
    virtual void raiseError(QString const &errorMessage) = 0;
};

#endif // GUICOMPONENT_H
