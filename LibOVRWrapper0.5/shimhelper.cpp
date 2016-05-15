#include "stdafx.h"
#include <unordered_map>
#include "d3d11.h"
#include "shimhelper.h"

#include "../LibOVR0.5/Include/OVR_CAPI_0_5_0.h"
#include "../LibOVR0.5/Include/OVR_CAPI_D3D.h"

extern "C" HRESULT wrapCreateShaderResourceView(ID3D11Device* device, ID3D11Resource* resource, ID3D11ShaderResourceView** srv) {
	return device->CreateShaderResourceView(resource, NULL, srv);
}

extern "C" void initChains() {
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