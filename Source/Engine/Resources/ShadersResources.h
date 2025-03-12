#pragma once

#include <Windows.h>
#include <d3d11.h>

#include "../IncludesExternal.h"
#include "MeshResources.h"

enum EShaderType
{
    INVALID,
    VERTEX_SHADER,
    PIXEL_SHADER,
};

namespace VS_INPUT_DESC
{
    constexpr D3D11_INPUT_ELEMENT_DESC POS_INST[] = {
        { "POSITION",       0u,  DXGI_FORMAT_R32G32B32_FLOAT,   0u,      0u,     D3D11_INPUT_PER_VERTEX_DATA,      0u },
        { "SV_InstanceID",  0u,  DXGI_FORMAT_R32_UINT,          1u,      16u,    D3D11_INPUT_PER_INSTANCE_DATA,    0u },
    };

    constexpr D3D11_INPUT_ELEMENT_DESC POS_UV[] = {
        { "POSITION",    0u,  DXGI_FORMAT_R32G32B32A32_FLOAT,  0u,   0u,      D3D11_INPUT_PER_VERTEX_DATA, 0u },
        { "TEXCOORD",       0u,  DXGI_FORMAT_R32G32_FLOAT,        0u,   16u,     D3D11_INPUT_PER_VERTEX_DATA, 0u },
    };

    constexpr D3D11_INPUT_ELEMENT_DESC POS_NORM_TAN_UV_INST[] = {
        { "POSITION",       0u,  DXGI_FORMAT_R32G32B32_FLOAT,   0u,      0u,     D3D11_INPUT_PER_VERTEX_DATA,      0u },
        { "NORMAL",         0u,  DXGI_FORMAT_R32G32B32_FLOAT,   0u,      16u,    D3D11_INPUT_PER_VERTEX_DATA,      0u },
        { "TANGENT",        0u,  DXGI_FORMAT_R32G32B32_FLOAT,   0u,      32u,    D3D11_INPUT_PER_VERTEX_DATA,      0u },
        { "TEXCOORD",       0u,  DXGI_FORMAT_R32G32_FLOAT,      0u,      48u,    D3D11_INPUT_PER_VERTEX_DATA,      0u },
        { "SV_InstanceID",  0u,  DXGI_FORMAT_R32_UINT,          1u,      56u,    D3D11_INPUT_PER_INSTANCE_DATA,    0u },
    };
};

struct SVertexShader
{
    ID3DBlob* Blob = nullptr;
    ID3D11InputLayout* Input = nullptr;
    ID3D11VertexShader* Shader = nullptr;

    void CreateVertexShader(ID3D11Device*, const char*, const D3D11_INPUT_ELEMENT_DESC*, UINT);
    void Release();
};

struct SPixelShader
{
    ID3DBlob* Blob = nullptr;
    ID3D11PixelShader* Shader = nullptr;

    void CreatePixelShader(ID3D11Device*, const char*);
    void Release();
};

class MShaderResources
{
public:
    static ID3DBlob* CreateBlobFromFileName(const char*, EShaderType);
    static ID3DBlob* GetBlobFromFileName(const char*);
    static ID3DBlob* GetOrCreateBlobFromFileName(const char* _filename, EShaderType _shaderType);
    static void InitializeShaders();
public:
    static void DeleteAllBlobs();
private:
    static void CompileShader(const char*, const char*, ID3DBlob**);
};