#ifndef GUICOMPONENT_H
#define GUICOMPONENT_H

#include "vehicle.h"

class GUIComponent
{
public:
    virtual void navigate(Navigation pressed) = 0;
    virtual void raiseError(int errorCode, QString const &errorMessage) = 0;
};

#endif // GUICOMPONENT_H
