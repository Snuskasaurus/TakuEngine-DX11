#include "MGridSystem.h"

#include "../Engine/World.h"
#include "../Engine/Math.h"
#include "../Engine/Resources/AssetList.h"
#include "../Engine/Debug/DebugDraw.h"

#define DEBUG_DISPLAY_GRID_TERRAINS 1 
#define DEBUG_DISPLAY_GRID_PATH_TERRAINS_COST 1
#define ENABLE_GENERATE_FORESTS 0

typedef int TTerrainIndex;
typedef int TGridIndex;
typedef int TVisualGridIndex;

typedef uint8_t TTerrainCost;

struct TVisualMeshData
{
    CDrawable_InstancedMesh* InstancedMesh;
    float Rotation;
};
 
enum ETerrainType
{
    WATER,
    GROUND,
    MOUNTAIN,
    FOREST,
};

constexpr int G_GRID_WIDTH = 20; // X
constexpr int G_GRID_HEIGHT = 20; // Y
constexpr float G_TILE_SIZE = 15.0f;
constexpr float G_TILE_SIZE_HALF = G_TILE_SIZE * 0.5f;
constexpr float G_TILE_SIZE_THIRD = G_TILE_SIZE * 0.33f;
constexpr int G_NB_TILES = G_GRID_WIDTH * G_GRID_HEIGHT;

constexpr int G_GRID_WIDTH_VISUAL = G_GRID_WIDTH + 1;
constexpr int G_GRID_HEIGHT_VISUAL = G_GRID_HEIGHT + 1;
constexpr int G_NB_TILES_VISUAL = G_GRID_WIDTH_VISUAL * G_GRID_HEIGHT_VISUAL;

constexpr int G_NB_TILES_PATH = G_NB_TILES * 9;

constexpr float G_GRID_WIDTH_HALF = G_TILE_SIZE_HALF * (G_GRID_WIDTH - 1);
constexpr float G_GRID_HEIGHT_HALF = G_TILE_SIZE_HALF * (G_GRID_HEIGHT - 1);

// constexpr float G_GRID_GLOBAL_OFFSET_X = G_GRID_WIDTH_HALF;
// constexpr float G_GRID_GLOBAL_OFFSET_Y = G_GRID_HEIGHT_HALF;
constexpr float G_GRID_GLOBAL_OFFSET_X = 0;
constexpr float G_GRID_GLOBAL_OFFSET_Y = 0;

static std::vector<ETerrainType> G_GRID_TERRAINS;
static std::vector<TTerrainCost> G_GRID_PATH_TERRAINS_COST;
std::map<TTerrainIndex, TVisualMeshData> G_VISUAL_MESH_DATA_MAP;
bool G_DRAW_TILE_BORDER = false;
CDrawable_InstancedMesh* G_TILE_BORDER_MESH;
//---------------------------------------------------------------------------------------------------------------------
bool IsValidGridPosition(int row, int col)
{
    return row >= 0 && row < G_GRID_HEIGHT && col >= 0 && col < G_GRID_WIDTH;
}
//---------------------------------------------------------------------------------------------------------------------
bool IsValidVisualGridPosition(int row, int col)
{
    return row >= 0 && row < G_GRID_HEIGHT + 1 && col >= 0 && col < G_GRID_WIDTH + 1;
}
//---------------------------------------------------------------------------------------------------------------------
std::vector<TVisualGridIndex> GridIndex_To_VisualGridIndex(const TGridIndex& _gridIndex)
{
    const int row = _gridIndex / G_GRID_WIDTH;
    const int col = _gridIndex % G_GRID_WIDTH;
    return{
        row * (G_GRID_WIDTH_VISUAL) + col,
        row * (G_GRID_WIDTH_VISUAL) + (col + 1),
        (row + 1) * (G_GRID_WIDTH_VISUAL) + (col + 1),
        (row + 1) * (G_GRID_WIDTH_VISUAL) + col,
    };
}
//---------------------------------------------------------------------------------------------------------------------
std::vector<TGridIndex> VisualGridIndex_To_GridIndex(const TVisualGridIndex& _visualGridIndex)
{
    const int row = _visualGridIndex / (G_GRID_WIDTH + 1);
    const int col = _visualGridIndex % (G_GRID_WIDTH + 1);
    
    const TGridIndex topLeft = IsValidGridPosition(row - 1, col - 1) ? (row - 1) * G_GRID_WIDTH + (col - 1) : -1;
    const TGridIndex topRight = IsValidGridPosition(row - 1, col) ? (row - 1) * G_GRID_WIDTH + col : -1;
    const TGridIndex botRight = IsValidGridPosition(row, col) ? row * G_GRID_WIDTH + col : -1;
    const TGridIndex botLeft = IsValidGridPosition(row, col - 1) ? row * G_GRID_WIDTH + (col - 1) : -1;
    
    return { topLeft, topRight, botRight, botLeft };
}
//---------------------------------------------------------------------------------------------------------------------
TVisualMeshData* GetVisualMeshDataFromVisualGridIndex(const TVisualGridIndex& _visualGridIndex, bool IsMountain)
{
    const std::vector<TGridIndex> TileIndexes = VisualGridIndex_To_GridIndex(_visualGridIndex);
    TTerrainIndex terrainIndex = 0;
    for (int i = 0; i < 4; i++) {
        TTerrainIndex TerrainType = TileIndexes[i] >= 0 ? (TTerrainIndex)G_GRID_TERRAINS[TileIndexes[i]] : 0;
        if (IsMountain)
        {
            if (TerrainType != ETerrainType::WATER && TerrainType != ETerrainType::MOUNTAIN)
                TerrainType = (TTerrainIndex) ETerrainType::WATER;
        }
        else
        {
            if (TerrainType != ETerrainType::WATER && TerrainType != ETerrainType::GROUND)
                TerrainType = (TTerrainIndex) ETerrainType::GROUND;
        }
        
        terrainIndex = terrainIndex * 10 + TerrainType;
    }
    if (IsMountain && terrainIndex == 0)
        return nullptr;
    
    const auto visualMeshDataMapIterator = G_VISUAL_MESH_DATA_MAP.find(terrainIndex);
    if (visualMeshDataMapIterator == G_VISUAL_MESH_DATA_MAP.end())
        return nullptr;

    return &visualMeshDataMapIterator->second;
}
//---------------------------------------------------------------------------------------------------------------------
void InitVisualMeshDataMap()
{
    CGameScene* GameScene = MWorld::GetWorld()->GetCurrentScene();
    
    CDrawable_InstancedMesh* Mesh0000 = GameScene->AddInstancedMeshToDrawFromDrawableData(G_ASSET_TILE_0000);
    CDrawable_InstancedMesh* Mesh1000 = GameScene->AddInstancedMeshToDrawFromDrawableData(G_ASSET_TILE_1000);
    CDrawable_InstancedMesh* Mesh1001 = GameScene->AddInstancedMeshToDrawFromDrawableData(G_ASSET_TILE_1001);
    CDrawable_InstancedMesh* Mesh1010 = GameScene->AddInstancedMeshToDrawFromDrawableData(G_ASSET_TILE_1010);
    CDrawable_InstancedMesh* Mesh1101 = GameScene->AddInstancedMeshToDrawFromDrawableData(G_ASSET_TILE_1101);
    CDrawable_InstancedMesh* Mesh1111 = GameScene->AddInstancedMeshToDrawFromDrawableData(G_ASSET_TILE_1111);
    CDrawable_InstancedMesh* Mesh2000 = GameScene->AddInstancedMeshToDrawFromDrawableData(G_ASSET_TILE_2000);
    CDrawable_InstancedMesh* Mesh2002 = GameScene->AddInstancedMeshToDrawFromDrawableData(G_ASSET_TILE_2002);
    CDrawable_InstancedMesh* Mesh2020 = GameScene->AddInstancedMeshToDrawFromDrawableData(G_ASSET_TILE_2020);
    CDrawable_InstancedMesh* Mesh2202 = GameScene->AddInstancedMeshToDrawFromDrawableData(G_ASSET_TILE_2202);
    CDrawable_InstancedMesh* Mesh2222 = GameScene->AddInstancedMeshToDrawFromDrawableData(G_ASSET_TILE_2222);
    
    G_VISUAL_MESH_DATA_MAP.insert({0, {Mesh0000, 0.0f}});

    // Ground tiles
    {
        G_VISUAL_MESH_DATA_MAP.insert({1000, {Mesh1000, 0.0f}});
        G_VISUAL_MESH_DATA_MAP.insert({100,  {Mesh1000, MMath::Deg2Rad(-90.0f)}});
        G_VISUAL_MESH_DATA_MAP.insert({10,   {Mesh1000, MMath::Deg2Rad(-180.0f)}});
        G_VISUAL_MESH_DATA_MAP.insert({1,    {Mesh1000, MMath::Deg2Rad(-270.0f)}});
    
        G_VISUAL_MESH_DATA_MAP.insert({1001, {Mesh1001, 0.0f}});
        G_VISUAL_MESH_DATA_MAP.insert({1100, {Mesh1001, MMath::Deg2Rad(-90.0f)}});
        G_VISUAL_MESH_DATA_MAP.insert({110,  {Mesh1001, MMath::Deg2Rad(-180.0f)}});
        G_VISUAL_MESH_DATA_MAP.insert({11,   {Mesh1001, MMath::Deg2Rad(-270.0f)}});
    
        G_VISUAL_MESH_DATA_MAP.insert({1010, {Mesh1010, 0.0f}});
        G_VISUAL_MESH_DATA_MAP.insert({101,  {Mesh1010, MMath::Deg2Rad(-90.0f)}});
    
        G_VISUAL_MESH_DATA_MAP.insert({1101, {Mesh1101, 0.0f}});
        G_VISUAL_MESH_DATA_MAP.insert({1110, {Mesh1101, MMath::Deg2Rad(-90.0f)}});
        G_VISUAL_MESH_DATA_MAP.insert({111,  {Mesh1101, MMath::Deg2Rad(-180.0f)}});
        G_VISUAL_MESH_DATA_MAP.insert({1011, {Mesh1101, MMath::Deg2Rad(-270.0f)}});
        
        G_VISUAL_MESH_DATA_MAP.insert({1111, {Mesh1111, 0.0f}});
    }

    // Mountain tiles
    {
        G_VISUAL_MESH_DATA_MAP.insert({2000, {Mesh2000, 0.0f}});
        G_VISUAL_MESH_DATA_MAP.insert({200,  {Mesh2000, MMath::Deg2Rad(-90.0f)}});
        G_VISUAL_MESH_DATA_MAP.insert({20,   {Mesh2000, MMath::Deg2Rad(-180.0f)}});
        G_VISUAL_MESH_DATA_MAP.insert({2, 	  {Mesh2000, MMath::Deg2Rad(-270.0f)}});
									 
        G_VISUAL_MESH_DATA_MAP.insert({2002, {Mesh2002, 0.0f}});
        G_VISUAL_MESH_DATA_MAP.insert({2200, {Mesh2002, MMath::Deg2Rad(-90.0f)}});
        G_VISUAL_MESH_DATA_MAP.insert({220,  {Mesh2002, MMath::Deg2Rad(-180.0f)}});
        G_VISUAL_MESH_DATA_MAP.insert({22,   {Mesh2002, MMath::Deg2Rad(-270.0f)}});
									 
        G_VISUAL_MESH_DATA_MAP.insert({2020, {Mesh2020, 0.0f}});
        G_VISUAL_MESH_DATA_MAP.insert({202,  {Mesh2020, MMath::Deg2Rad(-90.0f)}});
									 
        G_VISUAL_MESH_DATA_MAP.insert({2202, {Mesh2202, 0.0f}});
        G_VISUAL_MESH_DATA_MAP.insert({2220, {Mesh2202, MMath::Deg2Rad(-90.0f)}});
        G_VISUAL_MESH_DATA_MAP.insert({222,  {Mesh2202, MMath::Deg2Rad(-180.0f)}});
        G_VISUAL_MESH_DATA_MAP.insert({2022, {Mesh2202, MMath::Deg2Rad(-270.0f)}});

        G_VISUAL_MESH_DATA_MAP.insert({2222, {Mesh2222, 0.0f}});
    }
    
#if _DEBUG // Draw all the available tiles at the bottom of the map
    Mesh0000->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 0, 0.0f, -200.0f }, 0.0f, 0.0f, 0.0f});

    Mesh1000->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 1, -(G_TILE_SIZE + 1.0f) * 0, -200.0f }, 0.0f, 0.0f, 0.0f});
    Mesh1000->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 1, -(G_TILE_SIZE + 1.0f) * 1, -200.0f }, MMath::Deg2Rad(-90.0f), 0.0f, 0.0f});
    Mesh1000->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 1, -(G_TILE_SIZE + 1.0f) * 2, -200.0f }, MMath::Deg2Rad(-180.0f), 0.0f, 0.0f});
    Mesh1000->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 1, -(G_TILE_SIZE + 1.0f) * 3, -200.0f }, MMath::Deg2Rad(-270.0f), 0.0f, 0.0f});

    Mesh1001->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 2, -(G_TILE_SIZE + 1.0f) * 0, -200.0f }, 0.0f, 0.0f, 0.0f});
    Mesh1001->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 2, -(G_TILE_SIZE + 1.0f) * 1, -200.0f }, MMath::Deg2Rad(-90.0f), 0.0f, 0.0f});
    Mesh1001->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 2, -(G_TILE_SIZE + 1.0f) * 2, -200.0f }, MMath::Deg2Rad(-180.0f), 0.0f, 0.0f});
    Mesh1001->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 2, -(G_TILE_SIZE + 1.0f) * 3, -200.0f }, MMath::Deg2Rad(-270.0f), 0.0f, 0.0f});
    
    Mesh1010->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 3, -(G_TILE_SIZE + 1.0f) * 0, -200.0f }, 0.0f, 0.0f, 0.0f});
    Mesh1010->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 3, -(G_TILE_SIZE + 1.0f) * 1, -200.0f }, MMath::Deg2Rad(-90.0f), 0.0f, 0.0f});
    
    Mesh1101->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 4, -(G_TILE_SIZE + 1.0f) * 0, -200.0f }, 0.0f, 0.0f, 0.0f});
    Mesh1101->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 4, -(G_TILE_SIZE + 1.0f) * 1, -200.0f }, MMath::Deg2Rad(-90.0f), 0.0f, 0.0f});
    Mesh1101->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 4, -(G_TILE_SIZE + 1.0f) * 2, -200.0f }, MMath::Deg2Rad(-180.0f), 0.0f, 0.0f});
    Mesh1101->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 4, -(G_TILE_SIZE + 1.0f) * 3, -200.0f }, MMath::Deg2Rad(-270.0f), 0.0f, 0.0f});
    
    Mesh1111->Instances.push_back({{(G_TILE_SIZE + 1.0f) * 5, 0.0f, -200.0f }, 0.0f, 0.0f, 0.0f});
#endif
}
//---------------------------------------------------------------------------------------------------------------------
TTerrainCost ConvertTerrainTypeToTerrainCost(ETerrainType _terrainType)
{
    switch (_terrainType)
    {
        case GROUND: return 2u;
        case FOREST: return 4u;
    }
    return 0u;
}
//---------------------------------------------------------------------------------------------------------------------
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
            else if (c == '2')
            {
                _terrains[iTerrain] = ETerrainType::MOUNTAIN;
                iTerrain++;
            }
        }
    }
    
    file.close();
    return true;
}
//---------------------------------------------------------------------------------------------------------------------
void MGridSystem::CreateGrid()
{
    // Generate grid terrain
    {
        G_GRID_TERRAINS.reserve(G_NB_TILES);
        G_GRID_TERRAINS.insert(std::begin(G_GRID_TERRAINS), G_NB_TILES, ETerrainType::WATER);
        const bool success = ReadMapAndFillTerrains(G_GRID_TERRAINS);
        assert(success);
    }

    // Initialize path cost grid
    G_GRID_PATH_TERRAINS_COST.reserve(G_NB_TILES_PATH);
    G_GRID_PATH_TERRAINS_COST.insert(G_GRID_PATH_TERRAINS_COST.end(), G_NB_TILES_PATH, 1);

    InitVisualMeshDataMap();
    
    // Create the terrain meshes from visual grid
    for (int i = 0; i < G_NB_TILES_VISUAL; ++i)
    {
        const int xTile = i % G_GRID_WIDTH_VISUAL;
        const int yTile = (G_GRID_WIDTH_VISUAL -1 ) - i / G_GRID_WIDTH_VISUAL;

        const TVector3f position =
        { 
            (static_cast<float>(xTile) * G_TILE_SIZE) - G_GRID_GLOBAL_OFFSET_X - G_TILE_SIZE_HALF, 
            (static_cast<float>(yTile) * G_TILE_SIZE) - G_GRID_GLOBAL_OFFSET_Y - G_TILE_SIZE_HALF, 
            0.0f 
        };

        // Add ground/water tiles
        { 
            const TVisualMeshData* visualMeshData = GetVisualMeshDataFromVisualGridIndex(i, false);
            if (visualMeshData != nullptr)
            {
                TTransform transform = { position, visualMeshData->Rotation, 0.0f, 0.0f };
                visualMeshData->InstancedMesh->Instances.push_back(transform);
            }
        }
        // Add Mountain tiles
        {
            const TVisualMeshData* visualMeshData = GetVisualMeshDataFromVisualGridIndex(i, true);
            if (visualMeshData != nullptr)
            {
                TTransform transform = { position, visualMeshData->Rotation, 0.0f, 0.0f };
                visualMeshData->InstancedMesh->Instances.push_back(transform);
            }
        }
    }
    
    CDrawable_InstancedMesh* TreeMesh1 = MWorld::GetWorld()->GetCurrentScene()->AddInstancedMeshToDraw_DEPRECATED(TAKU_ASSET_MESH_TREE_01);
    CDrawable_InstancedMesh* TreeMesh2 = MWorld::GetWorld()->GetCurrentScene()->AddInstancedMeshToDraw_DEPRECATED(TAKU_ASSET_MESH_TREE_02);
    CDrawable_InstancedMesh* TreeMesh3 = MWorld::GetWorld()->GetCurrentScene()->AddInstancedMeshToDraw_DEPRECATED(TAKU_ASSET_MESH_TREE_03);
    
    for (int iTile = 0; iTile < G_NB_TILES; ++iTile)
    {
        const int xTile = iTile % G_GRID_WIDTH;
        const int yTile = (G_GRID_HEIGHT - 1) - (iTile / G_GRID_WIDTH);
        
        if (xTile > 10) continue;
        if (yTile > 10) continue;
        
        TVector3f tilePosition =
            {
            static_cast<float>(xTile) * G_TILE_SIZE - G_GRID_GLOBAL_OFFSET_X,
            static_cast<float>(yTile) * G_TILE_SIZE - G_GRID_GLOBAL_OFFSET_Y,
            0.0f
        };
        TTransform tileTransform = { tilePosition, { 0.0f, 0.0f, 0.0f }};
        
#if DEBUG_DISPLAY_GRID_TERRAINS
        TColor colorLine = TColor::Blue;
        if (G_GRID_TERRAINS[iTile] == ETerrainType::GROUND)
        {
            colorLine = TColor::Green;
        }
        MDebugDraw::Line({tilePosition}, {tilePosition + TVector3f::Up * 20.0f}, colorLine);
#endif

        // Create forests
#if ENABLE_GENERATE_FORESTS
        if (G_GRID_TERRAINS[i] == ETerrainType::GROUND)
        {
            if (MMath::RandomNumberIntegerInRange(0, 100) <= 25)
            {
                G_GRID_TERRAINS[i] = ETerrainType::FOREST;
            
                constexpr float sizeChange = G_TILE_SIZE / 9 * 3.0f;
                constexpr float startOffsetX = -G_TILE_SIZE_HALF + sizeChange * 0.5f;
                constexpr float startOffsetY = -G_TILE_SIZE_HALF + sizeChange * 0.5f;

                constexpr float MaxRandOffset = sizeChange * 0.35f;
            
                for (int xOffset = 0; xOffset < 3; ++xOffset)
                {
                    for (int yOffset = 0; yOffset < 3; ++yOffset)
                    {
                        if (MMath::RandomNumberIntegerInRange(0, 8) == 0)
                            continue;

                        const float randOffsetX = MMath::RandomNumberIntegerInRange(-MaxRandOffset * 100.0f, MaxRandOffset * 100.0f) / 100.0f;
                        const float randOffsetY = MMath::RandomNumberIntegerInRange(-MaxRandOffset * 100.0f, MaxRandOffset * 100.0f) / 100.0f;
                    
                        const float offsetX = startOffsetX + sizeChange * (float)xOffset + randOffsetX;
                        const float offsetY = startOffsetY + sizeChange * (float)yOffset + randOffsetY;
                        const TVector3f positionTree = tilePosition + TVector3f(offsetX, offsetY, 0.0f);
                        const TRotator rotationTree = { MMath::Deg2Rad(MMath::RandomNumberIntegerInRange(0.0f, 360.0f)), MMath::Deg2Rad(MMath::RandomNumberIntegerInRange(-5.0f, 5.0f)), 0.0f };

                        const int varTree = MMath::RandomNumberIntegerInRange(1, 4);
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
#endif

        // Create paths costs
        for (int iPath = 0; iPath < 5; ++iPath)
        {
            const int xTilePath = iPath % 3;
            const int yTilePath = 2 - (iPath / 3);
            
            const int indexPathGrid = iTile * 9 + iPath;
            
            TVector3f tilePositionOffset =
            {
                static_cast<float>(xTilePath) * G_TILE_SIZE_THIRD - G_TILE_SIZE_THIRD,
                static_cast<float>(yTilePath) * G_TILE_SIZE_THIRD - G_TILE_SIZE_THIRD,
                0.0f
            };

            G_GRID_PATH_TERRAINS_COST[indexPathGrid] = ConvertTerrainTypeToTerrainCost(G_GRID_TERRAINS[iTile]);
        
#if DEBUG_DISPLAY_GRID_PATH_TERRAINS_COST
            //if (G_GRID_PATH_TERRAINS_COST[indexPathGrid] > 0)
            {
                TColor color = TColor::Black;
                if (yTilePath == 0)    color = TColor::Red;
                if (yTilePath == 1)    color = TColor::Green;
                if (yTilePath == 2)    color = TColor::Blue;
                // switch (G_GRID_PATH_TERRAINS_COST[indexPathGrid])
                // {
                //     case 2: color = TColor::Green; break;
                //     case 3: color = TColor(255/2, 255/2, 0); break;
                //     case 4: color = TColor::Red; break;
                // }
                MDebugDraw::Line({tilePosition + tilePositionOffset}, {tilePosition + tilePositionOffset + TVector3f::Up * 2.0f}, color);
            }
#endif
        }
    }
}
//---------------------------------------------------------------------------------------------------------------------
void MGridSystem::ToggleDisplayingGrid()
{
    if (G_DRAW_TILE_BORDER == false)
    {
        G_DRAW_TILE_BORDER = true;
        
        if (G_TILE_BORDER_MESH == nullptr)
            G_TILE_BORDER_MESH = MWorld::GetWorld()->GetCurrentScene()->AddInstancedMeshToDrawFromDrawableData(G_ASSET_TILE_BORDER);

        for (int i = 0; i < G_NB_TILES; ++i)
        {
            const int XTile = i % G_GRID_WIDTH;
            const int YTile = i / G_GRID_WIDTH;
            TVector3f tilePosition =
                {
                static_cast<float>(XTile) * G_TILE_SIZE - G_GRID_GLOBAL_OFFSET_X,
                static_cast<float>(YTile) * -G_TILE_SIZE + G_GRID_GLOBAL_OFFSET_Y,
                0.0f
                };
            TTransform tileBorderTransform = { tilePosition, { 0.0f, 0.0f, 0.0f }};
            G_TILE_BORDER_MESH->Instances.push_back(tileBorderTransform);
        }
    }
    else
    {
        G_DRAW_TILE_BORDER = false;
        G_TILE_BORDER_MESH->Instances.clear();
    }
}
//---------------------------------------------------------------------------------------------------------------------
