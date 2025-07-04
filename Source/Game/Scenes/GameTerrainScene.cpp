#include "GameTerrainScene.h"

#include "../../Engine/GameWindow.h"
#include "../../Engine/Resources/AssetList.h"
#include "../../Engine/World.h"
#include "../../Engine/Debug/DebugDraw.h"
#include "../../imgui/imgui.h"

//---------------------------------------------------------------------------------------------------------------------
void CGameTerrainScene::OnEvent_Create()
{
    assert(TerrainMesh == nullptr);
    //TerrainMesh = AddInstancedMeshToDrawFromDrawableData(G_ASSET_NAME_TERRAIN);
    //TerrainMesh->Instances.push_back({TVector3f::Zero, 0.0f, 0.0f, 0.0f});
    
    //SelectorMesh = AddInstancedMeshToDrawFromDrawableData(G_ASSET_SHAPE_TORUS);
    //SelectorMesh->Instances.push_back({TVector3f::Zero, 0.0f, 0.0f, 0.0f});

    WorldMap.CreateWorldMap();
}
//---------------------------------------------------------------------------------------------------------------------
void CGameTerrainScene::OnEvent_KeyReleased(EKeyCode _key)
{
    if (_key == EKeyCode::KEY_ESCAPE)
    {
        PostQuitMessage(1);
    }
    else if (_key == EKeyCode::KEY_ENTER)
    {
        WorldMap.CreateInterestPoint(CursorPosition);
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
    //HandleCursorTerrainCollision();
    WorldMap.UpdateWorldMap(_dt, CursorPosition);
}
//---------------------------------------------------------------------------------------------------------------------
void CGameTerrainScene::OnEvent_Destroy()
{
    
}
//---------------------------------------------------------------------------------------------------------------------
void CGameTerrainScene::HandleCursorTerrainCollision()
{
    const TVector2f mousePosition = MInput::GetMousePosition();
    const RayTraceResult rayResult = MMath::RayTrace(mousePosition);
    const CollisionMeshResult collisionResult = MMath::CollideRayWithMesh(rayResult.Start, rayResult.End, TerrainMesh->Instances[0], *TerrainMesh->MeshData);
    SelectorMesh->Instances[0].Position = TVector3f::Zero;
    if (collisionResult.Success == true)
    {
        SelectorMesh->Instances[0].Position = collisionResult.Intersection;
        SelectorMesh->Instances[0].Rotator = collisionResult.Rotation;
        CursorPosition = collisionResult.Intersection;
        MDebugDraw::Line(CursorPosition, CursorPosition + collisionResult.Normal * 10.0f, TColor::White, 0.0f);
    }
}
//---------------------------------------------------------------------------------------------------------------------
