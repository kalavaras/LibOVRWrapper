// LibOVRWrapper.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "LibOVRWrapper.h"


// This is an example of an exported variable
LIBOVRWRAPPER_API int nLibOVRWrapper=0;

// This is an example of an exported function.
LIBOVRWRAPPER_API int fnLibOVRWrapper(void)
{
    return 42;
}

// This is the constructor of a class that has been exported.
// see LibOVRWrapper.h for the class definition
CLibOVRWrapper::CLibOVRWrapper()
{
    return;
}
