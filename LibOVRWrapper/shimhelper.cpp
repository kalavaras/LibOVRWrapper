#include "stdafx.h"
#include "d3d11.h"

extern "C" HRESULT wrapCreateShaderResourceView(ID3D11Device* device, ID3D11Resource* resource, ID3D11ShaderResourceView** srv) {
	return device->CreateShaderResourceView(resource, NULL, srv);
}

extern "C" ovrTextureSwapChain1_3 getChain(ovrSession1_3 session) {
	return NULL; //TODO
}
