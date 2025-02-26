#include "AssetList.h"
#include "../IncludesExternal.h"

#include "MeshResources.h"
#include "DrawableResources.h"
#include "ShadersResources.h"
#include "TextureResources.h"

void MAsset::LoadMeshes()
{
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_SQUARE);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_CUBE);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_SPHERE);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_SUZANNE);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_CRATE);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_MONSTER);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_TAKUMI);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_TILE_BORDER);
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

void MAsset::LoadShaders()
{
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_VS_BASE, VERTEX_SHADER);
    MShaderResources::CreateBlobFromFileName(TAKU_ASSET_PS_BASE, PIXEL_SHADER);
}

void MAsset::LoadTextures()
{
    MTextureResources::CreateTextureDataFromFileName(TAKU_ASSET_MESH_TAKUMI);
    MTextureResources::CreateTextureDataFromFileName(TAKU_ASSET_MESH_TILE_BORDER);
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

void MAsset::LoadDrawables()
{
    MDrawableResources::CreateDrawableData(G_ASSET_ARROW, {G_ASSET_MESH_ARROW, "", "", G_ASSET_T_DEBUG, G_ASSET_T_NONE, G_ASSET_T_DEBUG, G_ASSET_T_NONE});
    MDrawableResources::CreateDrawableData(G_ASSET_GIZMO, {G_ASSET_MESH_GIZMO, "", "", G_ASSET_T_DEBUG, G_ASSET_T_NONE, G_ASSET_T_DEBUG, G_ASSET_T_NONE});

    // Machine
    {
        MDrawableResources::CreateDrawableData(G_DRAWABLE_MACHINE,
       {G_ASSET_MESH_MACHINE, "", "",
           G_ASSET_T_MACHINE_1_COLOR, G_ASSET_T_MACHINE_1_NORMAL, "", G_ASSET_T_MACHINE_1_MRO });

        MDrawableResources::CreateDrawableData(G_DRAWABLE_MACHINE_PANELS,
            {G_ASSET_MESH_MACHINE_PANELS, "", "",
                G_ASSET_T_MACHINE_2_COLOR, G_ASSET_T_MACHINE_2_NORMAL, G_ASSET_T_MACHINE_2_EMISSION, G_ASSET_T_MACHINE_2_MRO });

        MDrawableResources::CreateDrawableData(G_DRAWABLE_MACHINE_BUTTONS,
            {G_ASSET_MESH_MACHINE_BUTTONS, "", "",
                G_ASSET_T_MACHINE_2_COLOR, G_ASSET_T_MACHINE_2_NORMAL, G_ASSET_T_MACHINE_2_EMISSION, G_ASSET_T_MACHINE_2_MRO });
    }
}

