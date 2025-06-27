#include "GameTerrainScene.h"

#include "../MGridSystem.h"
#include "../../Engine/GameWindow.h"
#include "../../Engine/Resources/AssetList.h"
#include "../../Engine/World.h"
#include "../../Engine/Debug/DebugDraw.h"
#include "../../imgui/imgui.h"

//---------------------------------------------------------------------------------------------------------------------
void CGameTerrainScene::OnEvent_Create()
{
    SceneLight.SetYaw(120.0f);
    SceneLight.SetPitch(130.0f);
    
    MDebugDraw::Line(TVector3f::Zero, TVector3f::Right * 100.0f, TColor::Red);     // +X
    MDebugDraw::Line(TVector3f::Zero, TVector3f::Up * 100.0f, TColor::Green);      // +Y
    MDebugDraw::Line(TVector3f::Zero, TVector3f::Forward * 100.0f, TColor::Blue);  // +Z
    
    assert(TerrainMesh == nullptr);
    TerrainMesh = AddInstancedMeshToDrawFromDrawableData(G_ASSET_NAME_TERRAIN);
    TerrainMesh->Instances.push_back({TVector3f::Zero, 0.0f, 0.0f, 0.0f});
    
    SelectorMesh = AddInstancedMeshToDrawFromDrawableData(G_ASSET_SHAPE_TORUS);
    SelectorMesh->Instances.push_back({TVector3f::Zero, 0.0f, 0.0f, 0.0f});
}
//---------------------------------------------------------------------------------------------------------------------
void CGameTerrainScene::OnEvent_KeyReleased(EKeyCode _key)
{
    if (_key == EKeyCode::KEY_ESCAPE)
    {
        PostQuitMessage(1);
    }
    if (_key == EKeyCode::KEY_ENTER)
    {
        //HandleCursorTerrainCollision();
    }
    else if (_key == EKeyCode::KEY_KEYPAD_8)
        SceneLight.AddPitch(5.0f);
    else if (_key == EKeyCode::KEY_KEYPAD_2)
        SceneLight.AddPitch(-5.0f);
    else if (_key == EKeyCode::KEY_KEYPAD_4)
        SceneLight.AddYaw(-5.0f);
    else if (_key == EKeyCode::KEY_KEYPAD_6)
        SceneLight.AddYaw(5.0f);
    else if (_key == EKeyCode::KEY_T)
    {
        MWorld::GetWorld()->GetFreeLookCamera().SetCameraPosition(TVector3f(0.0f, -5.0f, 0.0f));
        MWorld::GetWorld()->GetFreeLookCamera().SetCameraRotation(TRotator(MMath::Deg2Rad(180.0f), 0.0f, 0.0f));
    }
}
//---------------------------------------------------------------------------------------------------------------------
void CGameTerrainScene::OnEvent_Update(const float& _dt)
{
    HandleCursorTerrainCollision();
}
//---------------------------------------------------------------------------------------------------------------------
void CGameTerrainScene::OnEvent_Destroy()
{
    
}
//---------------------------------------------------------------------------------------------------------------------
void CGameTerrainScene::HandleCursorTerrainCollision()
{
    const TVector2f mousePosition = MInput::GetMousePosition();

    const auto [Start, End] = MMath::RayTrace(mousePosition);
    //MDebugDraw::Line(Start, End, TColor::Yellow);

    const auto [Success, Intersection] = MMath::CollideRayWithMesh(Start, End, TerrainMesh->Instances[0], *TerrainMesh->MeshData);
    if (Success == true)
    {
        SelectorMesh->Instances[0].Position = Intersection + TVector3f::Up * 1.0f;
    }
    else
    {
        SelectorMesh->Instances[0].Position = TVector3f::Zero;
    }
}
//---------------------------------------------------------------------------------------------------------------------
