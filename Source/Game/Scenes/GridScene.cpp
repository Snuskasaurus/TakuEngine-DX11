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

constexpr int G_GRID_WIDTH = 20; // X
constexpr int G_GRID_HEIGHT = 20; // Y
constexpr float G_TILE_SIZE = 10.0f;
constexpr float G_TILE_SIZE_HALF = G_TILE_SIZE * 0.5f;
constexpr int G_NB_TILES = G_GRID_WIDTH * G_GRID_HEIGHT;
constexpr int G_NB_TILES_VISUAL = (G_GRID_WIDTH + 1) * (G_GRID_HEIGHT + 1);
constexpr float G_GRID_WIDTH_HALF = G_TILE_SIZE_HALF * (G_GRID_WIDTH + 1);
constexpr float G_GRID_HEIGHT_HALF = G_TILE_SIZE_HALF * (G_GRID_HEIGHT + 1);
bool G_DRAW_TILE_BORDER = false;

bool IsValidGridPosition(int row, int col)
{
    return row >= 0 && row < G_GRID_HEIGHT && col >= 0 && col < G_GRID_WIDTH;
}
bool IsValidVisualGridPosition(int row, int col)
{
    return row >= 0 && row < G_GRID_HEIGHT + 1 && col >= 0 && col < G_GRID_WIDTH + 1;
}

std::vector<ETerrainType> GridTerrains;
std::map<TTerrainIndex, TVisualMeshData> VisualMeshDataMap;

std::vector<TVisualGridIndex> GridIndex_To_VisualGridIndex(const TGridIndex& _gridIndex)
{
    const int row = _gridIndex / G_GRID_WIDTH;
    const int col = _gridIndex % G_GRID_WIDTH;
    return {
        row * (G_GRID_WIDTH + 1) + col,
        row * (G_GRID_WIDTH + 1) + (col + 1),
        (row + 1) * (G_GRID_WIDTH + 1) + (col + 1),
        (row + 1) * (G_GRID_WIDTH + 1) + col,
    };
}

std::vector<TGridIndex> VisualGridIndex_To_GridIndex(const TVisualGridIndex& _visualGridIndex)
{
    const int row = _visualGridIndex / (G_GRID_WIDTH + 1);
    const int col = _visualGridIndex % (G_GRID_WIDTH + 1);
    
    const TGridIndex TopLeft = IsValidGridPosition(row - 1, col - 1) ? (row - 1) * G_GRID_WIDTH + (col - 1) : -1;
    const TGridIndex TopRight = IsValidGridPosition(row - 1, col) ? (row - 1) * G_GRID_WIDTH + col : -1;
    const TGridIndex BotRight = IsValidGridPosition(row, col) ? row * G_GRID_WIDTH + col : -1;
    const TGridIndex BotLeft = IsValidGridPosition(row, col - 1) ? row * G_GRID_WIDTH + (col - 1) : -1;
    
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
    
    Mesh0000->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 0, 0.0f, -20.0f }, 0.0f, 0.0f, 0.0f});

    Mesh1000->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 1, -(G_TILE_SIZE + 1.0f) * 0, -20.0f }, 0.0f, 0.0f, 0.0f});
    Mesh1000->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 1, -(G_TILE_SIZE + 1.0f) * 1, -20.0f }, MMath::Deg2Rad(-90.0f), 0.0f, 0.0f});
    Mesh1000->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 1, -(G_TILE_SIZE + 1.0f) * 2, -20.0f }, MMath::Deg2Rad(-180.0f), 0.0f, 0.0f});
    Mesh1000->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 1, -(G_TILE_SIZE + 1.0f) * 3, -20.0f }, MMath::Deg2Rad(-270.0f), 0.0f, 0.0f});

    Mesh1001->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 2, -(G_TILE_SIZE + 1.0f) * 0, -20.0f }, 0.0f, 0.0f, 0.0f});
    Mesh1001->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 2, -(G_TILE_SIZE + 1.0f) * 1, -20.0f }, MMath::Deg2Rad(-90.0f), 0.0f, 0.0f});
    Mesh1001->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 2, -(G_TILE_SIZE + 1.0f) * 2, -20.0f }, MMath::Deg2Rad(-180.0f), 0.0f, 0.0f});
    Mesh1001->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 2, -(G_TILE_SIZE + 1.0f) * 3, -20.0f }, MMath::Deg2Rad(-270.0f), 0.0f, 0.0f});
    
    Mesh1010->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 3, -(G_TILE_SIZE + 1.0f) * 0, -20.0f }, 0.0f, 0.0f, 0.0f});
    Mesh1010->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 3, -(G_TILE_SIZE + 1.0f) * 1, -20.0f }, MMath::Deg2Rad(-90.0f), 0.0f, 0.0f});
    
    Mesh1101->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 4, -(G_TILE_SIZE + 1.0f) * 0, -20.0f }, 0.0f, 0.0f, 0.0f});
    Mesh1101->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 4, -(G_TILE_SIZE + 1.0f) * 1, -20.0f }, MMath::Deg2Rad(-90.0f), 0.0f, 0.0f});
    Mesh1101->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 4, -(G_TILE_SIZE + 1.0f) * 2, -20.0f }, MMath::Deg2Rad(-180.0f), 0.0f, 0.0f});
    Mesh1101->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 4, -(G_TILE_SIZE + 1.0f) * 3, -20.0f }, MMath::Deg2Rad(-270.0f), 0.0f, 0.0f});
    
    Mesh1111->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 5, 0.0f, -20.0f }, 0.0f, 0.0f, 0.0f});
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
        GridTerrains.reserve(G_NB_TILES);
        GridTerrains.insert(GridTerrains.end(), G_NB_TILES, ETerrainType::WATER);
        bool Success = ReadMapAndFillTerrains(GridTerrains);
        assert(Success);
    }
    
    
    // Generate grid meshes
    {
        InitVisualMeshDataMap();
        
        for (int i = 0; i < G_NB_TILES_VISUAL; ++i)
        {
            const TVisualMeshData* visualMeshData = GetVisualMeshDataFromVisualGridIndex(i);
            if (visualMeshData == nullptr)
                continue;

            const int XTile = i % (G_GRID_WIDTH + 1);
            const int YTile = i / (G_GRID_WIDTH + 1);

            TVector3f position = { 
                (float)(XTile) * G_TILE_SIZE - G_GRID_WIDTH_HALF - G_TILE_SIZE_HALF, 
                (float)(YTile) * -G_TILE_SIZE + G_GRID_HEIGHT_HALF + G_TILE_SIZE_HALF, 
                0.0f 
            };
            TTransform Transform = { position, visualMeshData->Rotation, 0.0f, 0.0f };
            visualMeshData->InstancedMesh->Instances.push_back(Transform);
        }
    }
    
    CDrawable_InstancedMesh* TreeMesh1 = this->AddInstancedMeshToDraw(TAKU_ASSET_MESH_TREE_01);
    CDrawable_InstancedMesh* TreeMesh2 = this->AddInstancedMeshToDraw(TAKU_ASSET_MESH_TREE_02);
    CDrawable_InstancedMesh* TreeMesh3 = this->AddInstancedMeshToDraw(TAKU_ASSET_MESH_TREE_03);
    
    for (int i = 0; i < G_NB_TILES; ++i)
    {
        const int XTile = i % G_GRID_WIDTH;
        const int YTile = i / G_GRID_WIDTH;

        TVector3f tilePosition = { (float)(XTile) * G_TILE_SIZE - G_GRID_WIDTH_HALF, (float)(YTile) * -G_TILE_SIZE + G_GRID_HEIGHT_HALF, 0.0f };
        TTransform tileTransform = { tilePosition, { 0.0f, 0.0f, 0.0f }};
        
        if (GridTerrains[i] != ETerrainType::GROUND)
            continue;
         
        if (MMath::RandomNumberIntegerInRange(0, 8) >= 1)
        {
            float sizeChange = G_TILE_SIZE / 9 * 3.0f;
            float startOffsetX = -G_TILE_SIZE_HALF + sizeChange * 0.5f;
            float startOffsetY = -G_TILE_SIZE_HALF + sizeChange * 0.5f;

            float MaxRandOffset = sizeChange * 0.35f;
            
            for (int xOffset = 0; xOffset < 3; ++xOffset)
            {
                for (int yOffset = 0; yOffset < 3; ++yOffset)
                {
                    if (MMath::RandomNumberIntegerInRange(0, 8) == 0)
                        continue;
                    
                    float randOffsetX = MMath::RandomNumberIntegerInRange(-MaxRandOffset * 100.0f, MaxRandOffset * 100.0f) / 100.0f;
                    float randOffsetY = MMath::RandomNumberIntegerInRange(-MaxRandOffset * 100.0f, MaxRandOffset * 100.0f) / 100.0f;
                    
                    float offsetX = startOffsetX + sizeChange * (float)xOffset + randOffsetX;
                    float offsetY = startOffsetY + sizeChange * (float)yOffset + randOffsetY;
                    TVector3f positionTree = tilePosition + TVector3f(offsetX, offsetY, 0.35f);
                    TRotator rotationTree = { MMath::Deg2Rad(MMath::RandomNumberIntegerInRange(0.0f, 360.0f)), 0.0f, 0.0f };
                
                    int varTree = MMath::RandomNumberIntegerInRange(1, 4);
                    switch (varTree)
                    {
                        case 1: TreeMesh1->Instances.push_back({positionTree, rotationTree }); break;
                        case 2: TreeMesh2->Instances.push_back({positionTree, rotationTree }); break;
                        case 3: TreeMesh3->Instances.push_back({positionTree, rotationTree }); break;
                    }
                }
            }
        }
    }
}

void CGridScene::ToggleDisplayingGrid()
{
    if (G_DRAW_TILE_BORDER == false)
    {
        G_DRAW_TILE_BORDER = true;
        
        if (TileBorderMesh == nullptr)
            TileBorderMesh = this->AddInstancedMeshToDraw(TAKU_ASSET_MESH_TILE_BORDER);

        for (int i = 0; i < G_NB_TILES; ++i)
        {
            const int XTile = i % G_GRID_WIDTH;
            const int YTile = i / G_GRID_WIDTH;
            TVector3f tilePosition = { (float)(XTile) * G_TILE_SIZE - G_GRID_WIDTH_HALF, (float)(YTile) * -G_TILE_SIZE + G_GRID_HEIGHT_HALF, 0.0f };
            TVector3f tileBorderPosition = tilePosition + TVector3f(0.0f, 0.0f, 0.0f);
            TTransform tileBorderTransform = { tileBorderPosition, { 0.0f, 0.0f, 0.0f }};
            TileBorderMesh->Instances.push_back(tileBorderTransform);
        }
    }
    else
    {
        G_DRAW_TILE_BORDER = false;
        TileBorderMesh->Instances.clear();
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
        ToggleDisplayingGrid();
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
