#include "Drawable.h"

#include <d3d11.h>

#include "Graphic.h"

CDrawable_InstancedMesh::CDrawable_InstancedMesh()
{
    
}

CDrawable_InstancedMesh::~CDrawable_InstancedMesh()
{
    VertexBuffer->Release();
    VertexBuffer = nullptr;
    
    IndexBuffer->Release();
    IndexBuffer = nullptr;
    
    VSConstantBuffer->Release();
    VSConstantBuffer = nullptr;
    
    Texture->Release();
    Texture = nullptr;
    
    TextureView->Release();
    TextureView = nullptr;
}

void CDrawable_InstancedMesh::CreateVertexShader(ID3D11Device* _id3D11Device, ID3D11DeviceContext* _id3D11DeviceContext)
{
    
}

void CDrawable_InstancedMesh::CreatePixelShader(ID3D11Device* _id3D11Device, ID3D11DeviceContext* _id3D11DeviceContext)
{
    
}

void CDrawable_InstancedMesh::Draw(ID3D11Device* _device, ID3D11DeviceContext* _deviceContext)
{
    MGraphic::SetVertexAndIndexBuffer(_deviceContext, &VertexBuffer, IndexBuffer);
    MGraphic::SetPixelShader(_deviceContext, &TextureView);

    const UINT nbInstances = (UINT)Instances.size();
    UINT nbInstancesRemainingToDraw = nbInstances;
    while (nbInstancesRemainingToDraw > 0)
    {
        const UINT nbInstancesToDraw = MMath::Min(nbInstancesRemainingToDraw, MAX_INSTANCE_COUNT - 1);
        const UINT startInstances = nbInstances - nbInstancesRemainingToDraw;
            
        MGraphic::SetVSConstantBuffer_Instanced(_device, _deviceContext, &VSConstantBuffer, Instances, startInstances, nbInstancesToDraw);
        MGraphic::SetPrimitiveAndDraw_Instanced(_deviceContext, MeshData->IndexCount, nbInstancesToDraw + 1);
            
        nbInstancesRemainingToDraw -= nbInstancesToDraw;
    }
}
