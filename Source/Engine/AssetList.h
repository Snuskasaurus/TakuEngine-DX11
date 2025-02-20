#pragma once

#define TAKU_ASSET_MESH_SQUARE    "Square"
#define TAKU_ASSET_MESH_CUBE      "Cube"
#define TAKU_ASSET_MESH_SPHERE    "Sphere"
#define TAKU_ASSET_MESH_SUZANNE   "Suzanne"
#define TAKU_ASSET_MESH_CRATE     "Crate"
#define TAKU_ASSET_MESH_MONSTER   "Monster"
#define TAKU_ASSET_MESH_TAKUMI    "Takumi"

#define TAKU_ASSET_MESH_TILE_0000  "Tiles/Tile0000"
#define TAKU_ASSET_MESH_TILE_1000  "Tiles/Tile1000"
#define TAKU_ASSET_MESH_TILE_1001  "Tiles/Tile1001"
#define TAKU_ASSET_MESH_TILE_1010  "Tiles/Tile1010"
#define TAKU_ASSET_MESH_TILE_1101  "Tiles/Tile1101"
#define TAKU_ASSET_MESH_TILE_1111  "Tiles/Tile1111"

#define TAKU_ASSET_VS_BASE  "VertexShader"
#define TAKU_ASSET_PS_BASE  "PixelShader"

class MAsset
{
public:
    static void LoadMeshes();
    static void LoadShaders();
};