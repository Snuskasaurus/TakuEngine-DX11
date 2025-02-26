#include "DrawableResources.h"

#include "AssetList.h"
#include "MeshResources.h"
#include "TextureResources.h"
#include "../HResultHandler.h"

static std::map<std::string, SDrawableData*> G_DRAWABLE_DATA_MAP;

SDrawableData* MDrawableResources::CreateDrawableData(const char* _name, const SDrawableData_CreateInput& _input)
{
    SDrawableData* newDrawableData = new SDrawableData;

    if (!_input.Filename_Mesh.empty()) newDrawableData->MeshData = MMeshResources::GetOrCreateMeshDataFromFileName(_input.Filename_Mesh.c_str());
    if (!_input.Filename_ColorTexture.empty()) newDrawableData->ColorTextureData = MTextureResources::GetOrCreateTextureDataFromFileName(_input.Filename_ColorTexture.c_str());
    if (!_input.Filename_NormalTexture.empty()) newDrawableData->NormalTextureData = MTextureResources::GetOrCreateTextureDataFromFileName(_input.Filename_NormalTexture.c_str());
    if (!_input.Filename_MROTexture.empty()) newDrawableData->MROTextureData = MTextureResources::GetOrCreateTextureDataFromFileName(_input.Filename_MROTexture.c_str());
    if (!_input.Filename_EmissionTexture.empty()) newDrawableData->EmissionTextureData = MTextureResources::GetOrCreateTextureDataFromFileName(_input.Filename_EmissionTexture.c_str());
    if (!_input.Filename_VertexShader.empty()) {} // TODO
    if (!_input.Filename_PixelShader.empty()) {} // TODO;
    
    G_DRAWABLE_DATA_MAP.insert({ _name, newDrawableData });
    
    return newDrawableData;
}

SDrawableData* MDrawableResources::GetDrawableData(const char* _name)
{
    const std::string fileNameAsString = _name;
    const auto result = G_DRAWABLE_DATA_MAP.find(fileNameAsString);
    if (result == G_DRAWABLE_DATA_MAP.end())
        return nullptr;
    return result->second;
}
