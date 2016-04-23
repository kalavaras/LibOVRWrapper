/************************************************************************************
Filename    :   Win32_RoomTiny_Main.cpp
Content     :   First-person view test application for Oculus Rift
Created     :   11th May 2015
Authors     :   Tom Heath
Copyright   :   Copyright 2015 Oculus, Inc. All Rights reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*************************************************************************************/
/// This is an entry-level sample, showing a minimal VR sample, 
/// in a simple environment.  Use WASD keys to move around, and cursor keys.
/// Dismiss the health and safety warning by tapping the headset, 
/// or pressing any key. 
/// It runs with DirectX11.

// Include DirectX
#include "Win32_DirectXAppUtil.h"

// Include the Oculus SDK
#include "OVR_CAPI_D3D.h"


//------------------------------------------------------------
// ovrSwapTextureSet wrapper class that also maintains the render target views
// needed for D3D11 rendering.
struct OculusTexture
{
    ovrHmd                   hmd;
	ovrSwapTextureSet      * TextureSet;
    static const int         TextureCount = 2;
	ID3D11RenderTargetView * TexRtv[TextureCount];

    OculusTexture() :
        hmd(nullptr),
        TextureSet(nullptr)
    {
        TexRtv[0] = TexRtv[1] = nullptr;
    }

    bool Init(ovrHmd _hmd, int sizeW, int sizeH)
	{
        hmd = _hmd;

		D3D11_TEXTURE2D_DESC dsDesc;
		dsDesc.Width = sizeW;
		dsDesc.Height = sizeH;
		dsDesc.MipLevels = 1;
		dsDesc.ArraySize = 1;
        dsDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		dsDesc.SampleDesc.Count = 1;   // No multi-sampling allowed
		dsDesc.SampleDesc.Quality = 0;
		dsDesc.Usage = D3D11_USAGE_DEFAULT;
		dsDesc.CPUAccessFlags = 0;
		dsDesc.MiscFlags = 0;
		dsDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

		ovrResult result = ovr_CreateSwapTextureSetD3D11(hmd, DIRECTX.Device, &dsDesc, ovrSwapTextureSetD3D11_Typeless, &TextureSet);
        if (!OVR_SUCCESS(result))
            return false;

        VALIDATE(TextureSet->TextureCount == TextureCount, "TextureCount mismatch.");

		for (int i = 0; i < TextureCount; ++i)
		{
			ovrD3D11Texture* tex = (ovrD3D11Texture*)&TextureSet->Textures[i];
			D3D11_RENDER_TARGET_VIEW_DESC rtvd = {};
			rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			DIRECTX.Device->CreateRenderTargetView(tex->D3D11.pTexture, &rtvd, &TexRtv[i]);
		}

        return true;
    }

	~OculusTexture()
	{
		for (int i = 0; i < TextureCount; ++i)
        {
            Release(TexRtv[i]);
        }
		if (TextureSet)
        {
            ovr_DestroySwapTextureSet(hmd, TextureSet);
        }
	}

	void AdvanceToNextTexture()
	{
		TextureSet->CurrentIndex = (TextureSet->CurrentIndex + 1) % TextureSet->TextureCount;
	}
};

// return true to retry later (e.g. after display lost)
static bool MainLoop(bool retryCreate)
{
    // Initialize these to nullptr here to handle device lost failures cleanly
	ovrTexture     * mirrorTexture = nullptr;
	OculusTexture  * pEyeRenderTexture[2] = { nullptr, nullptr };
	DepthBuffer    * pEyeDepthBuffer[2] = { nullptr, nullptr };
    Scene          * roomScene = nullptr; 
    Camera         * mainCam = nullptr;
	D3D11_TEXTURE2D_DESC td = {};

	ovrHmd HMD;
	ovrGraphicsLuid luid;
	ovrResult result = ovr_Create(&HMD, &luid);
    if (!OVR_SUCCESS(result))
        return retryCreate;

    ovrHmdDesc hmdDesc = ovr_GetHmdDesc(HMD);

	// Setup Device and Graphics
	// Note: the mirror window can be any size, for this sample we use 1/2 the HMD resolution
    if (!DIRECTX.InitDevice(hmdDesc.Resolution.w / 2, hmdDesc.Resolution.h / 2, reinterpret_cast<LUID*>(&luid)))
        goto Done;

	// Make the eye render buffers (caution if actual size < requested due to HW limits). 
	ovrRecti         eyeRenderViewport[2];

	for (int eye = 0; eye < 2; ++eye)
	{
		ovrSizei idealSize = ovr_GetFovTextureSize(HMD, (ovrEyeType)eye, hmdDesc.DefaultEyeFov[eye], 1.0f);
		pEyeRenderTexture[eye] = new OculusTexture();
        if (!pEyeRenderTexture[eye]->Init(HMD, idealSize.w, idealSize.h))
        {
            if (retryCreate) goto Done;
	        VALIDATE(OVR_SUCCESS(result), "Failed to create eye texture.");
        }
		pEyeDepthBuffer[eye] = new DepthBuffer(DIRECTX.Device, idealSize.w, idealSize.h);
		eyeRenderViewport[eye].Pos.x = 0;
		eyeRenderViewport[eye].Pos.y = 0;
		eyeRenderViewport[eye].Size = idealSize;
        if (!pEyeRenderTexture[eye]->TextureSet)
        {
            if (retryCreate) goto Done;
            VALIDATE(false, "Failed to create texture.");
        }
	}

	// Create a mirror to see on the monitor.
	td.ArraySize = 1;
    td.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	td.Width = DIRECTX.WinSizeW;
	td.Height = DIRECTX.WinSizeH;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.SampleDesc.Count = 1;
	td.MipLevels = 1;
    result = ovr_CreateMirrorTextureD3D11(HMD, DIRECTX.Device, &td, 0, &mirrorTexture);
    if (!OVR_SUCCESS(result))
    {
        if (retryCreate) goto Done;
        VALIDATE(false, "Failed to create mirror texture.");
    }

	// Create the room model
    roomScene = new Scene(false);

	// Create camera
    mainCam = new Camera(&XMVectorSet(0.0f, 1.6f, 5.0f, 0), &XMQuaternionIdentity());

	// Setup VR components, filling out description
	ovrEyeRenderDesc eyeRenderDesc[2];
	eyeRenderDesc[0] = ovr_GetRenderDesc(HMD, ovrEye_Left, hmdDesc.DefaultEyeFov[0]);
	eyeRenderDesc[1] = ovr_GetRenderDesc(HMD, ovrEye_Right, hmdDesc.DefaultEyeFov[1]);

    bool isVisible = true;

	// Main loop
	while (DIRECTX.HandleMessages())
	{
		XMVECTOR forward = XMVector3Rotate(XMVectorSet(0, 0, -0.05f, 0), mainCam->Rot);
		XMVECTOR right   = XMVector3Rotate(XMVectorSet(0.05f, 0, 0, 0),  mainCam->Rot);
		if (DIRECTX.Key['W'] || DIRECTX.Key[VK_UP])	  mainCam->Pos = XMVectorAdd(mainCam->Pos, forward);
		if (DIRECTX.Key['S'] || DIRECTX.Key[VK_DOWN]) mainCam->Pos = XMVectorSubtract(mainCam->Pos, forward);
		if (DIRECTX.Key['D'])                         mainCam->Pos = XMVectorAdd(mainCam->Pos, right);
		if (DIRECTX.Key['A'])                         mainCam->Pos = XMVectorSubtract(mainCam->Pos, right);
		static float Yaw = 0;
		if (DIRECTX.Key[VK_LEFT])  mainCam->Rot = XMQuaternionRotationRollPitchYaw(0, Yaw += 0.02f, 0);
		if (DIRECTX.Key[VK_RIGHT]) mainCam->Rot = XMQuaternionRotationRollPitchYaw(0, Yaw -= 0.02f, 0);

		// Animate the cube
		static float cubeClock = 0;
		roomScene->Models[0]->Pos = XMFLOAT3(9 * sin(cubeClock), 3, 9 * cos(cubeClock += 0.015f));

		// Get both eye poses simultaneously, with IPD offset already included. 
		ovrPosef         EyeRenderPose[2];
		ovrVector3f      HmdToEyeViewOffset[2] = { eyeRenderDesc[0].HmdToEyeViewOffset,
			                                       eyeRenderDesc[1].HmdToEyeViewOffset };
        double frameTime = ovr_GetPredictedDisplayTime(HMD, 0);
        // Keeping sensorSampleTime as close to ovr_GetTrackingState as possible - fed into the layer
        double           sensorSampleTime = ovr_GetTimeInSeconds();
		ovrTrackingState hmdState = ovr_GetTrackingState(HMD, frameTime, ovrTrue);
		ovr_CalcEyePoses(hmdState.HeadPose.ThePose, HmdToEyeViewOffset, EyeRenderPose);

		// Render Scene to Eye Buffers
        if (isVisible)
        {
            for (int eye = 0; eye < 2; ++eye)
		    {
			    // Increment to use next texture, just before writing
			    pEyeRenderTexture[eye]->AdvanceToNextTexture();

			    // Clear and set up rendertarget
			    int texIndex = pEyeRenderTexture[eye]->TextureSet->CurrentIndex;
			    DIRECTX.SetAndClearRenderTarget(pEyeRenderTexture[eye]->TexRtv[texIndex], pEyeDepthBuffer[eye]);
			    DIRECTX.SetViewport((float)eyeRenderViewport[eye].Pos.x, (float)eyeRenderViewport[eye].Pos.y,
				    (float)eyeRenderViewport[eye].Size.w, (float)eyeRenderViewport[eye].Size.h);

			    //Get the pose information in XM format
			    XMVECTOR eyeQuat = XMVectorSet(EyeRenderPose[eye].Orientation.x, EyeRenderPose[eye].Orientation.y,
				                               EyeRenderPose[eye].Orientation.z, EyeRenderPose[eye].Orientation.w);
			    XMVECTOR eyePos = XMVectorSet(EyeRenderPose[eye].Position.x, EyeRenderPose[eye].Position.y, EyeRenderPose[eye].Position.z, 0);

			    // Get view and projection matrices for the Rift camera
			    XMVECTOR CombinedPos = XMVectorAdd(mainCam->Pos, XMVector3Rotate(eyePos, mainCam->Rot));
			    Camera finalCam(&CombinedPos, &(XMQuaternionMultiply(eyeQuat,mainCam->Rot)));
			    XMMATRIX view = finalCam.GetViewMatrix();
			    ovrMatrix4f p = ovrMatrix4f_Projection(eyeRenderDesc[eye].Fov, 0.2f, 1000.0f, ovrProjection_RightHanded);
			    XMMATRIX proj = XMMatrixSet(p.M[0][0], p.M[1][0], p.M[2][0], p.M[3][0],
				                            p.M[0][1], p.M[1][1], p.M[2][1], p.M[3][1],
				                            p.M[0][2], p.M[1][2], p.M[2][2], p.M[3][2],
				                            p.M[0][3], p.M[1][3], p.M[2][3], p.M[3][3]);
			    XMMATRIX prod = XMMatrixMultiply(view, proj);
			    roomScene->Render(&prod, 1, 1, 1, 1, true);
		    }
        }

		// Initialize our single full screen Fov layer.
        ovrLayerEyeFov ld = {};
		ld.Header.Type = ovrLayerType_EyeFov;
		ld.Header.Flags = 0;

		for (int eye = 0; eye < 2; ++eye)
		{
			ld.ColorTexture[eye] = pEyeRenderTexture[eye]->TextureSet;
			ld.Viewport[eye] = eyeRenderViewport[eye];
			ld.Fov[eye] = hmdDesc.DefaultEyeFov[eye];
			ld.RenderPose[eye] = EyeRenderPose[eye];
            ld.SensorSampleTime = sensorSampleTime;
		}

        ovrLayerHeader* layers = &ld.Header;
        result = ovr_SubmitFrame(HMD, 0, nullptr, &layers, 1);
        // exit the rendering loop if submit returns an error, will retry on ovrError_DisplayLost
        if (!OVR_SUCCESS(result))
            goto Done;

        isVisible = (result == ovrSuccess);

        // Render mirror
        ovrD3D11Texture* tex = (ovrD3D11Texture*)mirrorTexture;
        DIRECTX.Context->CopyResource(DIRECTX.BackBuffer, tex->D3D11.pTexture);
        DIRECTX.SwapChain->Present(0, 0);
	}

	// Release resources
Done:
    delete mainCam;
    delete roomScene;
	if (mirrorTexture) ovr_DestroyMirrorTexture(HMD, mirrorTexture);
    for (int eye = 0; eye < 2; ++eye)
    {
	    delete pEyeRenderTexture[eye];
        delete pEyeDepthBuffer[eye];
    }
	DIRECTX.ReleaseDevice();
	ovr_Destroy(HMD);

    // Retry on ovrError_DisplayLost
    return retryCreate || OVR_SUCCESS(result) || (result == ovrError_DisplayLost);
}

//-------------------------------------------------------------------------------------
int WINAPI WinMain(HINSTANCE hinst, HINSTANCE, LPSTR, int)
{
	// Initializes LibOVR, and the Rift
	ovrResult result = ovr_Initialize(nullptr);
	VALIDATE(OVR_SUCCESS(result), "Failed to initialize libOVR.");

    VALIDATE(DIRECTX.InitWindow(hinst, L"Oculus Room Tiny (DX11)"), "Failed to open window.");

    DIRECTX.Run(MainLoop);

	ovr_Shutdown();
	return(0);
}
