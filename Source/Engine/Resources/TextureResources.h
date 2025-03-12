#pragma once
#include "ShadersResources.h"

struct STextureData
{
    struct ID3D11Resource* texture = nullptr;
    struct ID3D11ShaderResourceView* textureView = nullptr;

    void Release();
};
    
class MTextureResources
{
public:
    static STextureData* CreateTextureDataFromFileName(const char*);
    static STextureData* GetTextureDataFromFileName(const char*); 
    static STextureData* GetOrCreateTextureDataFromFileName(const char*);
    static void DestroyTexturesData();
};
