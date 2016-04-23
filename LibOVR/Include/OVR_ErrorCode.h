/********************************************************************************//**
\file  OVR_ErrorCode.h
\brief     This header provides LibOVR error code declarations.
\copyright Copyright 2015-2016 Oculus VR, LLC All Rights reserved.
*************************************************************************************/

#ifndef OVR_ErrorCode_1_3_h
#define OVR_ErrorCode_1_3_h


#include "OVR_Version.h"
#include <stdint.h>







#ifndef OVR_RESULT_DEFINED
#define OVR_RESULT_DEFINED ///< Allows ovrResult to be independently defined.
/// API call results are represented at the highest level by a single ovrResult.
typedef int32_t ovrResult;
#endif


/// \brief Indicates if an ovrResult indicates success.
///
/// Some functions return additional successful values other than ovrSucces and
/// require usage of this macro to indicate successs.
///
#if !defined(OVR_SUCCESS)
    #define OVR_SUCCESS(result) (result >= 0)
#endif


/// \brief Indicates if an ovrResult indicates an unqualified success.
///
/// This is useful for indicating that the code intentionally wants to
/// check for result == ovrSuccess as opposed to OVR_SUCCESS(), which
/// checks for result >= ovrSuccess.
///
#if !defined(OVR_UNQUALIFIED_SUCCESS)
    #define OVR_UNQUALIFIED_SUCCESS(result) (result == ovrSuccess)
#endif


/// \brief Indicates if an ovrResult indicates failure.
///
#if !defined(OVR_FAILURE)
    #define OVR_FAILURE(result) (!OVR_SUCCESS(result))
#endif


// Success is a value greater or equal to 0, while all error types are negative values.
#ifndef OVR_SUCCESS_DEFINED
#define OVR_SUCCESS_DEFINED ///< Allows ovrResult to be independently defined.
typedef enum ovrSuccessType1_3_
{
    /// This is a general success result. Use OVR_SUCCESS to test for success.
    ovrSuccess1_3 = 0,

    /// Returned from a call to SubmitFrame. The call succeeded, but what the app
    /// rendered will not be visible on the HMD. Ideally the app should continue
    /// calling SubmitFrame, but not do any rendering. When the result becomes
    /// ovrSuccess, rendering should continue as usual.
    ovrSuccess1_3_NotVisible                 = 1000,

    ovrSuccess1_3_HMDFirmwareMismatch        = 4100,   ///< The HMD Firmware is out of date but is acceptable.
    ovrSuccess1_3_TrackerFirmwareMismatch    = 4101,   ///< The Tracker Firmware is out of date but is acceptable.
    ovrSuccess1_3_ControllerFirmwareMismatch = 4104,   ///< The controller firmware is out of date but is acceptable.
    ovrSuccess1_3_TrackerDriverNotFound      = 4105,   ///< The tracker driver interface was not found. Can be a temporary error

} ovrSuccessType1_3;
#endif


typedef enum ovrErrorType1_3_
{
    /* General errors */
    ovrError1_3_MemoryAllocationFailure    = -1000,   ///< Failure to allocate memory.
    ovrError1_3_SocketCreationFailure      = -1001,   ///< Failure to create a socket.
    ovrError1_3_InvalidSession             = -1002,   ///< Invalid ovrSession parameter provided.
    ovrError1_3_Timeout                    = -1003,   ///< The operation timed out.
    ovrError1_3_NotInitialized             = -1004,   ///< The system or component has not been initialized.
    ovrError1_3_InvalidParameter           = -1005,   ///< Invalid parameter provided. See error info or log for details.
    ovrError1_3_ServiceError               = -1006,   ///< Generic service error. See error info or log for details.
    ovrError1_3_NoHmd                      = -1007,   ///< The given HMD doesn't exist.
    ovrError1_3_Unsupported                = -1009,   ///< Function call is not supported on this hardware/software
    ovrError1_3_DeviceUnavailable          = -1010,   ///< Specified device type isn't available.
    ovrError1_3_InvalidHeadsetOrientation  = -1011,   ///< The headset was in an invalid orientation for the requested operation (e.g. vertically oriented during ovr_RecenterPose).
    ovrError1_3_ClientSkippedDestroy       = -1012,   ///< The client failed to call ovr_Destroy on an active session before calling ovr_Shutdown. Or the client crashed.
    ovrError1_3_ClientSkippedShutdown      = -1013,   ///< The client failed to call ovr_Shutdown or the client crashed.

    /* Audio error range, reserved for Audio errors. */
    ovrError1_3_AudioReservedBegin         = -2000,   ///< First Audio error.
    ovrError1_3_AudioDeviceNotFound        = -2001,   ///< Failure to find the specified audio device.
    ovrError1_3_AudioComError              = -2002,   ///< Generic COM error.
    ovrError1_3_AudioReservedEnd           = -2999,   ///< Last Audio error.

    /* Initialization errors. */
    ovrError1_3_Initialize                 = -3000,   ///< Generic initialization error.
    ovrError1_3_LibLoad                    = -3001,   ///< Couldn't load LibOVRRT.
    ovrError1_3_LibVersion                 = -3002,   ///< LibOVRRT version incompatibility.
    ovrError1_3_ServiceConnection          = -3003,   ///< Couldn't connect to the OVR Service.
    ovrError1_3_ServiceVersion             = -3004,   ///< OVR Service version incompatibility.
    ovrError1_3_IncompatibleOS             = -3005,   ///< The operating system version is incompatible.
    ovrError1_3_DisplayInit                = -3006,   ///< Unable to initialize the HMD display.
    ovrError1_3_ServerStart                = -3007,   ///< Unable to start the server. Is it already running?
    ovrError1_3_Reinitialization           = -3008,   ///< Attempting to re-initialize with a different version.
    ovrError1_3_MismatchedAdapters         = -3009,   ///< Chosen rendering adapters between client and service do not match
    ovrError1_3_LeakingResources           = -3010,   ///< Calling application has leaked resources
    ovrError1_3_ClientVersion              = -3011,   ///< Client version too old to connect to service
    ovrError1_3_OutOfDateOS                = -3012,   ///< The operating system is out of date.
    ovrError1_3_OutOfDateGfxDriver         = -3013,   ///< The graphics driver is out of date.
    ovrError1_3_IncompatibleGPU            = -3014,   ///< The graphics hardware is not supported
    ovrError1_3_NoValidVRDisplaySystem     = -3015,   ///< No valid VR display system found.
    ovrError1_3_Obsolete                   = -3016,   ///< Feature or API is obsolete and no longer supported.
    ovrError1_3_DisabledOrDefaultAdapter   = -3017,   ///< No supported VR display system found, but disabled or driverless adapter found.
    ovrError1_3_HybridGraphicsNotSupported = -3018,   ///< The system is using hybrid graphics (Optimus, etc...), which is not support.
    ovrError1_3_DisplayManagerInit         = -3019,   ///< Initialization of the DisplayManager failed.
    ovrError1_3_TrackerDriverInit          = -3020,   ///< Failed to get the interface for an attached tracker

    /* Hardware errors */
    ovrError1_3_InvalidBundleAdjustment    = -4000,   ///< Headset has no bundle adjustment data.
    ovrError1_3_USBBandwidth               = -4001,   ///< The USB hub cannot handle the camera frame bandwidth.
    ovrError1_3_USBEnumeratedSpeed         = -4002,   ///< The USB camera is not enumerating at the correct device speed.
    ovrError1_3_ImageSensorCommError       = -4003,   ///< Unable to communicate with the image sensor.
    ovrError1_3_GeneralTrackerFailure      = -4004,   ///< We use this to report various sensor issues that don't fit in an easily classifiable bucket.
    ovrError1_3_ExcessiveFrameTruncation   = -4005,   ///< A more than acceptable number of frames are coming back truncated.
    ovrError1_3_ExcessiveFrameSkipping     = -4006,   ///< A more than acceptable number of frames have been skipped.
    ovrError1_3_SyncDisconnected           = -4007,   ///< The sensor is not receiving the sync signal (cable disconnected?).
    ovrError1_3_TrackerMemoryReadFailure   = -4008,   ///< Failed to read memory from the sensor.
    ovrError1_3_TrackerMemoryWriteFailure  = -4009,   ///< Failed to write memory from the sensor.
    ovrError1_3_TrackerFrameTimeout        = -4010,   ///< Timed out waiting for a camera frame.
    ovrError1_3_TrackerTruncatedFrame      = -4011,   ///< Truncated frame returned from sensor.
    ovrError1_3_TrackerDriverFailure       = -4012,   ///< The sensor driver has encountered a problem.
    ovrError1_3_TrackerNRFFailure          = -4013,   ///< The sensor wireless subsystem has encountered a problem.
    ovrError1_3_HardwareGone               = -4014,   ///< The hardware has been unplugged
    ovrError1_3_NordicEnabledNoSync        = -4015,   ///< The nordic indicates that sync is enabled but it is not sending sync pulses
    ovrError1_3_NordicSyncNoFrames         = -4016,   ///< It looks like we're getting a sync signal, but no camera frames have been received
    ovrError1_3_CatastrophicFailure        = -4017,   ///< A catastrophic failure has occurred.  We will attempt to recover by resetting the device

    ovrError1_3_HMDFirmwareMismatch        = -4100,   ///< The HMD Firmware is out of date and is unacceptable.
    ovrError1_3_TrackerFirmwareMismatch    = -4101,   ///< The sensor Firmware is out of date and is unacceptable.
    ovrError1_3_BootloaderDeviceDetected   = -4102,   ///< A bootloader HMD is detected by the service.
    ovrError1_3_TrackerCalibrationError    = -4103,   ///< The sensor calibration is missing or incorrect.
    ovrError1_3_ControllerFirmwareMismatch = -4104,   ///< The controller firmware is out of date and is unacceptable.

    ovrError1_3_IMUTooManyLostSamples      = -4200,   ///< Too many lost IMU samples.
    ovrError1_3_IMURateError               = -4201,   ///< IMU rate is outside of the expected range.
    ovrError1_3_FeatureReportFailure       = -4202,   ///< A feature report has failed.

    /* Synchronization errors */
    ovrError1_3_Incomplete                 = -5000,   ///<Requested async work not yet complete.
    ovrError1_3_Abandoned                  = -5001,   ///<Requested async work was abandoned and result is incomplete.

    /* Rendering errors */
    ovrError1_3_DisplayLost                = -6000,   ///<In the event of a system-wide graphics reset or cable unplug this is returned to the app.
    ovrError1_3_TextureSwapChainFull       = -6001,   ///<ovr_CommitTextureSwapChain was called too many times on a texture swapchain without calling submit to use the chain.
    ovrError1_3_TextureSwapChainInvalid    = -6002,   ///<The ovrTextureSwapChain is in an incomplete or inconsistent state. Ensure ovr_CommitTextureSwapChain was called at least once first.

    /* Fatal errors */
    ovrError1_3_RuntimeException           = -7000,   ///< A runtime exception occurred. The application is required to shutdown LibOVR and re-initialize it before this error state will be cleared.


    ovrError1_3_MetricsUnknownApp            = -90000,
    ovrError1_3_MetricsDuplicateApp          = -90001,
    ovrError1_3_MetricsNoEvents              = -90002,
    ovrError1_3_MetricsRuntime               = -90003,
    ovrError1_3_MetricsFile                  = -90004,
    ovrError1_3_MetricsNoClientInfo          = -90005,
    ovrError1_3_MetricsNoAppMetaData         = -90006,
    ovrError1_3_MetricsNoApp                 = -90007,
    ovrError1_3_MetricsOafFailure            = -90008,
    ovrError1_3_MetricsSessionAlreadyActive  = -90009,
    ovrError1_3_MetricsSessionNotActive      = -90010,

} ovrErrorType1_3;



/// Provides information about the last error.
/// \see ovr_GetLastErrorInfo
typedef struct ovrErrorInfo1_3_
{
    ovrResult Result;               ///< The result from the last API call that generated an error ovrResult.
    char      ErrorString[512];     ///< A UTF8-encoded null-terminated English string describing the problem. The format of this string is subject to change in future versions.
} ovrErrorInfo1_3;

#endif /* OVR_ErrorCode_h */
