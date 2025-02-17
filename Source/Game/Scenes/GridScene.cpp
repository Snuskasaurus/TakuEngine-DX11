#include "GridScene.h"

#include "../../Engine/Math.h"
#include "../../Engine/AssetList.h"

constexpr int GridWidth = 100; // X
constexpr int GridHeight = 100; // Y
constexpr float TileOffset = 2.0f;

enum ETerrainType
{
    NONE,
    WATER,
    GROUND,
    HILL,
};

std::vector<ETerrainType> GridTerrains;

CDrawable_InstancedMesh* TileMesh_Ground = nullptr;
CDrawable_InstancedMesh* TileMesh_Water = nullptr;

//---------------------------------------------------------------------------------------------------------------------
void CGridScene::OnCreate()
{
    GridTerrains.reserve(GridWidth * GridHeight);
    GridTerrains.insert(GridTerrains.end(), GridWidth * GridHeight, ETerrainType::WATER);
    for (int i = 1; i < GridWidth * GridHeight; ++i)
    {
        GridTerrains[i] = (ETerrainType)MMath::RandomNumberIntegerInRange(WATER, HILL);
    }

    // Generate grid mesh
    {
        TileMesh_Ground = CGridScene::AddInstancedMeshToDraw(JU_ASSET_TILE_GROUND);
        TileMesh_Water = CGridScene::AddInstancedMeshToDraw(JU_ASSET_TILE_WATER);

        float HalfWidthGrid = TileOffset * GridWidth / 2.0f;
        float HalfHeightGrid = TileOffset * GridHeight / 2.0f;
        for (int i = 1; i < GridWidth * GridHeight; ++i)
        {
            int XTile = (i - 1) % GridWidth;
            int YTile = (i - 1) / GridWidth;
            TVector3f Position = { (float)(XTile) * TileOffset - HalfWidthGrid, (float)(YTile) * TileOffset - HalfHeightGrid, 0.0f };
            TTransform Transform = { Position, 0.0f, 0.0f, 0.0f };

            if (GridTerrains[i] == ETerrainType::GROUND)
            {
                TileMesh_Ground->Instances.push_back(Transform);
            }
            else
            {
                TileMesh_Water->Instances.push_back(Transform);
            }
        }
    }
}
void CGridScene::OnKeyPressed(EKeyCode _key)
{
    if (_key == EKeyCode::KEY_ESCAPE)
    {
        PostQuitMessage(1);
    }
    else if (_key == EKeyCode::KEY_F1)
    {
        ChangeGameScene(EGameSceneType::TakumiScene);
    }
    else if (_key == EKeyCode::KEY_F2)
    {
        ChangeGameScene(EGameSceneType::GridScene);
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
