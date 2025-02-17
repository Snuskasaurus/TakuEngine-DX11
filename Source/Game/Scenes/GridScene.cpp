#include "GridScene.h"

#include "../../Engine/Math.h"
#include "../../Engine/AssetList.h"

constexpr int GridWidth = 100; // X
constexpr int GridHeight = 100; // Y
constexpr float TileOffset = 2.0f;

//---------------------------------------------------------------------------------------------------------------------
void CGridScene::OnCreate()
{
    InstancedMesh = CGridScene::AddInstancedMeshToDraw({}, JU_ASSET_TILE);
    for (int i = 1; i < GridWidth * GridHeight; ++i)
    {
        int XTile = (i - 1) % GridWidth;
        int YTile = (i - 1) / GridWidth;
        TVector3f Position = { (float)(XTile) * TileOffset, (float)(YTile) * TileOffset, 0.0f };
        TTransform Transform = { Position, 0.0f, 0.0f, 0.0f };
        InstancedMesh->Instances.push_back(Transform);
    }
}
//---------------------------------------------------------------------------------------------------------------------
void CGridScene::OnUpdate(const float& _dt)
{
}
//---------------------------------------------------------------------------------------------------------------------
void CGridScene::OnDestroy()
{
    
}
//---------------------------------------------------------------------------------------------------------------------
