#include "AssetList.h"
#include "../IncludesExternal.h"

#include "MeshResources.h"
#include "DrawableResources.h"
#include "ShadersResources.h"
#include "TextureResources.h"

void LoadMeshes()
{
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_TILE_0000);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_TILE_1000);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_TILE_1001);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_TILE_1010);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_TILE_1101);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_TILE_1111);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_TILE_2000);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_TILE_2002);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_TILE_2020);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_TILE_2202);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_TILE_2222);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_TREE_01);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_TREE_02);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_TREE_03);
}

void LoadShaders()
{
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_VS_BASE,            VERTEX_SHADER);
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_PS_BASE,            PIXEL_SHADER);
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_VS_2D,              VERTEX_SHADER);
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_PS_2D,              PIXEL_SHADER);
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_VS_SHADOW,          VERTEX_SHADER);
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_PS_POST_PROCESS_1,  PIXEL_SHADER);
}

void LoadTextures()
{
    MTextureResources::CreateTextureDataFromFileName(TAKU_ASSET_MESH_TILE_0000);
    MTextureResources::CreateTextureDataFromFileName(TAKU_ASSET_MESH_TILE_1000);
    MTextureResources::CreateTextureDataFromFileName(TAKU_ASSET_MESH_TILE_1001);
    MTextureResources::CreateTextureDataFromFileName(TAKU_ASSET_MESH_TILE_1010);
    MTextureResources::CreateTextureDataFromFileName(TAKU_ASSET_MESH_TILE_1101);
    MTextureResources::CreateTextureDataFromFileName(TAKU_ASSET_MESH_TILE_1111);
    MTextureResources::CreateTextureDataFromFileName(TAKU_ASSET_MESH_TILE_2000);
    MTextureResources::CreateTextureDataFromFileName(TAKU_ASSET_MESH_TILE_2002);
    MTextureResources::CreateTextureDataFromFileName(TAKU_ASSET_MESH_TILE_2020);
    MTextureResources::CreateTextureDataFromFileName(TAKU_ASSET_MESH_TILE_2202);
    MTextureResources::CreateTextureDataFromFileName(TAKU_ASSET_MESH_TILE_2222);
    MTextureResources::CreateTextureDataFromFileName(TAKU_ASSET_MESH_TREE_01);
    MTextureResources::CreateTextureDataFromFileName(TAKU_ASSET_MESH_TREE_02);
    MTextureResources::CreateTextureDataFromFileName(TAKU_ASSET_MESH_TREE_03);
}

void LoadDrawables()
{
    MDrawableResources::CreateDrawableData(G_ASSET_TILE_BORDER, {G_ASSET_MESH_TILE_BORDER, "", "", G_ASSET_T_NONE_WHITE, G_ASSET_T_NONE_WHITE, G_ASSET_T_NONE_WHITE, G_ASSET_T_NONE_WHITE, false});
    MDrawableResources::CreateDrawableData(G_ASSET_ARROW, {G_ASSET_MESH_ARROW, "", "", G_ASSET_T_DEBUG, G_ASSET_T_NONE_WHITE, G_ASSET_T_DEBUG, G_ASSET_T_NONE_WHITE, false});
    MDrawableResources::CreateDrawableData(G_ASSET_GIZMO, {G_ASSET_MESH_GIZMO, "", "", G_ASSET_T_DEBUG, G_ASSET_T_NONE_WHITE, G_ASSET_T_DEBUG, G_ASSET_T_NONE_WHITE, false});
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

