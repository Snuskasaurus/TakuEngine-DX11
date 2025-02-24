#pragma once

#include "../IncludesExternal.h"

struct SDrawableData_CreateInput
{
    std::string Filename_Mesh;
    std::string Filename_VertexShader;
    std::string Filename_PixelShader;
    std::string Filename_ColorTexture;
    std::string Filename_NormalTexture;
    std::string Filename_PBRTexture;
};

struct SDrawableData
{
    struct SMeshData* MeshData = nullptr;
    struct STextureData* ColorTextureData = nullptr;
    struct STextureData* PBRTextureData = nullptr;
    struct STextureData* NormalTextureData = nullptr;
    struct SVertexShaderData* VertexShaderData = nullptr;
    struct SPixelShaderData* PixelShaderData = nullptr;
};

class MDrawableResources
{
public:
    static SDrawableData* CreateDrawableData(const char*, const SDrawableData_CreateInput&);
    static SDrawableData* GetDrawableData(const char*);
};
