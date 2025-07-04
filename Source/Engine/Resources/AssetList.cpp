#include "AssetList.h"
#include "../IncludesExternal.h"

#include "MeshResources.h"
#include "DrawableResources.h"
#include "ShadersResources.h"
#include "TextureResources.h"

void LoadMeshes()
{
}

void LoadShaders()
{
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_SHADER_VS_DEBUG_DRAW,      VERTEX_SHADER);
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_SHADER_VS_BASE,            VERTEX_SHADER);
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_SHADER_PS_BASE,            PIXEL_SHADER);
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_SHADER_VS_2D,              VERTEX_SHADER);
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_SHADER_PS_DEBUG_SCREEN,    PIXEL_SHADER);
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_SHADER_VS_SHADOW,          VERTEX_SHADER);
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_SHADER_PS_POST_PROCESS_1,  PIXEL_SHADER);
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_SHADER_PS_SIMPLE_COLOR,    PIXEL_SHADER);
}

void LoadTextures()
{
}

void LoadDrawables()
{
     MDrawableResources::CreateDrawableData(G_ASSET_SHAPE_TORUS, {"Shapes/Torus", "", "", G_ASSET_T_NONE_WHITE, G_ASSET_T_NONE_NORMAL, G_ASSET_T_NONE_WHITE, G_ASSET_T_NONE_WHITE, false});

    MDrawableResources::CreateDrawableData("Table", {"World/Table",
        "", "", G_ASSET_T_NONE_WHITE, G_ASSET_T_NONE_NORMAL,
        G_ASSET_T_NONE_DARK, G_ASSET_T_NONE_DARK, true});
    
    for (int i = 0; i < 130; ++i)
    {
        const std::string nameAsset = "WorldTile" + std::to_string(i);
        const std::string nameColorTexture = "World/Color_" + std::to_string(i) + ".png";
        MDrawableResources::CreateDrawableData(nameAsset.c_str(), {"World/Tile_Empty",
            "", "", nameColorTexture, G_ASSET_T_NONE_NORMAL,
            G_ASSET_T_NONE_DARK, G_ASSET_T_NONE_DARK, true});
    }
}

void MAsset::LoadAssets()
{
    LoadMeshes();
    LoadShaders();
    LoadTextures();
    LoadDrawables();
}

void MAsset::UnloadAssets()
{
    MDrawableResources::DestroyDrawables();
    MTextureResources::DestroyTexturesData();
    MShaderResources::DestroyBlobs();
    MMeshResources::DestroyMeshesData();
}

