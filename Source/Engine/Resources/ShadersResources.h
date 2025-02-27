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

struct SVertexShaderData
{
    ID3DBlob* Blob = nullptr;
    ID3D11InputLayout* Input = nullptr;
    ID3D11VertexShader* Shader = nullptr;

    ~SVertexShaderData()
    {
        Input->Release();
        Shader->Release();
    }
};

struct SPixelShaderData
{
    ID3DBlob* Blob = nullptr;
    ID3D11PixelShader* Shader = nullptr;
    ID3D11Buffer* ConstantBuffer;
    ID3D11SamplerState* TextureSamplerState;

    ~SPixelShaderData()
    {
        Shader->Release();
        if (ConstantBuffer != nullptr) ConstantBuffer->Release();
        if (TextureSamplerState != nullptr) TextureSamplerState->Release();
    }
};

class MShaderResources
{
public:
    static ID3DBlob* CreateBlobFromFileName(const char*, EShaderType);
    static ID3DBlob* GetBlobFromFileName(const char*);
    static ID3DBlob* GetOrCreateBlobFromFileName(const char* _filename, EShaderType _shaderType);
public:
    static void DeleteAllBlobs();
private:
    static void CompileShader(const char*, const char*, ID3DBlob**);
};
