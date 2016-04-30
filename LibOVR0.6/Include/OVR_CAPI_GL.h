/********************************************************************************//**
\file      OVR_CAPI_GL.h
\brief     OpenGL-specific structures used by the CAPI interface.
\copyright Copyright 2013 Oculus VR, LLC. All Rights reserved.
************************************************************************************/

#ifndef OVR_CAPI_GL_h
#define OVR_CAPI_GL_h

#include "OVR_CAPI.h"


// We avoid gl.h #includes here which interferes with some users' use of alternatives and typedef GLuint manually.
typedef unsigned int GLuint;


#if defined(_MSC_VER)
    #pragma warning(push)
    #pragma warning(disable: 4324) // structure was padded due to __declspec(align())
#endif

/// Used to pass GL eye texture data to ovrHmd_EndFrame.
typedef struct ovrGLTextureData_s
{
    ovrTextureHeader Header;    ///< General device settings.
    GLuint           TexId;     ///< The OpenGL name for this texture.
} ovrGLTextureData;

OVR_STATIC_ASSERT(sizeof(ovrTexture) >= sizeof(ovrGLTextureData), "Insufficient size.");
OVR_STATIC_ASSERT(sizeof(ovrGLTextureData) == sizeof(ovrTextureHeader) + 4, "size mismatch");

/// Contains OpenGL-specific texture information.
typedef union ovrGLTexture_s
{
    ovrTexture       Texture;   ///< General device settings.
    ovrGLTextureData OGL;       ///< OpenGL-specific settings.
} ovrGLTexture;


#if defined(_MSC_VER)
    #pragma warning(pop)
#endif



/// Creates a Texture Set suitable for use with OpenGL.
///
/// Multiple calls to ovrHmd_CreateSwapTextureSetD3D11 for the same ovrHmd is supported, but applications
/// cannot rely on switching between ovrSwapTextureSets at runtime without a performance penalty.
///
/// \param[in]  hmd Specifies an ovrHmd previously returned by ovrHmd_Create.
/// \param[in]  format Specifies the texture format.
/// \param[in]  width Specifies the requested texture width.
/// \param[in]  height Specifies the requested texture height.
/// \param[out] outTextureSet Specifies the created ovrSwapTextureSet, which will be valid upon a successful return value, else it will be NULL.
///             This texture set must be eventually destroyed via ovrHmd_DestroySwapTextureSet before destroying the HMD with ovrHmd_Destroy.
///
/// \return Returns an ovrResult indicating success or failure. In the case of failure, use 
///         ovr_GetLastErrorInfo to get more information.
///
/// \see ovrHmd_DestroySwapTextureSet
///
OVR_PUBLIC_FUNCTION(ovrResult) ovrHmd_CreateSwapTextureSetGL(ovrHmd hmd, GLuint format,
                                                             int width, int height,
                                                             ovrSwapTextureSet** outTextureSet);


/// Creates a Mirror Texture which is auto-refreshed to mirror Rift contents produced by this application.
///
/// A second call to ovrHmd_CreateMirrorTextureGL for a given ovrHmd  before destroying the first one
/// is not supported and will result in an error return.
///
/// \param[in]  hmd Specifies an ovrHmd previously returned by ovrHmd_Create.
/// \param[in]  format Specifies the texture format.
/// \param[in]  width Specifies the requested texture width.
/// \param[in]  height Specifies the requested texture height.
/// \param[out] outMirrorTexture Specifies the created ovrSwapTexture, which will be valid upon a successful return value, else it will be NULL.
///             This texture must be eventually destroyed via ovrHmd_DestroyMirrorTexture before destroying the HMD with ovrHmd_Destroy.
///
/// \return Returns an ovrResult indicating success or failure. In the case of failure, use 
///         ovr_GetLastErrorInfo to get more information.
///
/// \see ovrHmd_DestroyMirrorTexture
///
OVR_PUBLIC_FUNCTION(ovrResult) ovrHmd_CreateMirrorTextureGL(ovrHmd hmd, GLuint format,
                                                            int width, int height,
                                                            ovrTexture** outMirrorTexture);

#endif    // OVR_CAPI_GL_h
