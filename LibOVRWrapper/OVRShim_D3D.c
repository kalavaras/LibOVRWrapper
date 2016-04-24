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

ovrTextureFormat1_3 getOVRFormat(DXGI_FORMAT format) {
	switch (format) {
		case DXGI_FORMAT_B5G6R5_UNORM:
			return OVR_FORMAT_B5G6R5_UNORM;
		case DXGI_FORMAT_B5G5R5A1_UNORM:
			return OVR_FORMAT_B5G5R5A1_UNORM;
		case DXGI_FORMAT_B4G4R4A4_UNORM:
			return OVR_FORMAT_B4G4R4A4_UNORM;
		case DXGI_FORMAT_R8G8B8A8_UNORM:
			return OVR_FORMAT_R8G8B8A8_UNORM;
		case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
			return OVR_FORMAT_R8G8B8A8_UNORM_SRGB;
		case DXGI_FORMAT_B8G8R8A8_UNORM:
			return OVR_FORMAT_B8G8R8A8_UNORM;
		case DXGI_FORMAT_B8G8R8A8_UNORM_SRGB:
			return OVR_FORMAT_B8G8R8A8_UNORM_SRGB;
		case DXGI_FORMAT_B8G8R8X8_UNORM:
			return OVR_FORMAT_B8G8R8X8_UNORM;
		case DXGI_FORMAT_B8G8R8X8_UNORM_SRGB:
			return OVR_FORMAT_B8G8R8X8_UNORM_SRGB;
		case DXGI_FORMAT_R16G16B16A16_FLOAT:
			return OVR_FORMAT_R16G16B16A16_FLOAT;
		case DXGI_FORMAT_D16_UNORM:
			return OVR_FORMAT_D16_UNORM;
		case DXGI_FORMAT_D24_UNORM_S8_UINT:
			return OVR_FORMAT_D24_UNORM_S8_UINT;
		case DXGI_FORMAT_D32_FLOAT:
			return OVR_FORMAT_D32_FLOAT;
		case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
			return OVR_FORMAT_D32_FLOAT_S8X24_UINT;
	}
	
	return OVR_FORMAT_UNKNOWN;		
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_CreateSwapTextureSetD3D11(ovrSession session,
	ID3D11Device* device,
	const D3D11_TEXTURE2D_DESC* desc,
	unsigned int miscFlags,
	ovrSwapTextureSet** outTextureSet) {
	
	ovrTextureSwapChainDesc1_3 d;
	d.Type = ovrTexture_2D;
	d.ArraySize = desc->ArraySize;
	
	d.Format = getOVRFormat(desc->Format);
	
	d.Width = desc->Width;
	d.Height = desc->Height;
	d.MipLevels = desc->MipLevels;
	d.SampleCount = desc->SampleDesc.Count;
	
	d.MiscFlags = 0;
	if (miscFlags & ovrSwapTextureSetD3D11_Typeless) {
		d.MiscFlags |= ovrTextureMisc_DX_Typeless;
	}
	
	BOOL makeShaderView = 0;
	d.BindFlags = 0;
	if (desc->BindFlags & D3D11_BIND_RENDER_TARGET) {
		d.BindFlags |= ovrTextureBind_DX_RenderTarget;
	}
	if (desc->BindFlags & D3D11_BIND_UNORDERED_ACCESS) {
		d.BindFlags |= ovrTextureBind_DX_UnorderedAccess;
	}
	if (desc->BindFlags & D3D11_BIND_DEPTH_STENCIL) {
		d.BindFlags |= ovrTextureBind_DX_DepthStencil;
	}
	if (desc->BindFlags & D3D11_BIND_SHADER_RESOURCE) {
		makeShaderView = 1;
	}	
	
	d.StaticImage = ovrFalse;

	ovrTextureSwapChain1_3* chain = (ovrTextureSwapChain1_3*)malloc(sizeof(ovrTextureSwapChain1_3));

	ovrResult result = ovr_CreateTextureSwapChainDX1_3((ovrSession1_3)session, (IUnknown*)device, &d, chain);

	if (!OVR_SUCCESS(result))
		return result;
	
	ovrSwapTextureSet* ts = (ovrSwapTextureSet*)malloc(sizeof(ovrSwapTextureSet));

	setChain((ovrSession1_3)session, ts, chain);

	int count = 0;
	ovr_GetTextureSwapChainLength1_3((ovrSession1_3)session, *chain, &count);

	ts->TextureCount = count;
	ts->CurrentIndex = count - 1;
	//union ovrD3D11Texture* texts = calloc(count, sizeof(ovrTexture));
	ts->Textures = calloc(count, sizeof(union ovrD3D11Texture));

	for (int i = 0; i < count; ++i)
	{
		ID3D11Texture2D* texture = 0;
		ovr_GetTextureSwapChainBufferDX1_3((ovrSession1_3)session, *chain, i, IID_ID3D11Texture2D, &texture);

		union ovrD3D11Texture* ovrtext = (union ovrD3D11Texture*)&ts->Textures[i];

		ovrtext->D3D11.pTexture = texture;

		if (makeShaderView) {
			wrapCreateShaderResourceView(device, (ID3D11Resource*)texture, &ovrtext->D3D11.pSRView);
		}		

		ovrtext->D3D11.Header.API = ovrRenderAPI_D3D11;
		ovrtext->D3D11.Header.TextureSize.w = d.Width;
		ovrtext->D3D11.Header.TextureSize.h = d.Height;
	}
	
	*outTextureSet = ts;	

	return result;
}

OVR_PUBLIC_FUNCTION(ovrResult) ovr_CreateMirrorTextureD3D11(ovrSession session,
	ID3D11Device* device,
	const D3D11_TEXTURE2D_DESC* desc,
	unsigned int miscFlags,
	ovrTexture** outMirrorTexture) {

	ovrMirrorTextureDesc1_3 d;
	
	d.Format = getOVRFormat(desc->Format);

	d.Width = desc->Width;
	d.Height = desc->Height;

	d.MiscFlags = 0;
	if (miscFlags & ovrSwapTextureSetD3D11_Typeless) {
		d.MiscFlags |= ovrTextureMisc_DX_Typeless;
	}

	ovrMirrorTexture1_3* mirror = (ovrMirrorTexture1_3*)malloc(sizeof(ovrMirrorTexture1_3));

	ovrResult result = ovr_CreateMirrorTextureDX1_3((ovrSession1_3)session, (IUnknown*)device, &d, mirror);

	if (!OVR_SUCCESS(result))
		return result;

	union ovrD3D11Texture* ovrtext = malloc(sizeof(union ovrD3D11Texture));

	ID3D11Texture2D* texture = 0;	
	ovr_GetMirrorTextureBufferDX1_3((ovrSession1_3)session, *mirror, IID_ID3D11Texture2D, &texture);

	ovrtext->D3D11.pTexture = texture;

	wrapCreateShaderResourceView(device, (ID3D11Resource*)texture, &ovrtext->D3D11.pSRView);

	ovrtext->D3D11.Header.API = ovrRenderAPI_D3D11;
	ovrtext->D3D11.Header.TextureSize.w = d.Width;
	ovrtext->D3D11.Header.TextureSize.h = d.Height;

	*outMirrorTexture = (ovrTexture*)ovrtext;

	setMirror(mirror);

	return result;

}