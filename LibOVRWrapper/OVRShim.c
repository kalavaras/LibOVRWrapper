#include <string.h>
#include "../LibOVR/Include/OVR_CAPI.h"

#if !defined(OVR_DLL_BUILD)
	#define OVR_DLL_BUILD
#endif

#include "../LibOVR0.8/Include/OVR_CAPI_0_8_0.h"

OVR_PUBLIC_FUNCTION(ovrResult) ovr_Initialize(const ovrInitParams* params) {
	return ovr_Initialize1_3((ovrInitParams1_3*)params);
}

OVR_PUBLIC_FUNCTION(void) ovr_Shutdown() {
	ovr_Shutdown1_3();
}

OVR_PUBLIC_FUNCTION(void) ovr_GetLastErrorInfo(ovrErrorInfo* errorInfo) {
	ovr_GetLastErrorInfo1_3((ovrErrorInfo1_3*)errorInfo);
}

OVR_PUBLIC_FUNCTION(const char*) ovr_GetVersionString() {
	return "OculusSDK0.8";
}

OVR_PUBLIC_FUNCTION(int) ovr_TraceMessage(int level, const char* message) {
	return ovr_TraceMessage1_3(level, message);
}

OVR_PUBLIC_FUNCTION(ovrHmdDesc) ovr_GetHmdDesc(ovrSession session) {
	ovrHmdDesc1_3 desc = ovr_GetHmdDesc1_3((ovrSession1_3)session);

	ovrHmdDesc d;

	d.AvailableHmdCaps = desc.AvailableHmdCaps;
	d.AvailableTrackingCaps = desc.AvailableTrackingCaps;
	
	ovrTrackerDesc1_3 tracker = ovr_GetTrackerDesc1_3((ovrSession1_3)session, 0);

	d.CameraFrustumFarZInMeters = tracker.FrustumFarZInMeters;
	d.CameraFrustumHFovInRadians = tracker.FrustumHFovInRadians;
	d.CameraFrustumNearZInMeters = tracker.FrustumNearZInMeters;
	d.CameraFrustumVFovInRadians = tracker.FrustumVFovInRadians;
	
	memcpy(d.DefaultEyeFov, desc.DefaultEyeFov, sizeof(d.DefaultEyeFov));
	d.DefaultHmdCaps = desc.DefaultHmdCaps;
	d.DefaultTrackingCaps = desc.DefaultTrackingCaps;
	d.DisplayRefreshRate = desc.DisplayRefreshRate;
	d.FirmwareMajor = desc.FirmwareMajor;
	d.FirmwareMinor = desc.FirmwareMinor;
	strncpy_s(d.Manufacturer, sizeof(d.Manufacturer), desc.Manufacturer, sizeof(d.Manufacturer) / sizeof(d.Manufacturer[0]));
	memcpy(d.MaxEyeFov, desc.MaxEyeFov, sizeof(d.MaxEyeFov));
	
	d.ProductId = desc.ProductId;
	strncpy_s(d.ProductName, sizeof(d.ProductName), desc.ProductName, sizeof(d.ProductName) / sizeof(d.ProductName[0]));
	d.Resolution = desc.Resolution;
	
	strncpy_s(d.SerialNumber, sizeof(d.SerialNumber), desc.SerialNumber, sizeof(d.SerialNumber) / sizeof(d.SerialNumber[0]));
	d.VendorId = desc.VendorId;

	if (desc.Type > 12) {
		d.Type = 12;
	}
	else {
		d.Type = desc.Type;
	}

	return d;
}