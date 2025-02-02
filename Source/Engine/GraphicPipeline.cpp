#include "GraphicPipeline.h"

#include <d3d11.h>
#include <sstream>

#include "GameWindow.h"
#include "HResultHandler.h"
#include "MeshManager.h"
#include "Shaders.h"
#include "WICTextureLoader.h"
#include "World.h"
#include "Debug/Profiling.h"

#define GAME_DATA_PATH "Game/Data/"
#define GAME_DATA_SHADER_PATH "Game/Data/Shaders/"

///---------------------------------------------------------------------------------------------------------------------
static SVertexShader G_VERTEX_SHADER_DATA;
static SPixelShader G_PIXEL_SHADER_DATA;
///---------------------------------------------------------------------------------------------------------------------
void CreateAndSetVertexShader(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, SVertexShader& _vertexShader)
{
    ShaderManager::CompileShader(GAME_DATA_SHADER_PATH L"VertexShader.hlsl", "vs_5_0", &_vertexShader.Blob);
    CHECK_HRESULT(_device->CreateVertexShader(_vertexShader.Blob->GetBufferPointer(), _vertexShader.Blob->GetBufferSize(), nullptr, &_vertexShader.Shader));
    // VertexShader Input Layout
    {
        constexpr D3D11_INPUT_ELEMENT_DESC inputElementDesc[] =
        {
            { "POSITION",  0u,  DXGI_FORMAT_R32G32B32_FLOAT,  0u,  D3D11_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA, 0u },
            { "UV",        0u,  DXGI_FORMAT_R32G32_FLOAT,     0u,  D3D11_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA, 0u },
            { "NORMAL",    0u,  DXGI_FORMAT_R32G32B32_FLOAT,  0u,  D3D11_APPEND_ALIGNED_ELEMENT,  D3D11_INPUT_PER_VERTEX_DATA, 0u },
            };
        UINT sizeInputElementDesc = 3u;
        CHECK_HRESULT(_device->CreateInputLayout(inputElementDesc, sizeInputElementDesc, _vertexShader.Blob->GetBufferPointer(), _vertexShader.Blob->GetBufferSize(), &_vertexShader.Input));
    }
    _deviceContext->VSSetShader(_vertexShader.Shader, nullptr, 0u);
    _deviceContext->IASetInputLayout(_vertexShader.Input);
}
///---------------------------------------------------------------------------------------------------------------------
void CreateAndSetPixelShader(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, SPixelShader& _pixelShader)
{
    ShaderManager::CompileShader(GAME_DATA_SHADER_PATH L"PixelShader.hlsl", "ps_5_0", &_pixelShader.Blob);
    CHECK_HRESULT(_device->CreatePixelShader(_pixelShader.Blob->GetBufferPointer(), _pixelShader.Blob->GetBufferSize(), nullptr, &_pixelShader.Shader));
    _deviceContext->PSSetShader(_pixelShader.Shader, nullptr, 0u);
    
    D3D11_SAMPLER_DESC samplerDesc = {};
    {
        samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
        samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_BORDER;
        samplerDesc.BorderColor[0] = SColor::Magenta.ToFloat().r;
        samplerDesc.BorderColor[1] = SColor::Magenta.ToFloat().g;
        samplerDesc.BorderColor[2] = SColor::Magenta.ToFloat().b;
        samplerDesc.BorderColor[3] = 1.0f;
    }
    
    CHECK_HRESULT(_device->CreateSamplerState(&samplerDesc, &_pixelShader.TextureSamplerState));
    _deviceContext->PSSetSamplers(0, 1, &_pixelShader.TextureSamplerState);
}
///---------------------------------------------------------------------------------------------------------------------
void CreatePixelShaderConstantBuffer(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, SPixelShader& _pixelShader)
{
    SPixelShaderConstantBuffer ConstantBufferPixelShader;
    {
        ConstantBufferPixelShader.SunDirection = TVector3f::Zero;
        ConstantBufferPixelShader.SunAmbient = 0.0f;
    }
    D3D11_SUBRESOURCE_DATA subResourceData = {};
    {
        subResourceData.pSysMem = &ConstantBufferPixelShader;
    }
    
    D3D11_BUFFER_DESC bufferDesc = {};
    {
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.ByteWidth = sizeof(SPixelShaderConstantBuffer);
        bufferDesc.StructureByteStride = 0u;
    }
    
    CHECK_HRESULT(_device->CreateBuffer(&bufferDesc, &subResourceData, &_pixelShader.ConstantBuffer));
}
///---------------------------------------------------------------------------------------------------------------------
void UpdatePixelShaderConstantBuffer(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, const SPixelShader& _pixelShader)
{
    SPixelShaderConstantBuffer ConstantBufferPixelShader;
    {
        ConstantBufferPixelShader.SunDirection = MWorld::GetWorld()->GetSunDirection();
        ConstantBufferPixelShader.SunAmbient = 0.2f;
    }
    
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    CHECK_HRESULT(_deviceContext->Map(_pixelShader.ConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
    memcpy(mappedResource.pData, &ConstantBufferPixelShader, sizeof(SPixelShaderConstantBuffer));
    _deviceContext->Unmap(_pixelShader.ConstantBuffer, 0);
    _deviceContext->PSSetConstantBuffers(0u, 1u, &_pixelShader.ConstantBuffer);
}
///---------------------------------------------------------------------------------------------------------------------
void CreateVertexBuffer(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, SMeshPipeline& _meshPipeline, const SMeshData& _meshData)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    {
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0u;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.ByteWidth = _meshData.VertexBuffer_ByteWidth;
        bufferDesc.StructureByteStride = SMeshData::VertexBuffer_StructureByteStride;
    }

    D3D11_SUBRESOURCE_DATA subResourceData = {};
    {
        subResourceData.pSysMem = _meshData.VertexBuffer.data();
    }

    CHECK_HRESULT(_device->CreateBuffer(&bufferDesc, &subResourceData, &_meshPipeline.vertexBuffer));
}
///---------------------------------------------------------------------------------------------------------------------
void CreateIndexBuffer(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, SMeshPipeline& _meshPipeline, const SMeshData& _meshData)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    {
        bufferDesc.Usage = D3D11_USAGE_DEFAULT;
        bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
        bufferDesc.CPUAccessFlags = 0u;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.ByteWidth = _meshData.IndexBuffer_ByteWidth;
        bufferDesc.StructureByteStride = SMeshData::IndexBuffer_StructureByteStride;
    }

    D3D11_SUBRESOURCE_DATA subResourceData = {};
    {
        subResourceData.pSysMem = _meshData.IndexBuffer.data();
    }

    CHECK_HRESULT(_device->CreateBuffer(&bufferDesc, &subResourceData, &_meshPipeline.IndexBuffer));
}
///---------------------------------------------------------------------------------------------------------------------
void CreateVertexShaderBuffer(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, SMeshPipeline& _pipeline)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    {
        bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
        bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
        bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
        bufferDesc.MiscFlags = 0u;
        bufferDesc.ByteWidth = sizeof(SVertexShaderConstantBuffer);
        bufferDesc.StructureByteStride = 0u;
    }
    
    D3D11_SUBRESOURCE_DATA subResourceData = {};
    {
        subResourceData.pSysMem = &_pipeline.VertexConstantBuffer;
    }
    
    CHECK_HRESULT(_device->CreateBuffer(&bufferDesc, &subResourceData, &_pipeline.VertexConstantBuffer));
}
///---------------------------------------------------------------------------------------------------------------------
void UpdateVertexShaderBuffer(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext, SMeshPipeline& _pipeline, const TTransform& _transform)
{
    SVertexShaderConstantBuffer constantBufferData;
    {
        const TMatrix4f objectWorldMatrix = TTransform::ToMatrix(_transform);
        const TMatrix4f invertedCameraMatrix = MWorld::GetWorld()->GetInverseCameraMatrix();
        const TMatrix4f perspectiveMatrix = GameWindow::GetPerspectiveMatrix();
        TMatrix4f worldViewProjectionMatrix = TMatrix4f::Transpose(objectWorldMatrix * invertedCameraMatrix * TMatrix4f::View * perspectiveMatrix);
        
        constantBufferData.WorldViewProjection = worldViewProjectionMatrix;
    }

    D3D11_MAPPED_SUBRESOURCE mappedResource;
    CHECK_HRESULT(_deviceContext->Map(_pipeline.VertexConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
    memcpy(mappedResource.pData, &constantBufferData, sizeof(SVertexShaderConstantBuffer));
    _deviceContext->Unmap(_pipeline.VertexConstantBuffer, 0);
    _deviceContext->VSSetConstantBuffers(0u, 1u, &_pipeline.VertexConstantBuffer);
}
///---------------------------------------------------------------------------------------------------------------------
void CStaticMesh::InitializeCommonPipeline()
{
    ID3D11Device* device = GameWindow::GetDevice();
    ID3D11DeviceContext* deviceContext = GameWindow::GetImmediateContext();
    
    CreateAndSetVertexShader(device, deviceContext, G_VERTEX_SHADER_DATA);
    CreateAndSetPixelShader(device, deviceContext, G_PIXEL_SHADER_DATA);
    CreatePixelShaderConstantBuffer(device, deviceContext, G_PIXEL_SHADER_DATA);
}
///---------------------------------------------------------------------------------------------------------------------
void CStaticMesh::UpdateCommonPipeline()
{
    ID3D11Device* device = GameWindow::GetDevice();
    ID3D11DeviceContext* deviceContext = GameWindow::GetImmediateContext();
    
    UpdatePixelShaderConstantBuffer(device, deviceContext, G_PIXEL_SHADER_DATA);
}
///---------------------------------------------------------------------------------------------------------------------
void CStaticMesh::UninitializeCommonPipeline()
{
    G_VERTEX_SHADER_DATA.Release();
    G_PIXEL_SHADER_DATA.Release();
}
///---------------------------------------------------------------------------------------------------------------------
bool CStaticMesh::InitializeStaticMeshPipeline(const TTransform& _transform, const char* _meshName)
{
    PROFILE_FUNCTION
    
    ID3D11Device* device = GameWindow::GetDevice();
    ID3D11DeviceContext* deviceContext = GameWindow::GetImmediateContext();
    
    Transform = _transform;

    // Load the mesh data from file
    {
        std::stringstream OBJFilenameStream;
        OBJFilenameStream << GAME_DATA_PATH << _meshName << ".obj";
        const bool Success = MeshManager::TryToImportOBJ(OBJFilenameStream.str().c_str(), &MeshData);
        if (Success == false)
            return false;
    }
    
    // Load the texture
    {
        std::wstringstream TextureFilenameStream;
        TextureFilenameStream << GAME_DATA_PATH << _meshName << ".bmp";
        CHECK_HRESULT(CreateWICTextureFromFile(device, deviceContext, TextureFilenameStream.str().c_str(), &Pipeline.Texture, &Pipeline.TextureView, 0));
    }
    
    CreateVertexBuffer(device, deviceContext, Pipeline, MeshData);
    CreateIndexBuffer(device, deviceContext, Pipeline, MeshData);
    CreateVertexShaderBuffer(device, deviceContext, Pipeline);

    return true;
}
///---------------------------------------------------------------------------------------------------------------------
void CStaticMesh::UninitializeStaticMeshPipeline()
{
}
///---------------------------------------------------------------------------------------------------------------------
void CStaticMesh::DrawStaticMesh()
{
    ID3D11Device* device = GameWindow::GetDevice();
    ID3D11DeviceContext* deviceContext = GameWindow::GetImmediateContext();
    
    constexpr UINT offset = 0u;
    deviceContext->IASetVertexBuffers(0u, 1u, &Pipeline.vertexBuffer, &SMeshData::VertexBuffer_StructureByteStride, &offset);
    deviceContext->IASetIndexBuffer(Pipeline.IndexBuffer, DXGI_FORMAT_R16_UINT, offset);
    
    UpdateVertexShaderBuffer(device, deviceContext, Pipeline, Transform);
    
    deviceContext->PSSetShaderResources(0u, 1u, &Pipeline.TextureView);

    deviceContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    deviceContext->DrawIndexed(MeshData.IndexCount,  0u, 0);
}
///---------------------------------------------------------------------------------------------------------------------