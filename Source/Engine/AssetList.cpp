#include "AssetList.h"
#include "IncludesExternal.h"

#include "Resources/MeshResources.h"

void MAsset::LoadMeshes()
{
    MMeshResources::CreateMeshDataFromFileName(JU_ASSET_SQUARE);
    MMeshResources::CreateMeshDataFromFileName(JU_ASSET_CUBE);
    MMeshResources::CreateMeshDataFromFileName(JU_ASSET_SPHERE);
    MMeshResources::CreateMeshDataFromFileName(JU_ASSET_SUZANNE);
    MMeshResources::CreateMeshDataFromFileName(JU_ASSET_CRATE);
    MMeshResources::CreateMeshDataFromFileName(JU_ASSET_MONSTER);
    MMeshResources::CreateMeshDataFromFileName(JU_ASSET_TAKUMI);
    
    MMeshResources::CreateMeshDataFromFileName(JU_ASSET_TILE_0000);
    MMeshResources::CreateMeshDataFromFileName(JU_ASSET_TILE_1000);
    MMeshResources::CreateMeshDataFromFileName(JU_ASSET_TILE_1001);
    MMeshResources::CreateMeshDataFromFileName(JU_ASSET_TILE_1010);
    MMeshResources::CreateMeshDataFromFileName(JU_ASSET_TILE_1101);
    MMeshResources::CreateMeshDataFromFileName(JU_ASSET_TILE_1111);
}
