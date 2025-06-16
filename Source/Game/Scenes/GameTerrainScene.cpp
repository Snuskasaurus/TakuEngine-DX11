#include "GameTerrainScene.h"

#include "../MGridSystem.h"
#include "../../Engine/Resources/AssetList.h"
#include "../../Engine/World.h"
#include "../../Engine/Debug/DebugDraw.h"

//---------------------------------------------------------------------------------------------------------------------
void CGameTerrainScene::OnCreate()
{
    SceneLight.SetYaw(120.0f);
    SceneLight.SetPitch(130.0f);

    auto terrain = AddInstancedMeshToDrawFromDrawableData(G_ASSET_NAME_TERRAIN);
    terrain->Instances.push_back({TVector3f::Zero, 0.0f, 0.0f, 0.0f});
}
//---------------------------------------------------------------------------------------------------------------------
void CGameTerrainScene::OnKeyPressed(EKeyCode _key)
{
    if (_key == EKeyCode::KEY_ESCAPE)
    {
        PostQuitMessage(1);
    }
    else if (_key == EKeyCode::KEY_KEYPAD_8)
        SceneLight.AddPitch(5.0f);
    else if (_key == EKeyCode::KEY_KEYPAD_2)
        SceneLight.AddPitch(-5.0f);
    else if (_key == EKeyCode::KEY_KEYPAD_4)
        SceneLight.AddYaw(-5.0f);
    else if (_key == EKeyCode::KEY_KEYPAD_6)
        SceneLight.AddYaw(5.0f);
}
//---------------------------------------------------------------------------------------------------------------------
void CGameTerrainScene::OnUpdate(const float& _dt)
{
}
//---------------------------------------------------------------------------------------------------------------------
void CGameTerrainScene::OnDestroy()
{
    
}
//---------------------------------------------------------------------------------------------------------------------
