#include "AssetList.h"
#include "../IncludesExternal.h"

#include "MeshResources.h"
#include "DrawableResources.h"
#include "ShadersResources.h"
#include "TextureResources.h"

void LoadMeshes()
{
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_TREE_01);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_TREE_02);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_TREE_03);
}

void LoadShaders()
{
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_SHADER_VS_DEBUG_DRAW,      VERTEX_SHADER);
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_SHADER_VS_BASE,            VERTEX_SHADER);
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_SHADER_PS_BASE,            PIXEL_SHADER);
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_SHADER_VS_2D,              VERTEX_SHADER);
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_SHADER_PS_DEBUG_SCREEN,              PIXEL_SHADER);
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_SHADER_VS_SHADOW,          VERTEX_SHADER);
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_SHADER_PS_POST_PROCESS_1,  PIXEL_SHADER);
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_SHADER_PS_SIMPLE_COLOR,  PIXEL_SHADER);
}

void LoadTextures()
{
    MTextureResources::CreateTextureDataFromFileName(TAKU_ASSET_MESH_TREE_01);
    MTextureResources::CreateTextureDataFromFileName(TAKU_ASSET_MESH_TREE_02);
    MTextureResources::CreateTextureDataFromFileName(TAKU_ASSET_MESH_TREE_03);
}

void LoadDrawables()
{
    MDrawableResources::CreateDrawableData(G_ASSET_TILE_0000, {G_ASSET_MESH_TILE_0000, "", "", G_ASSET_T_TILES_C, G_ASSET_T_NONE_NORMAL, G_ASSET_T_NONE_DARK, G_ASSET_T_TILES_SO, false});
    MDrawableResources::CreateDrawableData(G_ASSET_TILE_1000, {G_ASSET_MESH_TILE_1000, "", "", G_ASSET_T_TILES_C, G_ASSET_T_NONE_NORMAL, G_ASSET_T_NONE_DARK, G_ASSET_T_TILES_SO, false});
    MDrawableResources::CreateDrawableData(G_ASSET_TILE_1001, {G_ASSET_MESH_TILE_1001, "", "", G_ASSET_T_TILES_C, G_ASSET_T_NONE_NORMAL, G_ASSET_T_NONE_DARK, G_ASSET_T_TILES_SO, false});
    MDrawableResources::CreateDrawableData(G_ASSET_TILE_1010, {G_ASSET_MESH_TILE_1010, "", "", G_ASSET_T_TILES_C, G_ASSET_T_NONE_NORMAL, G_ASSET_T_NONE_DARK, G_ASSET_T_TILES_SO, false});
    MDrawableResources::CreateDrawableData(G_ASSET_TILE_1101, {G_ASSET_MESH_TILE_1101, "", "", G_ASSET_T_TILES_C, G_ASSET_T_NONE_NORMAL, G_ASSET_T_NONE_DARK, G_ASSET_T_TILES_SO, false});
    MDrawableResources::CreateDrawableData(G_ASSET_TILE_1111, {G_ASSET_MESH_TILE_1111, "", "", G_ASSET_T_TILES_C, G_ASSET_T_NONE_NORMAL, G_ASSET_T_NONE_DARK, G_ASSET_T_TILES_SO, false});
    MDrawableResources::CreateDrawableData(G_ASSET_TILE_2000, {G_ASSET_MESH_TILE_2000, "", "", G_ASSET_T_TILES_C, G_ASSET_T_NONE_NORMAL, G_ASSET_T_NONE_DARK, G_ASSET_T_TILES_SO, false});
    MDrawableResources::CreateDrawableData(G_ASSET_TILE_2002, {G_ASSET_MESH_TILE_2002, "", "", G_ASSET_T_TILES_C, G_ASSET_T_NONE_NORMAL, G_ASSET_T_NONE_DARK, G_ASSET_T_TILES_SO, false});
    MDrawableResources::CreateDrawableData(G_ASSET_TILE_2020, {G_ASSET_MESH_TILE_2020, "", "", G_ASSET_T_TILES_C, G_ASSET_T_NONE_NORMAL, G_ASSET_T_NONE_DARK, G_ASSET_T_TILES_SO, false});
    MDrawableResources::CreateDrawableData(G_ASSET_TILE_2202, {G_ASSET_MESH_TILE_2202, "", "", G_ASSET_T_TILES_C, G_ASSET_T_NONE_NORMAL, G_ASSET_T_NONE_DARK, G_ASSET_T_TILES_SO, false});
    MDrawableResources::CreateDrawableData(G_ASSET_TILE_2222, {G_ASSET_MESH_TILE_2222, "", "", G_ASSET_T_TILES_C, G_ASSET_T_NONE_NORMAL, G_ASSET_T_NONE_DARK, G_ASSET_T_TILES_SO, false});

    MDrawableResources::CreateDrawableData(G_ASSET_TILE_BORDER, {G_ASSET_MESH_TILE_BORDER, "", "", G_ASSET_T_NONE_WHITE, G_ASSET_T_NONE_WHITE, G_ASSET_T_NONE_WHITE, G_ASSET_T_NONE_WHITE, false});
    MDrawableResources::CreateDrawableData(G_ASSET_ARROW, {G_ASSET_MESH_ARROW, "", "", G_ASSET_T_DEBUG, G_ASSET_T_NONE_WHITE, G_ASSET_T_DEBUG, G_ASSET_T_NONE_WHITE, false});
    MDrawableResources::CreateDrawableData(G_ASSET_GIZMO, {G_ASSET_MESH_GIZMO, "", "", G_ASSET_T_DEBUG, G_ASSET_T_NONE_WHITE, G_ASSET_T_DEBUG, G_ASSET_T_NONE_WHITE, false});

    MDrawableResources::CreateDrawableData(G_ASSET_2D_CIRCLE, {"Debugs/2D_Circle", "", "", G_ASSET_T_DEBUG, G_ASSET_T_NONE_WHITE, G_ASSET_T_DEBUG, G_ASSET_T_NONE_WHITE, false});
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

