// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// reference additional headers your program requires here
#include "../LibOVR/Include/OVR_CAPI.h"
#include "../LibOVR/Include/OVR_Version.h"
#include "../LibOVR/Include/OVR_ErrorCode.h"

#include "../LibOVR/Include/OVR_CAPI_D3D.h"   
#include "../LibOVR/Include/OVR_CAPI_GL.h"  