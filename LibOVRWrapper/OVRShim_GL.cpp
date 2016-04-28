#include "stdafx.h"

#undef OVR_PUBLIC_FUNCTION
#undef OVR_PUBLIC_CLASS
#undef OVR_PRIVATE_FUNCTION
#undef OVR_PRIVATE_CLASS

#if !defined(OVR_DLL_BUILD)
#define OVR_DLL_BUILD
#endif

#include "../LibOVR0.8/Include/OVR_CAPI_0_8_0.h"
#include "../LibOVR0.8/Include/OVR_CAPI_GL.h"

#include "shimhelper.h"

OVR_PUBLIC_FUNCTION(ovrResult) ovr_CreateSwapTextureSetGL(ovrSession session, GLuint format,
	int width, int height,
	ovrSwapTextureSet** outTextureSet) {
	return -1;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_CreateMirrorTextureGL(ovrSession session, GLuint format,
	int width, int height,
	ovrTexture** outMirrorTexture) {
	return -1;
}