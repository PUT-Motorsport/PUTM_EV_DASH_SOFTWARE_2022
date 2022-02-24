#ifndef GUICOMPONENT_H
#define GUICOMPONENT_H

#include "vehicle.h"

class GUIComponent
{
public:
    virtual void navigate(Navigation pressed) = 0;
    virtual void raiseError(QString const &errorMessage, int errorCode = -1) = 0;
};

#endif // GUICOMPONENT_H
