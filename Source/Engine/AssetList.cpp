#include "AssetList.h"
#include "IncludesExternal.h"

#include "Resources/MeshResources.h"
#include "Resources/ShadersResources.h"

void MAsset::LoadMeshes()
{
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_DEBUG_ARROW);
    MMeshResources::CreateMeshDataFromFileName(TAKU_ASSET_MESH_DEBUG_GIZMO);
    
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
