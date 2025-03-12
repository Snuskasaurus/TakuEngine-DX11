#pragma once

#include "../IncludesExternal.h"

struct SDrawableData_CreateInput
{
    std::string Filename_Mesh;
    std::string Filename_VertexShader;
    std::string Filename_PixelShader;
    std::string Filename_ColorTexture;
    std::string Filename_NormalTexture;
    std::string Filename_EmissionTexture;
    std::string Filename_MROTexture;
    bool CastShadow = true;
};

struct SDrawableData
{
    struct SMeshData* MeshData = nullptr;
    struct STextureData* ColorTextureData = nullptr;
    struct STextureData* NormalTextureData = nullptr;
    struct STextureData* EmissionTextureData = nullptr;
    struct STextureData* MROTextureData = nullptr;
    bool CastShadow = true;
    
    void Release();
};

class MDrawableResources
{
public:
    static SDrawableData* CreateDrawableData(const char*, const SDrawableData_CreateInput&);
    static SDrawableData* GetDrawableData(const char*);
    static void DestroyDrawables();
};
