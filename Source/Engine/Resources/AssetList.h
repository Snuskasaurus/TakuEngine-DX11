#pragma once

inline const char* G_ASSET_SHAPE_TORUS = "Torus";

inline const char* G_ASSET_ARROW = "Arrow";
inline const char* G_ASSET_GIZMO = "Gizmo";
inline const char* G_ASSET_2D_CIRCLE = "2D_Circle";

inline const char* G_ASSET_T_NONE_NORMAL = "T_NoneNormal.bmp";
inline const char* G_ASSET_T_NONE_WHITE =  "T_NoneWhite.bmp";
inline const char* G_ASSET_T_NONE_DARK =   "T_NoneDark.bmp";

inline const char* G_ASSET_T_DEBUG = "Debugs/T_Debug";
inline const char* G_ASSET_MESH_ARROW = "Debugs/Arrow";
inline const char* G_ASSET_MESH_GIZMO = "Debugs/Gizmo";

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