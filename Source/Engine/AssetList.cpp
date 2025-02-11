#include "AssetList.h"

#include "Resources/MeshResources.h"

void MAsset::LoadMeshes()
{
    MMeshResources::CreateMeshDataFromFileName(JU_ASSET_SQUARE);
    MMeshResources::CreateMeshDataFromFileName(JU_ASSET_CUBE);
    MMeshResources::CreateMeshDataFromFileName(JU_ASSET_SPHERE);
    MMeshResources::CreateMeshDataFromFileName(JU_ASSET_SUZANNE);
    MMeshResources::CreateMeshDataFromFileName(JU_ASSET_CRATE);
    MMeshResources::CreateMeshDataFromFileName(JU_ASSET_MONSTER);
}
