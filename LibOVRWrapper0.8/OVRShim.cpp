#include "stdafx.h"

#undef OVR_PUBLIC_FUNCTION
#undef OVR_PUBLIC_CLASS
#undef OVR_PRIVATE_FUNCTION
#undef OVR_PRIVATE_CLASS

#if !defined(OVR_DLL_BUILD)
	#define OVR_DLL_BUILD
#endif

#include "../LibOVR0.8/Include/OVR_CAPI_0_8_0.h"
#include "../LibOVR0.8/Include/OVR_CAPI_D3D.h"

#include "shimhelper.h"
#include "OVRShim.h"

OVR_PUBLIC_FUNCTION(ovrResult) ovr_Initialize(const ovrInitParams* params) {
	initChains();

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
		d.Type = (ovrHmdType)12;
	}
	else {
		d.Type = (ovrHmdType)desc.Type;
	}

	return d;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_Create(ovrSession* pSession, ovrGraphicsLuid* pLuid) {
	ovrResult r = ovr_Create1_3((ovrSession1_3*)pSession, (ovrGraphicsLuid1_3*)pLuid);

	if (!OVR_SUCCESS(r)) {
		return r;
	}

	ovr_SetTrackingOriginType1_3(*(ovrSession1_3*)pSession, ovrTrackingOrigin1_3_EyeLevel);

	return r;
}

OVR_PUBLIC_FUNCTION(void) ovr_Destroy(ovrSession session) {
	ovr_Destroy1_3((ovrSession1_3)session);
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_GetSessionStatus(ovrSession session, ovrSessionStatus* sessionStatus) {
	ovrSessionStatus1_3 status;

	ovrResult r = ovr_GetSessionStatus1_3((ovrSession1_3)session, &status);

	sessionStatus->HmdPresent = status.HmdPresent;
	sessionStatus->HasVrFocus = status.IsVisible;

	if (status.ShouldRecenter) {
		ovr_RecenterTrackingOrigin1_3((ovrSession1_3)session);

		//or ovr_ClearShouldRecenterFlag?
	}

	return r;
}

OVR_PUBLIC_FUNCTION(unsigned int) ovr_GetEnabledCaps(ovrSession session) {
	ovrHmdDesc1_3 desc = ovr_GetHmdDesc1_3((ovrSession1_3)session);

	//not possible anymore
	return desc.DefaultHmdCaps;
}

OVR_PUBLIC_FUNCTION(void) ovr_SetEnabledCaps(ovrSession session, unsigned int hmdCaps) {
	//not possible anymore
}

OVR_PUBLIC_FUNCTION(unsigned int) ovr_GetTrackingCaps(ovrSession session) {
	ovrHmdDesc1_3 desc = ovr_GetHmdDesc1_3((ovrSession1_3)session);

	return desc.DefaultTrackingCaps;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_ConfigureTracking(ovrSession session, unsigned int requestedTrackingCaps,
	unsigned int requiredTrackingCaps) {
	//not used anymore
	return ovrSuccess1_3;
}

OVR_PUBLIC_FUNCTION(void) ovr_RecenterPose(ovrSession session) {
	ovr_RecenterTrackingOrigin1_3((ovrSession1_3)session);
}

void copyPose(ovrPosef* dest, const ovrPosef1_3* source) {
	dest->Orientation = source->Orientation;
	dest->Position = source->Position;
}

void copyPoseR(ovrPosef1_3* dest, const ovrPosef* source) {
	dest->Orientation = source->Orientation;
	dest->Position = source->Position;
}

void copyPoseState(ovrPoseStatef* dest, const ovrPoseStatef1_3* source) {
	dest->AngularAcceleration = source->AngularAcceleration;
	dest->AngularVelocity = source->AngularVelocity;
	dest->LinearAcceleration = source->LinearAcceleration;
	dest->LinearVelocity = source->LinearVelocity;
	copyPose(&(dest->ThePose), &(source->ThePose));
	dest->TimeInSeconds = source->TimeInSeconds;
}

OVR_PUBLIC_FUNCTION(ovrTrackingState) ovr_GetTrackingState(ovrSession session, double absTime, ovrBool latencyMarker) {
	ovrTrackingState1_3 state = ovr_GetTrackingState1_3((ovrSession1_3)session, absTime, latencyMarker);
	ovrTrackerPose1_3 tpose = ovr_GetTrackerPose1_3((ovrSession1_3)session, 0);

	ovrTrackingState r;	
	copyPose(&(r.CameraPose), &(tpose.Pose));
	r.CameraPose.Orientation = tpose.Pose.Orientation;
	r.CameraPose.Position = tpose.Pose.Position;
	copyPoseState(&(r.HandPoses[0]), &(state.HandPoses[0]));
	copyPoseState(&(r.HandPoses[1]), &(state.HandPoses[1]));

	r.HandStatusFlags[0] = state.HandStatusFlags[0];
	r.HandStatusFlags[1] = state.HandStatusFlags[1];

	copyPoseState(&(r.HeadPose), &(state.HeadPose));

	//r.LastCameraFrameCounter not filled

	copyPose(&(r.LeveledCameraPose), &(tpose.LeveledPose));

	//r.RawSensorData not filled
	r.StatusFlags = state.StatusFlags | ovrStatus_CameraPoseTracked | ovrStatus_PositionConnected | ovrStatus_HmdConnected;
	
	return r;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_GetInputState(ovrSession session, unsigned int controllerTypeMask, ovrInputState* inputState) {
	ovrInputState1_3 state;

	ovrResult res = ovr_GetInputState1_3((ovrSession1_3)session, (ovrControllerType1_3)controllerTypeMask, &state);

	if (res < 0) {
		return res;
	}

	inputState->Buttons = state.Buttons; // needs mapping?
	inputState->ConnectedControllerTypes = ovr_GetConnectedControllerTypes1_3((ovrSession1_3)session);
	inputState->HandTrigger[0] = state.HandTrigger[0];
	inputState->HandTrigger[1] = state.HandTrigger[1];

	inputState->IndexTrigger[0] = state.IndexTrigger[0];
	inputState->IndexTrigger[1] = state.IndexTrigger[1];

	inputState->Thumbstick[0] = state.Thumbstick[0];
	inputState->Thumbstick[1] = state.Thumbstick[1];

	inputState->TimeInSeconds = state.TimeInSeconds;
	inputState->Touches = state.Touches;

	return res;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_SetControllerVibration(ovrSession session, unsigned int controllerTypeMask,
	float frequency, float amplitude) {
	return ovr_SetControllerVibration1_3((ovrSession1_3)session, (ovrControllerType1_3)controllerTypeMask, frequency, amplitude);
}

OVR_PUBLIC_FUNCTION(void) ovr_DestroySwapTextureSet(ovrSession session, ovrSwapTextureSet* textureSet) {	
	ovr_DestroyTextureSwapChain1_3((ovrSession1_3)session, getChain((ovrSession1_3)session, textureSet)->swapChain);

	removeChain((ovrSession1_3)session, textureSet);
}

OVR_PUBLIC_FUNCTION(void) ovr_DestroyMirrorTexture(ovrSession session, ovrTexture* mirrorTexture) {
	ovrMirrorTexture1_3* mirror = getMirror();

	ovr_DestroyMirrorTexture1_3((ovrSession1_3)session, *mirror);

	setMirror(NULL);

	if (mirrorTexture->Header.API == ovrRenderAPI_D3D11) {
		union ovrD3D11Texture* ovrtext = (union ovrD3D11Texture*)mirrorTexture;

		ovrtext->D3D11.pTexture->Release();
	}
}

OVR_PUBLIC_FUNCTION(ovrSizei) ovr_GetFovTextureSize(ovrSession session, ovrEyeType eye, ovrFovPort fov,
	float pixelsPerDisplayPixel) {
	ovrFovPort1_3 fport;
	fport.DownTan = fov.DownTan;
	fport.LeftTan = fov.LeftTan;
	fport.RightTan = fov.RightTan;
	fport.UpTan = fov.UpTan;

	return ovr_GetFovTextureSize1_3((ovrSession1_3)session, (ovrEyeType1_3)eye, fport, pixelsPerDisplayPixel);
}

OVR_PUBLIC_FUNCTION(ovrEyeRenderDesc) ovr_GetRenderDesc(ovrSession session,
	ovrEyeType eyeType, ovrFovPort fov) {

	ovrFovPort1_3 fport;
	fport.DownTan = fov.DownTan;
	fport.LeftTan = fov.LeftTan;
	fport.RightTan = fov.RightTan;
	fport.UpTan = fov.UpTan;

	ovrEyeRenderDesc1_3 desc = ovr_GetRenderDesc1_3((ovrSession1_3)session, (ovrEyeType1_3)eyeType, fport);

	ovrEyeRenderDesc r;

	r.DistortedViewport = desc.DistortedViewport;
	r.Eye = (ovrEyeType)desc.Eye;
	r.Fov.DownTan = desc.Fov.DownTan;
	r.Fov.LeftTan = desc.Fov.LeftTan;
	r.Fov.RightTan = desc.Fov.RightTan;
	r.Fov.UpTan = desc.Fov.UpTan;
	r.HmdToEyeViewOffset = desc.HmdToEyeOffset;
	r.PixelsPerTanAngleAtCenter = desc.PixelsPerTanAngleAtCenter;

	return r;
}

ovrTextureSwapChain1_3 renderChain(ovrSession1_3 session, ovrSwapTextureSet* ts)
{
	ovrTextureSwapChainWrapper* chainwrapper = getChain(session, ts);

	int currentIndex = 0;
	ovr_GetTextureSwapChainCurrentIndex1_3(session, chainwrapper->swapChain, &currentIndex);

	CopyTexture(chainwrapper->pContext, chainwrapper->textures[currentIndex], &ts->Textures[ts->CurrentIndex]);	
	
	ovr_CommitTextureSwapChain1_3(session, chainwrapper->swapChain);

	return chainwrapper->swapChain;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_SubmitFrame(ovrSession session, long long frameIndex,
	const ovrViewScaleDesc* viewScaleDesc,
	ovrLayerHeader const * const * layerPtrList, unsigned int layerCount) {

	//ovrLayerType 2, 6 do not exists anymore. max layer count is 16 instead of 32

	unsigned int trueLayerCount = 0;
	for (unsigned int i = 0;i < layerCount;i++) {
		if (layerPtrList[i] != NULL) {
			trueLayerCount++;
		}
	}

	if (trueLayerCount > 16) {
		return ovrError_RuntimeException;
	}	

	ovrLayerHeader1_3** newlayers = (ovrLayerHeader1_3**)malloc(sizeof(ovrLayerHeader1_3*) * trueLayerCount);
	
	unsigned int np = 0;

	for (unsigned int i = 0;i < layerCount;i++) {
		const ovrLayerHeader* layer = layerPtrList[i];

		if (layer == NULL) {
			continue;
		}

		if (layer->Type == ovrLayerType_EyeFov) {
			const ovrLayerEyeFov* oldelayer = (const ovrLayerEyeFov*)layer;
			ovrLayerEyeFov1_3 *elayer = (ovrLayerEyeFov1_3*)malloc(sizeof(ovrLayerEyeFov1_3));

			elayer->ColorTexture[0] = renderChain((ovrSession1_3)session, oldelayer->ColorTexture[0]);
			elayer->ColorTexture[1] = renderChain((ovrSession1_3)session, oldelayer->ColorTexture[1]);
			
			elayer->Fov[0].DownTan = oldelayer->Fov[0].DownTan;
			elayer->Fov[0].LeftTan = oldelayer->Fov[0].LeftTan;
			elayer->Fov[0].UpTan = oldelayer->Fov[0].UpTan;
			elayer->Fov[0].RightTan = oldelayer->Fov[0].RightTan;
			elayer->Fov[1].DownTan = oldelayer->Fov[1].DownTan;
			elayer->Fov[1].LeftTan = oldelayer->Fov[1].LeftTan;
			elayer->Fov[1].UpTan = oldelayer->Fov[1].UpTan;
			elayer->Fov[1].RightTan = oldelayer->Fov[1].RightTan;

			elayer->Header.Flags = oldelayer->Header.Flags;
			elayer->Header.Type = (ovrLayerType1_3)oldelayer->Header.Type;

			copyPoseR(&elayer->RenderPose[0], &oldelayer->RenderPose[0]);
			copyPoseR(&elayer->RenderPose[1], &oldelayer->RenderPose[1]);

			elayer->SensorSampleTime = oldelayer->SensorSampleTime;
			elayer->Viewport[0] = oldelayer->Viewport[0];
			elayer->Viewport[1] = oldelayer->Viewport[1];

			newlayers[np] = (ovrLayerHeader1_3*)elayer;
		}
		else if (layer->Type == ovrLayerType_EyeMatrix) {
			const ovrLayerEyeMatrix* oldelayer = (const ovrLayerEyeMatrix*)layer;
			ovrLayerEyeMatrix1_3 *elayer = (ovrLayerEyeMatrix1_3*)malloc(sizeof(ovrLayerEyeMatrix1_3));

			elayer->ColorTexture[0] = renderChain((ovrSession1_3)session, oldelayer->ColorTexture[0]);
			elayer->ColorTexture[1] = renderChain((ovrSession1_3)session, oldelayer->ColorTexture[1]);

			elayer->Matrix[0] = oldelayer->Matrix[0];
			elayer->Matrix[1] = oldelayer->Matrix[1];			

			elayer->Header.Flags = oldelayer->Header.Flags;
			elayer->Header.Type = (ovrLayerType1_3)oldelayer->Header.Type;

			copyPoseR(&elayer->RenderPose[0], &oldelayer->RenderPose[0]);
			copyPoseR(&elayer->RenderPose[1], &oldelayer->RenderPose[1]);

			elayer->SensorSampleTime = oldelayer->SensorSampleTime;
			elayer->Viewport[0] = oldelayer->Viewport[0];
			elayer->Viewport[1] = oldelayer->Viewport[1];

			newlayers[np] = (ovrLayerHeader1_3*)elayer;
		}
		else if (layer->Type == ovrLayerType_Quad) {
			const ovrLayerQuad* oldelayer = (const ovrLayerQuad*)layer;
			ovrLayerQuad1_3 *elayer = (ovrLayerQuad1_3*)malloc(sizeof(ovrLayerQuad1_3));

			elayer->ColorTexture = renderChain((ovrSession1_3)session, oldelayer->ColorTexture);
			
			copyPoseR(&elayer->QuadPoseCenter, &oldelayer->QuadPoseCenter);

			elayer->QuadSize = oldelayer->QuadSize;

			newlayers[np] = (ovrLayerHeader1_3*)elayer;
		}
		else if (layer->Type == ovrLayerType_Disabled) {			
			ovrLayerHeader1_3 *elayer = (ovrLayerHeader1_3*)malloc(sizeof(ovrLayerHeader1_3));

			elayer->Flags = layer->Flags;
			elayer->Type = (ovrLayerType1_3)layer->Type;

			newlayers[np] = (ovrLayerHeader1_3*)elayer;
		}
		else {
			return ovrError_InvalidParameter;
		}

		np++;
	}
	
	ovrResult r = ovr_SubmitFrame1_3((ovrSession1_3)session, frameIndex, (const ovrViewScaleDesc1_3*)viewScaleDesc, newlayers, trueLayerCount);

	for (unsigned int i = 0;i < trueLayerCount;i++) {
		free(newlayers[i]);
	}

	free(newlayers);

	return r;
}

OVR_PUBLIC_FUNCTION(double) ovr_GetPredictedDisplayTime(ovrSession session, long long frameIndex) {
	return ovr_GetPredictedDisplayTime1_3((ovrSession1_3)session, frameIndex);
}

OVR_PUBLIC_FUNCTION(double) ovr_GetTimeInSeconds() {
	return ovr_GetTimeInSeconds1_3();
}

OVR_PUBLIC_FUNCTION(void) ovr_ResetBackOfHeadTracking(ovrSession session) {
	//nothing
}

OVR_PUBLIC_FUNCTION(void) ovr_ResetMulticameraTracking(ovrSession session) {
	//nothing
}

OVR_PUBLIC_FUNCTION(ovrBool) ovr_GetBool(ovrSession session, const char* propertyName, ovrBool defaultVal) {
	return ovr_GetBool1_3((ovrSession1_3)session, propertyName, defaultVal);
}

OVR_PUBLIC_FUNCTION(ovrBool) ovr_SetBool(ovrSession session, const char* propertyName, ovrBool value) {
	return ovr_SetBool1_3((ovrSession1_3)session, propertyName, value);
}

OVR_PUBLIC_FUNCTION(int) ovr_GetInt(ovrSession session, const char* propertyName, int defaultVal) {
	return ovr_GetInt1_3((ovrSession1_3)session, propertyName, defaultVal);
}

OVR_PUBLIC_FUNCTION(ovrBool) ovr_SetInt(ovrSession session, const char* propertyName, int value) {
	return ovr_SetInt1_3((ovrSession1_3)session, propertyName, value);
}

OVR_PUBLIC_FUNCTION(float) ovr_GetFloat(ovrSession session, const char* propertyName, float defaultVal) {
	if (strcmp(propertyName, OVR_KEY_IPD) == 0) {
		float values[2];
		ovr_GetFloatArray1_3((ovrSession1_3)session, OVR_KEY_NECK_TO_EYE_DISTANCE_1_3, values, 2);

		return values[0] + values[1];
	}

	return ovr_GetFloat1_3((ovrSession1_3)session, propertyName, defaultVal);
}

OVR_PUBLIC_FUNCTION(ovrBool) ovr_SetFloat(ovrSession session, const char* propertyName, float value) {
	if (strcmp(propertyName, OVR_KEY_IPD) == 0) {
		return ovrFalse;
	}

	return ovr_SetFloat1_3((ovrSession1_3)session, propertyName, value);
}

OVR_PUBLIC_FUNCTION(unsigned int) ovr_GetFloatArray(ovrSession session, const char* propertyName,
	float values[], unsigned int valuesCapacity) {
	return ovr_GetFloatArray1_3((ovrSession1_3)session, propertyName, values, valuesCapacity);
}

OVR_PUBLIC_FUNCTION(ovrBool) ovr_SetFloatArray(ovrSession session, const char* propertyName,
	const float values[], unsigned int valuesSize) {
	return ovr_SetFloatArray1_3((ovrSession1_3)session, propertyName, values, valuesSize);
}

OVR_PUBLIC_FUNCTION(const char*) ovr_GetString(ovrSession session, const char* propertyName,
	const char* defaultVal) {
	return ovr_GetString1_3((ovrSession1_3)session, propertyName, defaultVal);
}

OVR_PUBLIC_FUNCTION(ovrBool) ovr_SetString(ovrSession session, const char* propertyName,
	const char* value) {
	return ovr_SetString1_3((ovrSession1_3)session, propertyName, value);
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_SetQueueAheadFraction(ovrSession session, float queueAheadFraction) {
	return ovr_SetQueueAheadFraction1_3((ovrSession1_3)session, queueAheadFraction);
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_Lookup(const char* name, void** data) {
	return ovr_Lookup1_3(name, data);
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_GetTextureSwapChainCurrentIndex(ovrSession session, ovrSwapTextureSet* textureSet, int* currentIndex) {
	ovrTextureSwapChain1_3 chain = getChain((ovrSession1_3)session, textureSet)->swapChain;

	return ovr_GetTextureSwapChainCurrentIndex1_3((ovrSession1_3)session, chain, currentIndex);
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_CommitTextureSwapChain(ovrSession session, ovrSwapTextureSet* textureSet) {
	return ovr_CommitTextureSwapChain1_3((ovrSession1_3)session, getChain((ovrSession1_3)session, textureSet)->swapChain);
}