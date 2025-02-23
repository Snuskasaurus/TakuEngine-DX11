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
    
    VSConstantBuffer_InstancedObject->Release();
    VSConstantBuffer_InstancedObject = nullptr;
    
    Texture->Release();
    Texture = nullptr;
    
    TextureView->Release();
    TextureView = nullptr;
}
