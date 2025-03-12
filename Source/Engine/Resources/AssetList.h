#pragma once


inline const char* G_ASSET_TILE_BORDER = "TileBorder";
inline const char* G_ASSET_ARROW = "Arrow";
inline const char* G_ASSET_GIZMO = "Gizmo";

inline const char* G_ASSET_T_NONE_NORMAL = "T_NoneNormal";
inline const char* G_ASSET_T_NONE_WHITE = "T_NoneWhite";
inline const char* G_ASSET_T_NONE_DARK = "T_NoneDark";

inline const char* G_ASSET_T_DEBUG = "Debugs/T_Debug";
inline const char* G_ASSET_MESH_ARROW = "Debugs/Arrow";
inline const char* G_ASSET_MESH_GIZMO = "Debugs/Gizmo";

inline const char* G_ASSET_MESH_TILE_BORDER = "Tiles/TileBorder";

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

#define TAKU_ASSET_VS_2D  "2d_vs"
#define TAKU_ASSET_PS_2D  "2d_ps"
#define TAKU_ASSET_PS_POST_PROCESS_1  "ps_post_process"

#define TAKU_ASSET_VS_SHADOW  "Shadow_vs"

class MAsset
{
public:
    static void LoadAssets();
    static void UnloadAssets();
};