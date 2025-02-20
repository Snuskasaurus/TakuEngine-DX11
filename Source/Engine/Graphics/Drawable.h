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

class CDrawable_Base
{
    friend class MGraphic;

public:
    virtual ~CDrawable_Base() = default;
    void virtual CreateVertexShader(ID3D11Device*, ID3D11DeviceContext*) {};
    void virtual CreatePixelShader(ID3D11Device*, ID3D11DeviceContext*) {};
    void virtual Draw(ID3D11Device*, ID3D11DeviceContext*) {};
};

class CDrawable_InstancedMesh : public CDrawable_Base
{
    friend class MGraphic;
    friend class CGameScene;
public:
    CDrawable_InstancedMesh();
    ~CDrawable_InstancedMesh() override;
public:
    void virtual CreateVertexShader(ID3D11Device*, ID3D11DeviceContext*) override;
    void virtual CreatePixelShader(ID3D11Device*, ID3D11DeviceContext*) override;
    void virtual Draw(ID3D11Device*, ID3D11DeviceContext*) override;
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