#pragma once

#include <d3dcommon.h>

#include "Math.h"
#include "Resources/MeshResources.h"
#include "Resources/ShadersResources.h"

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

struct SVertexShaderConstantBuffer
{
    TMatrix4f WorldViewProjection; 
};

__declspec(align(16)) struct SPixelShaderConstantBuffer
{
    TVector3f SunDirection;
    float SunAmbient;
};

class CStaticMesh
{
public:
    
    bool LoadMeshData(const TTransform& _transform, const char* _meshName);
    void UninitializeStaticMeshPipeline();

public:
    SGraphicResources_Mesh GraphicResource;
    SMeshData* MeshData;
    TTransform Transform;
};

class MGraphic
{
public:
    static void InitializeGraphic();
    static void StartDrawPipeline();
    static void DrawPipeline();
    static void UninitializeGraphic();
public:
    static CStaticMesh* AddMeshToDraw(const TTransform&, const char*);
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
    static void CreateVertexBuffer(ID3D11Device*, ID3D11DeviceContext*, SGraphicResources_Mesh&, const SMeshData&);
    static void CreateIndexBuffer(ID3D11Device*, ID3D11DeviceContext*, SGraphicResources_Mesh&, const SMeshData&);
    static void CreateVertexShaderBuffer(ID3D11Device*, ID3D11DeviceContext*, SGraphicResources_Mesh&);
private:
    static void SetVertexShader(ID3D11Device*, ID3D11DeviceContext*, SGraphicResources_Mesh&, const TTransform&);
    static void SetVertexAndIndexBuffer(ID3D11DeviceContext*, const SGraphicResources_Mesh&);
private:
    static void SetPixelShader(ID3D11DeviceContext*, const SGraphicResources_Mesh&);
    static void SetPixelShaderConstantBuffer(ID3D11Device*, ID3D11DeviceContext*, const SPixelShader&);
private:
    static void SetPrimitiveAndDraw(ID3D11DeviceContext*, const SMeshData&);
    static void Rasterize(ID3D11Device*, ID3D11DeviceContext*);
    static void ConfigureViewport(ID3D11DeviceContext*);
    static void PresentSwapChain(IDXGISwapChain*);
    static void ClearRenderTarget(ID3D11DeviceContext*, ID3D11RenderTargetView*);
    static void ClearDepthStencil(ID3D11DeviceContext*, ID3D11DepthStencilView*);
};