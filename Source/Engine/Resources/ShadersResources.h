#pragma once

#include <Windows.h>
#include <d3d11.h>

struct SVertexShader
{
    ID3DBlob* Blob = nullptr;
    ID3D11InputLayout* Input = nullptr;
    ID3D11VertexShader* Shader = nullptr;

    void Release()
    {
        Blob->Release();
        Blob = nullptr;
        
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
        Blob->Release();
        Blob = nullptr;
        
        Shader->Release();
        Shader = nullptr;
        
        ConstantBuffer->Release();
        ConstantBuffer = nullptr;
        
        TextureSamplerState->Release();
        TextureSamplerState = nullptr;
    }
};

class ShaderManager
{
public:
    static void CompileShader(LPCWSTR _fileName, const char* _target, ID3DBlob** _shaderOut);
};
