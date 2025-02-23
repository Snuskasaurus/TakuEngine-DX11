#pragma once
#include "ShadersResources.h"

struct STextureData
{
    struct ID3D11Resource* texture = nullptr;
    struct ID3D11ShaderResourceView* textureView = nullptr;

    ~STextureData()
    {
        texture->Release();
        textureView->Release();
    }
};
    
class MTextureResources
{
public:
    static STextureData* CreateTextureDataFromFileName(const char*);
    static STextureData* GetTextureDataFromFileName(const char*); 
};
