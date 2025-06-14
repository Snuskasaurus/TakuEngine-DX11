#pragma once

#include <d3d11.h>
#include "../Math.h"
#include "../Resources/ShadersResources.h"
#include "../Defines.h"


#define MAX_INSTANCE_COUNT 1024u

struct alignas(16) b00_vs_buffer_sceneEachFrame
{
    TMatrix4f cameraViewProjection;
    TMatrix4f lightViewMatrix;
    TMatrix4f lightProjectionMatrix;
};

struct alignas(16) b01_vs_buffer_object
{
    TMatrix4f world[MAX_INSTANCE_COUNT];
};

struct alignas(16) b13_vs_buffer_debug_draw_line
{
    struct debug_line
    {
        TMatrix4f wvp[2];
        float r, g, b;
    };
    debug_line debugLines[416];
};

struct alignas(16) b00_ps_buffer_sceneEachFrame
{
    TVector3f camDir;
    TVector3f lightDir;
    TVector3f lightColor;
    float lightAmbientIntensity;
    float lightColorIntensity;
};

struct SShaderBufferHolder
{
    ID3D11Buffer* Buffer = nullptr;
    EShaderType ShaderType = EShaderType::INVALID;
    UINT Slot = 0u;
#if DEBUG_DO_CHECK_IN_SHADER_BUFFER
    UINT SizeBuffer = 0u;
#endif

    void CreateShaderBuffer(EShaderType _shaderType, UINT _slot, UINT _sizeStruct);
    void Release();

    static void FillBuffer_VS_Object(SShaderBufferHolder*, const TTransform* _transforms, UINT _start, UINT _nbInstances);
    static void FillBuffer_VS_SceneEachFrame(SShaderBufferHolder*, bool _isViewLight);
    static void FillBuffer_VS_DebugLine(SShaderBufferHolder* _shaderBufferHolder, const struct SDebugLine* _debugLines, UINT _start, UINT _nbInstances);
    static void FillBuffer_PS_SceneEachFrame(SShaderBufferHolder*);
};
