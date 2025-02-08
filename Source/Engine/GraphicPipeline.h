#pragma once
#include <d3dcommon.h>
#include <DirectXMath.h>

#include "Math.h"
#include "MeshManager.h"
#include "Shaders.h"

struct ID3D11Resource;
struct ID3D11ShaderResourceView;

struct SMeshPipeline
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
    
    static void InitializeCommonPipeline();
    static void UpdateCommonPipeline();
    static void UninitializeCommonPipeline();
    
    bool InitializeStaticMeshPipeline(const TTransform& _transform, const char* _meshName);
    void UninitializeStaticMeshPipeline();
    void DrawStaticMesh();

private:
    SMeshPipeline Pipeline;  // TEMPORARY CODE | Will stay as local in CreateStaticMesh
    SMeshData MeshData;  // TEMPORARY CODE | Will stay as local in CreateStaticMesh
    TTransform Transform; // TEMPORARY CODE | Will stay as local in CreateStaticMesh
};

class MGraphicPipeline
{
public:
    static void CreateAndSetVertexShader(ID3D11Device*, ID3D11DeviceContext*, SVertexShader&);
    static void CreateAndSetPixelShader(ID3D11Device*, ID3D11DeviceContext*, SPixelShader&);
    static void CreatePixelShaderConstantBuffer(ID3D11Device*, ID3D11DeviceContext*, SPixelShader&);
    static void UpdatePixelShaderConstantBuffer(ID3D11Device*, ID3D11DeviceContext*, const SPixelShader&);
    static void CreateVertexBuffer(ID3D11Device*, ID3D11DeviceContext*, SMeshPipeline&, const SMeshData&);
    static void CreateIndexBuffer(ID3D11Device*, ID3D11DeviceContext*, SMeshPipeline&, const SMeshData&);
    static void CreateVertexShaderBuffer(ID3D11Device*, ID3D11DeviceContext*, SMeshPipeline&);
    static void UpdateVertexShaderBuffer(ID3D11Device*, ID3D11DeviceContext*, SMeshPipeline&, const TTransform&);
};