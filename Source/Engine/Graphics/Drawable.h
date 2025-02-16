#pragma once

#include "../IncludesExternal.h"
#include "../Math.h"

struct TTransform;
struct ID3D11Resource;
struct ID3D11ShaderResourceView;
struct ID3D11Buffer;
struct SMeshData;

class CDrawable_Base
{
    friend class MGraphic;
};

class CDrawable_InstancedMesh : public CDrawable_Base
{
    friend class MGraphic;
    friend class CGameScene;
public:
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
