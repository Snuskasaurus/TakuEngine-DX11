#pragma once

inline const char* G_ASSET_ARROW = "Arrow";
inline const char* G_ASSET_GIZMO = "Gizmo";
inline const char* G_ASSET_2D_CIRCLE = "2D_Circle";

inline const char* G_ASSET_T_NONE_NORMAL = "T_NoneNormal";
inline const char* G_ASSET_T_NONE_WHITE = "T_NoneWhite";
inline const char* G_ASSET_T_NONE_DARK = "T_NoneDark";

inline const char* G_ASSET_T_DEBUG = "Debugs/T_Debug";
inline const char* G_ASSET_MESH_ARROW = "Debugs/Arrow";
inline const char* G_ASSET_MESH_GIZMO = "Debugs/Gizmo";

inline const char* G_ASSET_MESH_TILE_0000 = "Tiles/Tile0000";
inline const char* G_ASSET_MESH_TILE_1000 = "Tiles/Tile1000";
inline const char* G_ASSET_MESH_TILE_1001 = "Tiles/Tile1001";
inline const char* G_ASSET_MESH_TILE_1010 = "Tiles/Tile1010";
inline const char* G_ASSET_MESH_TILE_1101 = "Tiles/Tile1101";
inline const char* G_ASSET_MESH_TILE_1111 = "Tiles/Tile1111";
inline const char* G_ASSET_MESH_TILE_2000 = "Tiles/Tile2000";
inline const char* G_ASSET_MESH_TILE_2002 = "Tiles/Tile2002";
inline const char* G_ASSET_MESH_TILE_2020 = "Tiles/Tile2020";
inline const char* G_ASSET_MESH_TILE_2202 = "Tiles/Tile2202";
inline const char* G_ASSET_MESH_TILE_2222 = "Tiles/Tile2222";

inline const char* G_ASSET_TILE_0000 = "Tile0000";
inline const char* G_ASSET_TILE_1000 = "Tile1000";
inline const char* G_ASSET_TILE_1001 = "Tile1001";
inline const char* G_ASSET_TILE_1010 = "Tile1010";
inline const char* G_ASSET_TILE_1101 = "Tile1101";
inline const char* G_ASSET_TILE_1111 = "Tile1111";
inline const char* G_ASSET_TILE_2000 = "Tile2000";
inline const char* G_ASSET_TILE_2002 = "Tile2002";
inline const char* G_ASSET_TILE_2020 = "Tile2020";
inline const char* G_ASSET_TILE_2202 = "Tile2202";
inline const char* G_ASSET_TILE_2222 = "Tile2222";
inline const char* G_ASSET_TILE_BORDER = "TileBorder";
inline const char* G_ASSET_MESH_TILE_BORDER = "Tiles/TileBorder";
inline const char* G_ASSET_T_TILES_C = "Tiles/T_TileColor";
inline const char* G_ASSET_T_TILES_SO= "Tiles/T_TileSO";

#define TAKU_ASSET_MESH_TREE_01  "Tree01"
#define TAKU_ASSET_MESH_TREE_02  "Tree02"
#define TAKU_ASSET_MESH_TREE_03  "Tree03"

#define TAKU_ASSET_SHADER_VS_DEBUG_DRAW  "vs_debug_draw"
#define TAKU_ASSET_SHADER_VS_BASE  "vs_instanced_mesh"
#define TAKU_ASSET_SHADER_PS_BASE  "ps_instanced_mesh"
#define TAKU_ASSET_SHADER_VS_2D  "vs_2d"
#define TAKU_ASSET_SHADER_PS_DEBUG_SCREEN  "ps_debug_screen"
#define TAKU_ASSET_SHADER_PS_POST_PROCESS_1  "ps_post_process"
#define TAKU_ASSET_SHADER_PS_SIMPLE_COLOR  "ps_simple_color"
#define TAKU_ASSET_SHADER_VS_SHADOW  "vs_shadow_map"

class MAsset
{
public:
    static void LoadAssets();
    static void UnloadAssets();
};