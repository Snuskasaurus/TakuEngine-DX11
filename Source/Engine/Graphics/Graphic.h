#pragma once

#include "../Resources/MeshResources.h"
#include "../Resources/ShadersResources.h"
#include "Engine/Resources/ShadersResources.h"

class CDrawable_InstancedMesh;
struct ID3D11Resource;
struct ID3D11ShaderResourceView;

struct SDepthStencilResources
{
    ID3D11DepthStencilState* State = nullptr;
    ID3D11Texture2D* Texture = nullptr;
    ID3D11DepthStencilView* View = nullptr;
    ID3D11ShaderResourceView* ResourceView = nullptr;
    
    ID3D11DepthStencilView* ViewLight = nullptr;
    ID3D11Texture2D* TextureLight = nullptr;
    ID3D11ShaderResourceView* ResourceViewLight = nullptr;

    void Release()
    {
        State->Release();
        Texture->Release();
        View->Release();
        ResourceView->Release();
        ViewLight->Release();
        TextureLight->Release();
        ResourceViewLight->Release();
    }
};

class MGraphic
{
public:
    static ID3D11Device* GetDXDevice();
    static ID3D11DeviceContext* GetDXDeviceContext();
public:
    static void InitializeGraphic();
    static void InitializeShaders();
    static void PrepareUninitializeGraphic();
    static void UninitializeGraphic();
    static void ResizeScreen();
public:
    static void ReportLiveObjects(bool _showDetails);
public:
    static void RenderFrame();
    static void RenderFrame_SceneShadowMap();
    static void RenderFrame_Scene();
    static void RenderFrame_DebugScreen();
    static void RenderFrame_PostProcess();
    static void RenderFrame_DebugLines();

public:
    static void CreateRasterizerState();
    static void CreateDeviceAndSwapChain();
    static void ReleaseRenderTargetView();
    static void CreateDepthStencil();
    static void CreateRenderTargetView();
    static void CreateAndSetSampleStates();
    static void ReleaseDepthStencil();
public:
    static void CreateVertexBuffer(ID3D11Device*, ID3D11DeviceContext*, ID3D11Buffer** _vertexBuffer, void* _vertices, UINT _nbVertices, UINT _sizeStruct);
    static void CreateIndexBuffer(ID3D11Device*, ID3D11DeviceContext*, ID3D11Buffer** _indexBuffer, TVertexIndex* _indexes, UINT _nbIndex, UINT _sizeStruct);
    static void SetVertexAndIndexBuffer(ID3D11Buffer** _vertexBuffer, ID3D11Buffer* _indexBuffer, UINT _vertexBufferByteStride);
public:
    static void SetPixelShaderTextureViews(UINT nbTextures, ID3D11ShaderResourceView** _textureViews);
public:
    static void SetPrimitiveAndDraw_Instanced(UINT _indexCountPerInstance, UINT _instanceCount);
    static void SetRasterizerState(ID3D11RasterizerState* _rasterizerState);
    static void ConfigureViewport();
    static void PresentSwapChain(IDXGISwapChain*);
    static void ClearRenderTarget(ID3D11DeviceContext*, ID3D11RenderTargetView*);
    static void ClearDepthStencil(ID3D11DeviceContext*, ID3D11DepthStencilView*);
};