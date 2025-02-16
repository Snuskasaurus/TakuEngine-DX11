#pragma once

#include "Math.h"
#include "Resources/MeshResources.h"
#include "Resources/ShadersResources.h"

#define MAX_INSTANCE_COUNT 1024u

class CDrawable_InstancedMesh;
struct ID3D11Resource;
struct ID3D11ShaderResourceView;

struct SGraphicResources_Pipeline
{
    ID3D11Device* Device = nullptr;
    ID3D11DeviceContext* DeviceContext = nullptr;
    IDXGISwapChain* SwapChain = nullptr;
    ID3D11RenderTargetView* RenderTargetView = nullptr;
    ID3D11Resource* BackBufferResource = nullptr;
    
    ID3D11DepthStencilState* DepthStencilState = nullptr;
    ID3D11Texture2D* DepthStencilTexture = nullptr;
    ID3D11DepthStencilView* DepthStencilView = nullptr;
    
    SVertexShader VertexShaderData;
    SPixelShader PixelShaderData;
};

struct SGraphicResources_Mesh
{
    ID3D11Resource* Texture = nullptr;
    ID3D11ShaderResourceView* TextureView = nullptr;
    ID3D11Buffer* vertexBuffer = nullptr;
    ID3D11Buffer* IndexBuffer = nullptr;
    ID3D11Buffer* VertexConstantBuffer = nullptr;

    void Release()
    {
        Texture->Release();
        Texture = nullptr;
        
        TextureView->Release();
        TextureView = nullptr;
        
        vertexBuffer->Release();
        vertexBuffer = nullptr;

        IndexBuffer->Release();
        IndexBuffer = nullptr;

        VertexConstantBuffer->Release();
        VertexConstantBuffer = nullptr;
    }
};

__declspec(align(16)) struct SVertexShaderConstantBuffer
{
    TMatrix4f WorldViewProjection[MAX_INSTANCE_COUNT]; 
};

__declspec(align(16)) struct SPixelShaderConstantBuffer
{
    TVector3f SunDirection;
    float SunAmbient;
};

class CMesh
{
    friend class MGraphic;
    
public:
    TTransform Transform;

private:
    SGraphicResources_Mesh GraphicResource;
    SMeshData* MeshData;
};

class CInstancedMesh
{
    friend class MGraphic;
    
public:
    std::vector<TTransform> Transforms;

private:
    SGraphicResources_Mesh GraphicResource;
    SMeshData* MeshData;
};

class MGraphic
{
public:
    static void InitializeGraphic();
    static void StartDrawPipeline();
    static void DrawPipeline();
    static void UninitializeGraphic();
public:
    static void FillGraphicResources_Instanced(CDrawable_InstancedMesh* _drawableInstancedMesh, const wchar_t* _textureFilename);
private:
    static void CreateDeviceAndSwapChain(ID3D11Device**, ID3D11DeviceContext**, IDXGISwapChain**);
    static void CreateAndSetDepthStencilState(ID3D11Device*, ID3D11DeviceContext*, ID3D11DepthStencilState**);
    static void CreateDepthStencilTexture(ID3D11Device*, ID3D11Texture2D**);
    static void CreateDepthStencilView(ID3D11Device*, ID3D11Texture2D*, ID3D11DepthStencilView**);
    static void CreateRenderTargetView(ID3D11Device*, IDXGISwapChain*, ID3D11Resource**, ID3D11RenderTargetView**);
    static void SetDepthStencilViewToRenderTargetView(ID3D11DeviceContext*, ID3D11RenderTargetView**, ID3D11DepthStencilView*);
    static void CreateAndSetVertexShader(ID3D11Device*, ID3D11DeviceContext*, SVertexShader&);
    static void CreateAndSetPixelShader(ID3D11Device*, ID3D11DeviceContext*, SPixelShader&);
    static void CreatePixelShaderConstantBuffer(ID3D11Device*, ID3D11DeviceContext*, SPixelShader&);
    static void CreateVertexBuffer(ID3D11Device*, ID3D11DeviceContext*, ID3D11Buffer** _vertexBuffer, const SMeshData&);
    static void CreateIndexBuffer(ID3D11Device*, ID3D11DeviceContext*, ID3D11Buffer** _indexBuffer, const SMeshData&);
    static void CreateVertexShaderBuffer(ID3D11Device*, ID3D11DeviceContext*, ID3D11Buffer** VertexConstantBuffer);
private:
    static void SetVSConstantBuffer_Instanced(ID3D11Device*, ID3D11DeviceContext*, ID3D11Buffer** _objectBuffer, const std::vector<TTransform>& _transforms, UINT _start, UINT _nbInstances);
    static void SetVertexAndIndexBuffer(ID3D11DeviceContext*, ID3D11Buffer** _vertexBuffer, ID3D11Buffer* _indexBuffer);
private:
    static void SetPixelShader(ID3D11DeviceContext*, ID3D11ShaderResourceView** _textureView);
    static void SetPixelShaderConstantBuffer(ID3D11Device*, ID3D11DeviceContext*, const SPixelShader&);
private:
    static void SetPrimitiveAndDraw_Instanced(ID3D11DeviceContext*, UINT _indexCountPerInstance, UINT _instanceCount);
    static void Rasterize(ID3D11Device*, ID3D11DeviceContext*);
    static void ConfigureViewport(ID3D11DeviceContext*);
    static void PresentSwapChain(IDXGISwapChain*);
    static void ClearRenderTarget(ID3D11DeviceContext*, ID3D11RenderTargetView*);
    static void ClearDepthStencil(ID3D11DeviceContext*, ID3D11DepthStencilView*);
};