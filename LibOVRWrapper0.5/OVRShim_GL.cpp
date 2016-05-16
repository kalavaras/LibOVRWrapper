#include "stdafx.h"

#undef OVR_PUBLIC_FUNCTION
#undef OVR_PUBLIC_CLASS
#undef OVR_PRIVATE_FUNCTION
#undef OVR_PRIVATE_CLASS

#if !defined(OVR_DLL_BUILD)
#define OVR_DLL_BUILD
#endif

#include "../LibOVR0.5/Include/OVR_CAPI_0_5_0.h"
#include "../LibOVR0.5/Include/OVR_CAPI_GL.h"

#include "shimhelper.h"
