#pragma once

#include "stdafx.h"
#include "d3d11.h"

#include "../LibOVR0.6/Include/OVR_CAPI_0_6_0.h"

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

typedef struct ovrTextureSwapChainWrapper_
{
	ovrTextureSwapChain1_3 swapChain;
	int textureCount;
	ID3D11Texture2D** textures;	
	ID3D11DeviceContext* pContext;	
} ovrTextureSwapChainWrapper;

EXTERNC HRESULT wrapCreateShaderResourceView(ID3D11Device* device, ID3D11Resource* resource, ID3D11ShaderResourceView** srv);
EXTERNC ovrTextureSwapChainWrapper* getChain(ovrSession1_3 session, ovrSwapTextureSet* ts);
EXTERNC void setChain(ovrSession1_3 session, ovrSwapTextureSet* ts, ovrTextureSwapChainWrapper* chain);
EXTERNC void initChains();
EXTERNC void removeChain(ovrSession1_3 session, ovrSwapTextureSet* ts);
EXTERNC void setMirror(ovrMirrorTexture1_3* mirror);
EXTERNC ovrMirrorTexture1_3* getMirror();
EXTERNC ovrResult makeD3D11Texture(IUnknown* device,
	const D3D11_TEXTURE2D_DESC* desc,
	ID3D11Texture2D** outTexture);
EXTERNC void GetContext(ID3D11Device* device, ID3D11DeviceContext** context);
EXTERNC void CopyTexture(ID3D11DeviceContext* device, ID3D11Texture2D* dest, ovrTexture* src);

#undef EXTERNC