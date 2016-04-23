/********************************************************************************//**
\file      OVR_CAPI.h
\brief     Keys for CAPI proprty function calls
\copyright Copyright 2015 Oculus VR, LLC All Rights reserved.
************************************************************************************/

#ifndef OVR_CAPI_Keys_1_3_h
#define OVR_CAPI_Keys_1_3_h

#include "OVR_Version.h"



#define OVR_KEY_USER_1_3                        "User"                // string

#define OVR_KEY_NAME_1_3                        "Name"                // string

#define OVR_KEY_GENDER_1_3                      "Gender"              // string "Male", "Female", or "Unknown"
#define OVR_DEFAULT_GENDER_1_3                  "Unknown"

#define OVR_KEY_PLAYER_HEIGHT_1_3               "PlayerHeight"        // float meters
#define OVR_DEFAULT_PLAYER_HEIGHT_1_3           1.778f

#define OVR_KEY_EYE_HEIGHT_1_3                  "EyeHeight"           // float meters
#define OVR_DEFAULT_EYE_HEIGHT_1_3              1.675f

#define OVR_KEY_NECK_TO_EYE_DISTANCE_1_3        "NeckEyeDistance"     // float[2] meters
#define OVR_DEFAULT_NECK_TO_EYE_HORIZONTAL_1_3  0.0805f
#define OVR_DEFAULT_NECK_TO_EYE_VERTICAL_1_3    0.075f


#define OVR_KEY_EYE_TO_NOSE_DISTANCE_1_3        "EyeToNoseDist"       // float[2] meters





#define OVR_PERF_HUD_MODE_1_3                       "PerfHudMode"                       // int, allowed values are defined in enum ovrPerfHudMode

#define OVR_LAYER_HUD_MODE_1_3                      "LayerHudMode"                      // int, allowed values are defined in enum ovrLayerHudMode
#define OVR_LAYER_HUD_CURRENT_LAYER_1_3             "LayerHudCurrentLayer"              // int, The layer to show 
#define OVR_LAYER_HUD_SHOW_ALL_LAYERS_1_3           "LayerHudShowAll"                   // bool, Hide other layers when the hud is enabled

#define OVR_DEBUG_HUD_STEREO_MODE_1_3               "DebugHudStereoMode"                // int, allowed values are defined in enum ovrDebugHudStereoMode
#define OVR_DEBUG_HUD_STEREO_GUIDE_INFO_ENABLE_1_3  "DebugHudStereoGuideInfoEnable"     // bool
#define OVR_DEBUG_HUD_STEREO_GUIDE_SIZE_1_3         "DebugHudStereoGuideSize2f"         // float[2]
#define OVR_DEBUG_HUD_STEREO_GUIDE_POSITION_1_3     "DebugHudStereoGuidePosition3f"     // float[3]
#define OVR_DEBUG_HUD_STEREO_GUIDE_YAWPITCHROLL_1_3 "DebugHudStereoGuideYawPitchRoll3f" // float[3]
#define OVR_DEBUG_HUD_STEREO_GUIDE_COLOR_1_3        "DebugHudStereoGuideColor4f"        // float[4]



#endif // OVR_CAPI_Keys_h
