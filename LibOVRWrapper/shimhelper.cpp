#include "stdafx.h"
#include "d3d11.h"

extern "C" HRESULT wrapCreateShaderResourceView(ID3D11Device* device, ID3D11Resource* resource, ID3D11ShaderResourceView** srv) {
	return device->CreateShaderResourceView(resource, NULL, srv);
}

ovrTextureSwapChain1_3* globalChain;

extern "C" ovrTextureSwapChain1_3* getChain(ovrSession1_3 session) {
	return globalChain; //TODO
}

extern "C" void setChain(ovrSession1_3 session, ovrTextureSwapChain1_3* chain) {
	globalChain = chain;
}