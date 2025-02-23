#pragma once

#include "../IncludesExternal.h"

inline const char* G_ASSET_ARROW = "Arrow";
inline const char* G_ASSET_GIZMO = "Gizmo";
inline const char* G_ASSET_TEXT_DEBUG = "Debugs/TC_Debug";
inline const char* G_ASSET_MESH_ARROW = "Debugs/Arrow";
inline const char* G_ASSET_MESH_GIZMO = "Debugs/Gizmo";

#define TAKU_ASSET_MESH_SQUARE    "Square"
#define TAKU_ASSET_MESH_CUBE      "Cube"
#define TAKU_ASSET_MESH_SPHERE    "Sphere"
#define TAKU_ASSET_MESH_SUZANNE   "Suzanne"
#define TAKU_ASSET_MESH_CRATE     "Crate"
#define TAKU_ASSET_MESH_MONSTER   "Monster"
#define TAKU_ASSET_MESH_TAKUMI    "Takumi"

#define TAKU_ASSET_MESH_TILE_BORDER  "Tiles/TileBorder"
#define TAKU_ASSET_MESH_TILE_0000  "Tiles/Tile0000"
#define TAKU_ASSET_MESH_TILE_1000  "Tiles/Tile1000"
#define TAKU_ASSET_MESH_TILE_1001  "Tiles/Tile1001"
#define TAKU_ASSET_MESH_TILE_1010  "Tiles/Tile1010"
#define TAKU_ASSET_MESH_TILE_1101  "Tiles/Tile1101"
#define TAKU_ASSET_MESH_TILE_1111  "Tiles/Tile1111"
#define TAKU_ASSET_MESH_TILE_2000  "Tiles/Tile2000"
#define TAKU_ASSET_MESH_TILE_2002  "Tiles/Tile2002"
#define TAKU_ASSET_MESH_TILE_2020  "Tiles/Tile2020"
#define TAKU_ASSET_MESH_TILE_2202  "Tiles/Tile2202"
#define TAKU_ASSET_MESH_TILE_2222  "Tiles/Tile2222"

#define TAKU_ASSET_MESH_TREE_01  "Tree01"
#define TAKU_ASSET_MESH_TREE_02  "Tree02"
#define TAKU_ASSET_MESH_TREE_03  "Tree03"

#define TAKU_ASSET_VS_BASE  "VertexShader"
#define TAKU_ASSET_PS_BASE  "PixelShader"

class MAsset
{
public:
    static void LoadMeshes();
    static void LoadShaders();
    static void LoadTextures();
    static void LoadDrawables();
};