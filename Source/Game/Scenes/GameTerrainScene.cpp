#include "GameTerrainScene.h"

#include "../MGridSystem.h"
#include "../../Engine/Resources/AssetList.h"
#include "../../Engine/World.h"
#include "../../Engine/Debug/DebugDraw.h"
#include "../../imgui/imgui.h"

//---------------------------------------------------------------------------------------------------------------------
void CGameTerrainScene::OnEvent_Create()
{
    SceneLight.SetYaw(120.0f);
    SceneLight.SetPitch(130.0f);

    auto terrain = AddInstancedMeshToDrawFromDrawableData(G_ASSET_NAME_TERRAIN);
    terrain->Instances.push_back({TVector3f::Zero, 0.0f, 0.0f, 0.0f});
}
//---------------------------------------------------------------------------------------------------------------------
void DrawDebugMouseRay()
{
    const TVector2f MousePosition = MInput::GetMousePosition();
    
    TVector3f nearPlane = TVector3f(MousePosition.x, MousePosition.y,0.0f);
    TVector3f farPlane = TVector3f(MousePosition.x, MousePosition.y,1.0f);
    
    //rayorigin = transformcoord(near, inverseviewprojection);
    //rayend = transformcoord(far, inverseviewprojection);
    //raydir = normalize(rayend-rayorigin);
    //MDebugDraw::Line(TVector3f::Zero, TVector3f::Zero + TVector3f::Up * 1000.0f, TColor::Red);
}
//---------------------------------------------------------------------------------------------------------------------
void CGameTerrainScene::OnEvent_KeyPressed(EKeyCode _key)
{
    if (_key == EKeyCode::KEY_ESCAPE)
    {
        PostQuitMessage(1);
    }
    if (_key == EKeyCode::KEY_ENTER)
    {
        DrawDebugMouseRay();
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
void CGameTerrainScene::OnEvent_Update(const float& _dt)
{
}
//---------------------------------------------------------------------------------------------------------------------
void CGameTerrainScene::OnEvent_Destroy()
{
    
}
//---------------------------------------------------------------------------------------------------------------------
