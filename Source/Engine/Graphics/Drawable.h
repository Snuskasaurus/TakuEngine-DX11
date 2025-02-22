#pragma once

#include "../IncludesExternal.h"
#include "../Math.h"

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11Resource;
struct ID3D11ShaderResourceView;
struct ID3D11Buffer;
struct SMeshData;
struct TTransform;

class CDrawable_InstancedMesh
{
    friend class MGraphic;
    friend class CGameScene;
public:
    CDrawable_InstancedMesh();
    ~CDrawable_InstancedMesh();
public:
    std::vector<TTransform> Instances;
private:
    ID3D11Buffer* VertexBuffer = nullptr;
    ID3D11Buffer* IndexBuffer = nullptr;
    ID3D11Buffer* VSConstantBuffer = nullptr;
    ID3D11Resource* Texture = nullptr;
    ID3D11ShaderResourceView* TextureView = nullptr;
    SMeshData* MeshData;
};