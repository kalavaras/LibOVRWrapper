#include "stdafx.h"
#include <unordered_map>
#include "d3d11.h"

#include "../LibOVR0.8/Include/OVR_CAPI_0_8_0.h"

extern "C" HRESULT wrapCreateShaderResourceView(ID3D11Device* device, ID3D11Resource* resource, ID3D11ShaderResourceView** srv) {
	return device->CreateShaderResourceView(resource, NULL, srv);
}

std::unordered_map<ovrSwapTextureSet*, ovrTextureSwapChain1_3*> *swapchains;
ovrMirrorTexture1_3* globalMirror;

extern "C" void setMirror(ovrMirrorTexture1_3* mirror) {
	globalMirror = mirror;
}

extern "C" ovrMirrorTexture1_3* getMirror() {
	return globalMirror;
}

extern "C" void initChains() {
	swapchains = new std::unordered_map<ovrSwapTextureSet*, ovrTextureSwapChain1_3*>();
}

extern "C" void removeChain(ovrSession1_3 session, ovrSwapTextureSet* ts) {
	swapchains->erase(ts);
}

extern "C" ovrTextureSwapChain1_3* getChain(ovrSession1_3 session, ovrSwapTextureSet* ts) {	
	return swapchains->find(ts)->second; //TODO
}

extern "C" void setChain(ovrSession1_3 session, ovrSwapTextureSet* ts, ovrTextureSwapChain1_3* chain) {		
	swapchains->emplace(ts, chain);
}