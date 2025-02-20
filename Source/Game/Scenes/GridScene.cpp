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

constexpr int GridWidth = 320; // X
constexpr int GridHeight = 150; // Y
constexpr float TileSize = 2.0f;
constexpr int nbTiles = GridWidth * GridHeight;
constexpr int nbVisualTiles = (GridWidth + 1) * (GridHeight + 1);

bool IsValidGridPosition(int row, int col)
{
    return row >= 0 && row < GridHeight && col >= 0 && col < GridWidth;
}
bool IsValidVisualGridPosition(int row, int col)
{
    return row >= 0 && row < GridHeight + 1 && col >= 0 && col < GridWidth + 1;
}

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
    const int row = _visualGridIndex / (GridWidth + 1);
    const int col = _visualGridIndex % (GridWidth + 1);
    
    const TGridIndex TopLeft = IsValidGridPosition(row - 1, col - 1) ? (row - 1) * GridWidth + (col - 1) : -1;
    const TGridIndex TopRight = IsValidGridPosition(row - 1, col) ? (row - 1) * GridWidth + col : -1;
    const TGridIndex BotRight = IsValidGridPosition(row, col) ? row * GridWidth + col : -1;
    const TGridIndex BotLeft = IsValidGridPosition(row, col - 1) ? row * GridWidth + (col - 1) : -1;
    
    return {
        TopLeft,
        TopRight,
        BotRight,
        BotLeft
    };
}

TVisualMeshData* GetVisualMeshDataFromVisualGridIndex(const TVisualGridIndex& _visualGridIndex)
{
    std::vector<TGridIndex> TileIndexes = VisualGridIndex_To_GridIndex(_visualGridIndex);
    TTerrainIndex terrainIndex = 0;
    for (int i = 0; i < 4; i++) {
        const TTerrainIndex TerrainType = TileIndexes[i] >= 0 ? (TTerrainIndex)GridTerrains[TileIndexes[i]] : 0;
        terrainIndex = terrainIndex * 10 + TerrainType;
    }
    const auto visualMeshDataMapIterator = VisualMeshDataMap.find(terrainIndex);
    if (visualMeshDataMapIterator == VisualMeshDataMap.end())
        return nullptr;

    return &visualMeshDataMapIterator->second;
}

void InitVisualMeshDataMap()
{
    CGameScene* GameScene = MWorld::GetWorld()->GetCurrentScene();
    
    CDrawable_InstancedMesh* Mesh0000 = GameScene->AddInstancedMeshToDraw(TAKU_ASSET_MESH_TILE_0000);
    CDrawable_InstancedMesh* Mesh1000 = GameScene->AddInstancedMeshToDraw(TAKU_ASSET_MESH_TILE_1000);
    CDrawable_InstancedMesh* Mesh1001 = GameScene->AddInstancedMeshToDraw(TAKU_ASSET_MESH_TILE_1001);
    CDrawable_InstancedMesh* Mesh1010 = GameScene->AddInstancedMeshToDraw(TAKU_ASSET_MESH_TILE_1010);
    CDrawable_InstancedMesh* Mesh1101 = GameScene->AddInstancedMeshToDraw(TAKU_ASSET_MESH_TILE_1101);
    CDrawable_InstancedMesh* Mesh1111 = GameScene->AddInstancedMeshToDraw(TAKU_ASSET_MESH_TILE_1111);
    
    VisualMeshDataMap.insert({0, {Mesh0000, 0.0f}});
    
    VisualMeshDataMap.insert({1000, {Mesh1000, 0.0f}});
    VisualMeshDataMap.insert({100, {Mesh1000, MMath::Deg2Rad(-90.0f)}});
    VisualMeshDataMap.insert({10, {Mesh1000, MMath::Deg2Rad(-180.0f)}});
    VisualMeshDataMap.insert({1, {Mesh1000, MMath::Deg2Rad(-270.0f)}});
    
    VisualMeshDataMap.insert({1001, {Mesh1001, 0.0f}});
    VisualMeshDataMap.insert({1100, {Mesh1001, MMath::Deg2Rad(-90.0f)}});
    VisualMeshDataMap.insert({110, {Mesh1001, MMath::Deg2Rad(-180.0f)}});
    VisualMeshDataMap.insert({11, {Mesh1001, MMath::Deg2Rad(-270.0f)}});
    
    VisualMeshDataMap.insert({1010, {Mesh1010, 0.0f}});
    VisualMeshDataMap.insert({101, {Mesh1010, MMath::Deg2Rad(-90.0f)}});
    
    VisualMeshDataMap.insert({1101, {Mesh1101, 0.0f}});
    VisualMeshDataMap.insert({1110, {Mesh1101, MMath::Deg2Rad(-90.0f)}});
    VisualMeshDataMap.insert({111, {Mesh1101, MMath::Deg2Rad(-180.0f)}});
    VisualMeshDataMap.insert({1011, {Mesh1101, MMath::Deg2Rad(-270.0f)}});
    
    VisualMeshDataMap.insert({1111, {Mesh1111, 0.0f}});
    
    Mesh0000->Instances.push_back({{(TileSize + 1.0f) * 0, 0.0f, -20.0f }, 0.0f, 0.0f, 0.0f});

    Mesh1000->Instances.push_back({{(TileSize + 1.0f) * 1, -(TileSize + 1.0f) * 0, -20.0f }, 0.0f, 0.0f, 0.0f});
    Mesh1000->Instances.push_back({{(TileSize + 1.0f) * 1, -(TileSize + 1.0f) * 1, -20.0f }, MMath::Deg2Rad(-90.0f), 0.0f, 0.0f});
    Mesh1000->Instances.push_back({{(TileSize + 1.0f) * 1, -(TileSize + 1.0f) * 2, -20.0f }, MMath::Deg2Rad(-180.0f), 0.0f, 0.0f});
    Mesh1000->Instances.push_back({{(TileSize + 1.0f) * 1, -(TileSize + 1.0f) * 3, -20.0f }, MMath::Deg2Rad(-270.0f), 0.0f, 0.0f});

    Mesh1001->Instances.push_back({{(TileSize + 1.0f) * 2, -(TileSize + 1.0f) * 0, -20.0f }, 0.0f, 0.0f, 0.0f});
    Mesh1001->Instances.push_back({{(TileSize + 1.0f) * 2, -(TileSize + 1.0f) * 1, -20.0f }, MMath::Deg2Rad(-90.0f), 0.0f, 0.0f});
    Mesh1001->Instances.push_back({{(TileSize + 1.0f) * 2, -(TileSize + 1.0f) * 2, -20.0f }, MMath::Deg2Rad(-180.0f), 0.0f, 0.0f});
    Mesh1001->Instances.push_back({{(TileSize + 1.0f) * 2, -(TileSize + 1.0f) * 3, -20.0f }, MMath::Deg2Rad(-270.0f), 0.0f, 0.0f});
    
    Mesh1010->Instances.push_back({{(TileSize + 1.0f) * 3, -(TileSize + 1.0f) * 0, -20.0f }, 0.0f, 0.0f, 0.0f});
    Mesh1010->Instances.push_back({{(TileSize + 1.0f) * 3, -(TileSize + 1.0f) * 1, -20.0f }, MMath::Deg2Rad(-90.0f), 0.0f, 0.0f});
    
    Mesh1101->Instances.push_back({{(TileSize + 1.0f) * 4, -(TileSize + 1.0f) * 0, -20.0f }, 0.0f, 0.0f, 0.0f});
    Mesh1101->Instances.push_back({{(TileSize + 1.0f) * 4, -(TileSize + 1.0f) * 1, -20.0f }, MMath::Deg2Rad(-90.0f), 0.0f, 0.0f});
    Mesh1101->Instances.push_back({{(TileSize + 1.0f) * 4, -(TileSize + 1.0f) * 2, -20.0f }, MMath::Deg2Rad(-180.0f), 0.0f, 0.0f});
    Mesh1101->Instances.push_back({{(TileSize + 1.0f) * 4, -(TileSize + 1.0f) * 3, -20.0f }, MMath::Deg2Rad(-270.0f), 0.0f, 0.0f});
    
    Mesh1111->Instances.push_back({{(TileSize + 1.0f) * 5, 0.0f, -20.0f }, 0.0f, 0.0f, 0.0f});
}

bool ReadMapAndFillTerrains(std::vector<ETerrainType>& _terrains)
{
    std::ifstream file("Data/Map.txt"); // Open the file
    if (!file)
        return false;
    
    int iTerrain = 0;
    std::string line;
    while(std::getline(file, line))
    {
        for (char c : line)
        {
            if (c == '0')
            {
                _terrains[iTerrain] = ETerrainType::WATER;
                iTerrain++;
            }
            else if (c == '1')
            {
                _terrains[iTerrain] = ETerrainType::GROUND;
                iTerrain++;
            }
        }
    }
    
    file.close();
    return true;
}
//---------------------------------------------------------------------------------------------------------------------
void CGridScene::OnCreate()
{
    // Generate grid terrain
    {
        GridTerrains.reserve(nbTiles);
        GridTerrains.insert(GridTerrains.end(), nbTiles, ETerrainType::WATER);
        bool Success = ReadMapAndFillTerrains(GridTerrains);
        assert(Success);

        // for (int i = 0; i < GridWidth * GridHeight; ++i)
        // {
        //     int XTile = (i - 1) % GridWidth;
        //     int YTile = (i - 1) / GridWidth;
        //     
        //     int FullWaterBorder = 3;
        //     if (XTile < FullWaterBorder
        //      || YTile < FullWaterBorder
        //      || XTile > GridWidth - FullWaterBorder
        //      || YTile > GridHeight - FullWaterBorder)
        //         continue;
        //     
        //     int LotOfWater = 6;
        //     if (XTile < LotOfWater
        //      || YTile < LotOfWater
        //      || XTile > GridWidth - LotOfWater
        //      || YTile > GridHeight - LotOfWater)
        //     {
        //         if (MMath::RandomNumberIntegerInRange(0, 2) == 0)
        //             continue;
        //     }
        //     
        //     int LessLotOfWater = 10;
        //     if (XTile < LotOfWater
        //      || YTile < LotOfWater
        //      || XTile > GridWidth - LotOfWater
        //      || YTile > GridHeight - LotOfWater)
        //     {
        //         if (MMath::RandomNumberIntegerInRange(0, 4) == 0)
        //             continue;
        //     }
        //     
        //     if (MMath::RandomNumberIntegerInRange(0, 15) == 0)
        //         continue;
        //
        //     GridTerrains[i] = ETerrainType::GROUND;
        // }
    }

    // Generate grid meshes
    {
        InitVisualMeshDataMap();
    
        float HalfTileSize = TileSize * 0.5f;
        float HalfWidthGrid = HalfTileSize * (GridWidth + 1);
        float HalfHeightGrid = HalfTileSize * (GridHeight + 1);

        for (int i = 0; i < nbVisualTiles; ++i)
        {
            const TVisualMeshData* visualMeshData = GetVisualMeshDataFromVisualGridIndex(i);
            if (visualMeshData == nullptr)
                continue;

            const int XTile = i % (GridWidth + 1);
            const int YTile = i / (GridWidth + 1);

            TVector3f position = { 
                (float)(XTile) * TileSize - HalfWidthGrid, 
                (float)(YTile) * -TileSize + HalfHeightGrid, 
                0.0f 
            };
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
