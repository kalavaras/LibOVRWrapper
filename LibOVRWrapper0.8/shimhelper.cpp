#include "stdafx.h"
#include <unordered_map>
#include "d3d11.h"
#include "shimhelper.h"

#include "../LibOVR0.8/Include/OVR_CAPI_0_8_0.h"
#include "../LibOVR0.8/Include/OVR_CAPI_D3D.h"

extern "C" HRESULT wrapCreateShaderResourceView(ID3D11Device* device, ID3D11Resource* resource, ID3D11ShaderResourceView** srv) {
	return device->CreateShaderResourceView(resource, NULL, srv);
}

std::unordered_map<ovrSwapTextureSet*, ovrTextureSwapChainWrapper*> *swapchains;
ovrMirrorTexture1_3* globalMirror;

extern "C" void setMirror(ovrMirrorTexture1_3* mirror) {
	globalMirror = mirror;
}

extern "C" ovrMirrorTexture1_3* getMirror() {
	return globalMirror;
}

extern "C" void initChains() {
	swapchains = new std::unordered_map<ovrSwapTextureSet*, ovrTextureSwapChainWrapper*>();
}

extern "C" void removeChain(ovrSession1_3 session, ovrSwapTextureSet* ts) {
	swapchains->erase(ts);
}

extern "C" ovrTextureSwapChainWrapper* getChain(ovrSession1_3 session, ovrSwapTextureSet* ts) {
	return swapchains->find(ts)->second; //TODO
}

extern "C" void setChain(ovrSession1_3 session, ovrSwapTextureSet* ts, ovrTextureSwapChainWrapper* chain) {
	swapchains->emplace(ts, chain);
}

extern "C" ovrResult makeD3D11Texture(IUnknown* device,
	const D3D11_TEXTURE2D_DESC* desc,
	ID3D11Texture2D** outTexture) {

	ID3D11Device* pDevice;
	HRESULT hr;
	hr = device->QueryInterface(&pDevice);

	if (FAILED(hr))
		return ovrError_RuntimeException;

	hr = pDevice->CreateTexture2D(desc, NULL, outTexture);

	pDevice->Release();

	if (FAILED(hr))
		return ovrError_RuntimeException;
	
	return ovrSuccess1_3;
}

extern "C" void CopyTexture(ID3D11DeviceContext* device, ID3D11Texture2D* dest, ovrTexture* src) {	
	union ovrD3D11Texture* ovrtext = (union ovrD3D11Texture*)src;

	/*ID3D11Device* pDevice;
	dest->GetDevice(&pDevice);
	ID3D11DeviceContext* pContext;
	pDevice->GetImmediateContext(&pContext);
	*/
	device->CopyResource(dest, ovrtext->D3D11.pTexture);

	//pDevice->Release();
	//pContext->Release();
}

extern "C" void GetContext(ID3D11Device* device, ID3D11DeviceContext** context) {
	device->GetImmediateContext(context);
}