#include "GridScene.h"

#include "../../Engine/World.h"
#include "../../Engine/Math.h"
#include "../../Engine/AssetList.h"

typedef int TTerrainIndex;
typedef int TGridIndex;
typedef int TVisualGridIndex;

struct TVisualMeshData
{
    CDrawable_InstancedMesh* InstancedMesh;
    float Rotation;
};
 
enum ETerrainType
{
    WATER,
    GROUND,
    HILL,
};

constexpr int GridWidth = 100; // X
constexpr int GridHeight = 100; // Y
constexpr float TileSize = 2.0f;
constexpr int nbTiles = GridWidth * GridHeight;
constexpr int nbVisualTiles = (GridWidth + 1) * (GridHeight + 1);

std::vector<ETerrainType> GridTerrains;
std::map<TTerrainIndex, TVisualMeshData> VisualMeshDataMap;

std::vector<TVisualGridIndex> GridIndex_To_VisualGridIndex(const TGridIndex& _gridIndex)
{
    const int row = _gridIndex / GridWidth;
    const int col = _gridIndex % GridWidth;
    return {
        row * (GridWidth + 1) + col,
        row * (GridWidth + 1) + (col + 1),
        (row + 1) * (GridWidth + 1) + (col + 1),
        (row + 1) * (GridWidth + 1) + col,
    };
}

std::vector<TGridIndex> VisualGridIndex_To_GridIndex(const TVisualGridIndex& _visualGridIndex)
{
    int row = _visualGridIndex / (GridWidth + 1);
    int col = _visualGridIndex % (GridWidth + 1);

    return {
        MMath::Clamp(row * GridWidth + col, 0, nbTiles - 1),
        MMath::Clamp(row * GridWidth + (col - 1), 0, nbTiles - 1),
        MMath::Clamp((row - 1) * GridWidth + col, 0, nbTiles - 1),
        MMath::Clamp((row - 1) * GridWidth + (col - 1), 0, nbTiles - 1),
    };
}

TVisualMeshData* GetVisualMeshDataFromVisualGridIndex(const TVisualGridIndex& _visualGridIndex)
{
    std::vector<TGridIndex> TileIndexes = VisualGridIndex_To_GridIndex(_visualGridIndex);
    char TileTerrainIndex = 0x0;

    TTerrainIndex terrainIndex = 0;
    for (int i = 0; i < 4; i++) {
        const TTerrainIndex TerrainType = (TTerrainIndex)GridTerrains[TileIndexes[i]];
        terrainIndex = terrainIndex * 10 + TerrainType;
    }
    return &VisualMeshDataMap.find(terrainIndex)->second;
}


void InitVisualMeshDataMap()
{
    CGameScene* GameScene = MWorld::GetWorld()->GetCurrentScene();
    
    CDrawable_InstancedMesh* Mesh0000 = GameScene->AddInstancedMeshToDraw(JU_ASSET_TILE_0000);
    CDrawable_InstancedMesh* Mesh1000 = GameScene->AddInstancedMeshToDraw(JU_ASSET_TILE_1000);
    CDrawable_InstancedMesh* Mesh1001 = GameScene->AddInstancedMeshToDraw(JU_ASSET_TILE_1001);
    CDrawable_InstancedMesh* Mesh1010 = GameScene->AddInstancedMeshToDraw(JU_ASSET_TILE_1010);
    CDrawable_InstancedMesh* Mesh1101 = GameScene->AddInstancedMeshToDraw(JU_ASSET_TILE_1101);
    CDrawable_InstancedMesh* Mesh1111 = GameScene->AddInstancedMeshToDraw(JU_ASSET_TILE_1111);
    
    VisualMeshDataMap.insert({0, {Mesh0000, 0.0f}});
    
    VisualMeshDataMap.insert({1000, {Mesh1000, 0.0f}});
    VisualMeshDataMap.insert({100, {Mesh1000, MMath::Deg2Rad(90.0f)}});
    VisualMeshDataMap.insert({10, {Mesh1000, MMath::Deg2Rad(180.0f)}});
    VisualMeshDataMap.insert({1, {Mesh1000, MMath::Deg2Rad(270.0f)}});
    
    VisualMeshDataMap.insert({1001, {Mesh1001, 0.0f}});
    VisualMeshDataMap.insert({1100, {Mesh1001, MMath::Deg2Rad(90.0f)}});
    VisualMeshDataMap.insert({110, {Mesh1001, MMath::Deg2Rad(180.0f)}});
    VisualMeshDataMap.insert({11, {Mesh1001, MMath::Deg2Rad(270.0f)}});
    
    VisualMeshDataMap.insert({1010, {Mesh1010, 0.0f}});
    VisualMeshDataMap.insert({101, {Mesh1010, MMath::Deg2Rad(90.0f)}});
    
    VisualMeshDataMap.insert({1101, {Mesh1101, 0.0f}});
    VisualMeshDataMap.insert({1110, {Mesh1101, MMath::Deg2Rad(90.0f)}});
    VisualMeshDataMap.insert({111, {Mesh1101, MMath::Deg2Rad(180.0f)}});
    VisualMeshDataMap.insert({1011, {Mesh1101, MMath::Deg2Rad(270.0f)}});
    
    VisualMeshDataMap.insert({1111, {Mesh1111, 0.0f}});
}
//---------------------------------------------------------------------------------------------------------------------
void CGridScene::OnCreate()
{
    // Generate grid terrain
    {
        GridTerrains.reserve(nbTiles);
        GridTerrains.insert(GridTerrains.end(), nbTiles, ETerrainType::WATER);
        for (int i = 1; i < GridWidth * GridHeight; ++i)
        {
            int XTile = (i - 1) % GridWidth;
            int YTile = (i - 1) / GridWidth;

            int FullWaterBorder = 3;
            if (XTile < FullWaterBorder
             || YTile < FullWaterBorder
             || XTile > GridWidth - FullWaterBorder
             || YTile > GridHeight - FullWaterBorder)
                continue;

            int LotOfWater = 6;
            if (XTile < LotOfWater
             || YTile < LotOfWater
             || XTile > GridWidth - LotOfWater
             || YTile > GridHeight - LotOfWater)
            {
                if (MMath::RandomNumberIntegerInRange(0, 2) == 0)
                    continue;
            }
            
            int LessLotOfWater = 10;
            if (XTile < LotOfWater
             || YTile < LotOfWater
             || XTile > GridWidth - LotOfWater
             || YTile > GridHeight - LotOfWater)
            {
                if (MMath::RandomNumberIntegerInRange(0, 4) == 0)
                    continue;
            }
            
            if (MMath::RandomNumberIntegerInRange(0, 15) == 0)
                continue;

            GridTerrains[i] = ETerrainType::GROUND;
        }
    }

    // Generate grid meshes
    {
        InitVisualMeshDataMap();
        
        float HalfTileSize = TileSize * 0.5f;
        float HalfWidthGrid = HalfTileSize * GridWidth - HalfTileSize;
        float HalfHeightGrid = HalfTileSize * GridHeight - HalfTileSize;
        
        for (int i = 1; i < nbVisualTiles; ++i)
        {
            const TVisualMeshData* visualMeshData = GetVisualMeshDataFromVisualGridIndex(i);
            
            const int XTile = (i - 1) % GridWidth;
            const int YTile = (i - 1) / GridWidth;
            TVector3f position = { (float)(XTile) * TileSize - HalfWidthGrid, (float)(YTile) * TileSize - HalfHeightGrid, 0.0f };
            TTransform Transform = { position, visualMeshData->Rotation, 0.0f, 0.0f };
            
            visualMeshData->InstancedMesh->Instances.push_back(Transform);
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
