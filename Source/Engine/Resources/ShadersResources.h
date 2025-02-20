#pragma once

#include <Windows.h>
#include <d3d11.h>

#include "../IncludesExternal.h"
#include "MeshResources.h"

enum EShaderType
{
    VERTEX_SHADER,
    PIXEL_SHADER,
};

struct SVertexShader
{
    ID3DBlob* Blob = nullptr;
    ID3D11InputLayout* Input = nullptr;
    ID3D11VertexShader* Shader = nullptr;

    void Release()
    {
        Input->Release();
        Input = nullptr;

        Shader->Release();
        Shader = nullptr;
    }
};

struct SPixelShader
{
    ID3DBlob* Blob = nullptr;
    ID3D11PixelShader* Shader = nullptr;
    ID3D11Buffer* ConstantBuffer;
    ID3D11SamplerState* TextureSamplerState;

    void Release()
    {
        Shader->Release();
        Shader = nullptr;
        
        ConstantBuffer->Release();
        ConstantBuffer = nullptr;
        
        TextureSamplerState->Release();
        TextureSamplerState = nullptr;
    }
};

class MShaderResources
{
public:
    static ID3DBlob* CreateBlobFromFileName(const char*, EShaderType);
    static ID3DBlob* GetBlobFromFileName(const char*);
public:
    static void DeleteAllBlobs();
private:
    static void CompileShader(const char*, const char*, ID3DBlob**);
};
