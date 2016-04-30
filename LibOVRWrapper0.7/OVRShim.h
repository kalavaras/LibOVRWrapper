#pragma once

#include "stdafx.h"

#if !defined(OVR_DLL_BUILD)
#define OVR_DLL_BUILD
#endif

#include "../LibOVR0.7/Include/OVR_CAPI_0_7_0.h"

void copyPose(ovrPosef* dest, const ovrPosef1_3* source);
void copyPoseState(ovrPoseStatef* dest, const ovrPoseStatef1_3* source);