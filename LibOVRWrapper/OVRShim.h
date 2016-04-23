#pragma once

#include "stdafx.h"

#if !defined(OVR_DLL_BUILD)
#define OVR_DLL_BUILD
#endif

#include "../LibOVR0.8/Include/OVR_CAPI_0_8_0.h"

void copyPose(ovrPosef* dest, ovrPosef1_3* source);
void copyPoseState(ovrPoseStatef* dest, ovrPoseStatef1_3* source);