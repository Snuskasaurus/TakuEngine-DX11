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

    ~SVertexShader()
    {
        Input->Release();
        Shader->Release();
    }
};

struct SPixelShader
{
    ID3DBlob* Blob = nullptr;
    ID3D11PixelShader* Shader = nullptr;
    ID3D11Buffer* ConstantBuffer;
    ID3D11SamplerState* TextureSamplerState;

    ~SPixelShader()
    {
        Shader->Release();
        ConstantBuffer->Release();
        TextureSamplerState->Release();
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
