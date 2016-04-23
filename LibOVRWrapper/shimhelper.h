#pragma once

#include "stdafx.h"
#include "d3d11.h"

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

EXTERNC HRESULT wrapCreateShaderResourceView(ID3D11Device* device, ID3D11Resource* resource, ID3D11ShaderResourceView** srv);
EXTERNC ovrTextureSwapChain1_3 getChain(ovrSession1_3 session);

#undef EXTERNC