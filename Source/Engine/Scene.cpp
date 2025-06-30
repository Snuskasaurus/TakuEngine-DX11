#include "Scene.h"

#include "Engine/Resources/AssetList.h"
#include "Engine/Graphics/Graphic.h"
#include "Engine/World.h"
#include "Engine/Graphics/Drawable.h"
#include "Engine/Resources/MeshResources.h"
#include "Engine/Debug/DebugDraw.h"
#include "Engine/Resources/DrawableResources.h"

#include "Game/Scenes/GridScene.h"
#include "Game/Scenes/GameTerrainScene.h"

#include "imgui/imgui.h"

//---------------------------------------------------------------------------------------------------------------------
void CGameScene::Create()
{
    OnCreate_Internal();
    OnEvent_Create();
}

void CGameScene::UpdateScene(const float& _dt)
{
    OnUpdate_Internal(_dt);
    OnEvent_Update(_dt);
}
//---------------------------------------------------------------------------------------------------------------------
void CGameScene::Destroy()
{
    OnEvent_Destroy();
    OnDestroy_Internal();
}
//---------------------------------------------------------------------------------------------------------------------
void CGameScene::OnCreate_Internal()
{
    SceneLight.SetPitch(12.0f);
    
    DebugCamera.InitializeCameraDebug();
    DebugCamera.SetCameraPosition(TVector3f(18.0f, 22.0f, 20.0f));
    DebugCamera.SetCameraRotation(TRotator(-0.52f, 2.44f, 0.0f));
    
    GameCamera.InitializeCameraGame();
    GameCamera.SetCameraPosition(TVector3f(0.0f, 70.0f, 20.0f));
    GameCamera.SetCameraRotation(TRotator(-0.94f, 0.0f, 0.0f));
}
//---------------------------------------------------------------------------------------------------------------------
void CGameScene::OnDestroy_Internal()
{
    for (int i = 0; i < InstancedMeshes.size(); ++i)
    {
        delete InstancedMeshes[i];
    }
    InstancedMeshes.clear();
}
//---------------------------------------------------------------------------------------------------------------------
void CGameScene::OnUpdate_Internal(const float& _dt)
{
    constexpr float lightSpeed = 0.5f;
    SceneLight.AddYaw(lightSpeed * _dt);
    
    if (DebugModeEnabled)
    {
        
        MDebugDraw::Line(TVector3f::Zero, TVector3f::Right * 100.0f, TColor::Red, 0.0f);     // +X
        MDebugDraw::Line(TVector3f::Zero, TVector3f::Up * 100.0f, TColor::Green, 0.0f);      // +Y
        MDebugDraw::Line(TVector3f::Zero, TVector3f::Forward * 100.0f, TColor::Blue, 0.0f);  // +
        
        MDebugDraw::Line(TVector3f::Zero, GetSceneLight().GetWorldLightDir() * 100.0f, TColor::Yellow, 0.0f);  // Sun
        
        DebugCamera.UpdateCamera(_dt);
    }
    else
    {
        GameCamera.UpdateCamera(_dt);
    }

}
//---------------------------------------------------------------------------------------------------------------------
void CGameScene::OnKeyPressed_Internal(EKeyCode _key)
{
}
//---------------------------------------------------------------------------------------------------------------------
void CGameScene::OnKeyReleased_Internal(EKeyCode _key)
{
    if (_key == EKeyCode::KEY_ESCAPE)
    {
        PostQuitMessage(1);
    }
    else if (_key == EKeyCode::KEY_BACKSPACE)
    {
        OnDebugModeToggle_Internal();
        OnEvent_DebugModeToggle();
    }
    else if (_key == EKeyCode::KEY_NUM_LOCK)
        MGraphic::ReportLiveObjects(true);
    else if (_key == EKeyCode::KEY_KEYPAD_8)
        SceneLight.AddPitch(1.0f);
    else if (_key == EKeyCode::KEY_KEYPAD_2)
        SceneLight.AddPitch(-1.0f);
    else if (_key == EKeyCode::KEY_KEYPAD_4)
        SceneLight.AddYaw(-1.0f);
    else if (_key == EKeyCode::KEY_KEYPAD_6)
        SceneLight.AddYaw(1.0f);
}

void CGameScene::OnDebugModeToggle_Internal()
{
    DebugModeEnabled = !DebugModeEnabled;
    if (DebugModeEnabled == true)
    {
        MInput::LockCursor();

        // Match debug camera view to the game camera view
        const TTransform transformGameCamera = GameCamera.GetCameraTransform();
        DebugCamera.SetCameraPosition(transformGameCamera.Position);
        DebugCamera.SetCameraRotation(transformGameCamera.Rotator);
    }
    else
    {
        MInput::UnlockCursor();
    }
}

//---------------------------------------------------------------------------------------------------------------------
CDrawable_InstancedMesh* CGameScene::AddInstancedMeshToDraw_DEPRECATED(const char* _meshName)
{
    CDrawable_InstancedMesh* InstancedMesh = new CDrawable_InstancedMesh;
    
    InstancedMesh->MeshData = MMeshResources::GetMeshDataFromFileName(_meshName);
    assert(InstancedMesh->MeshData != nullptr);

    InstancedMesh->ColorTexture = MTextureResources::GetTextureDataFromFileName(_meshName);
    assert(InstancedMesh->ColorTexture != nullptr);
    
    InstancedMesh->NormalTexture = MTextureResources::GetOrCreateTextureDataFromFileName(G_ASSET_T_NONE_NORMAL);
    assert(InstancedMesh->NormalTexture != nullptr);
    InstancedMesh->EmissionTexture = MTextureResources::GetOrCreateTextureDataFromFileName(G_ASSET_T_NONE_DARK);
    assert(InstancedMesh->EmissionTexture != nullptr);
    InstancedMesh->MROTexture = MTextureResources::GetOrCreateTextureDataFromFileName(G_ASSET_T_NONE_WHITE);
    assert(InstancedMesh->MROTexture != nullptr);
    
    MGraphic::CreateVertexBuffer(MGraphic::GetDXDevice(), MGraphic::GetDXDeviceContext(), &InstancedMesh->VertexBuffer, InstancedMesh->MeshData->VertexBuffer.data(), InstancedMesh->MeshData->VertexBuffer_Size, SMeshData::VertexBuffer_StructureByteStride);
    MGraphic::CreateIndexBuffer(MGraphic::GetDXDevice(), MGraphic::GetDXDeviceContext(), &InstancedMesh->IndexBuffer, InstancedMesh->MeshData->IndexBuffer.data(), InstancedMesh->MeshData->IndexBuffer_Size, SMeshData::IndexBuffer_StructureByteStride);
    
    InstancedMeshes.push_back(InstancedMesh);
    return InstancedMesh;
}
//---------------------------------------------------------------------------------------------------------------------
CDrawable_InstancedMesh* CGameScene::AddInstancedMeshToDrawFromDrawableData(const char* _meshName)
{
    CDrawable_InstancedMesh* InstancedMesh = new CDrawable_InstancedMesh;
    
    SDrawableData* DrawableData = MDrawableResources::GetDrawableData(_meshName);
    
    InstancedMesh->MeshData = DrawableData->MeshData;
    assert(InstancedMesh->MeshData != nullptr);
    
    InstancedMesh->ColorTexture = DrawableData->ColorTextureData;
    assert(InstancedMesh->ColorTexture != nullptr);
    
    InstancedMesh->NormalTexture = DrawableData->NormalTextureData;
    InstancedMesh->EmissionTexture = DrawableData->EmissionTextureData;
    InstancedMesh->MROTexture = DrawableData->MROTextureData;
    InstancedMesh->CastShadow = DrawableData->CastShadow;
    
    MGraphic::CreateVertexBuffer(MGraphic::GetDXDevice(), MGraphic::GetDXDeviceContext(), &InstancedMesh->VertexBuffer, InstancedMesh->MeshData->VertexBuffer.data(), InstancedMesh->MeshData->VertexBuffer_Size, InstancedMesh->MeshData->VertexBuffer_StructureByteStride);
    MGraphic::CreateIndexBuffer(MGraphic::GetDXDevice(), MGraphic::GetDXDeviceContext(), &InstancedMesh->IndexBuffer, InstancedMesh->MeshData->IndexBuffer.data(), InstancedMesh->MeshData->IndexBuffer_Size, InstancedMesh->MeshData->IndexBuffer_StructureByteStride);
    
    InstancedMeshes.push_back(InstancedMesh);
    return InstancedMesh;
}
//---------------------------------------------------------------------------------------------------------------------
void CGameScene::ChangeGameScene(const EGameSceneType& _gameSceneType)
{
    CGameScene* OldWorldGameScene = MWorld::GetWorld()->CurrentGameScene;
    if (OldWorldGameScene != nullptr)
    {
        OldWorldGameScene->Destroy();
        delete OldWorldGameScene;
        MWorld::GetWorld()->CurrentGameScene = nullptr;
    }

    CGameScene* NewWorldGameScene = nullptr;
    switch (_gameSceneType)
    {
        case None: break;
        case GridScene:            NewWorldGameScene = new CGridScene; break;
        case GameTerrainScene:     NewWorldGameScene = new CGameTerrainScene; break;
    }
    
    MWorld::GetWorld()->CurrentGameScene = NewWorldGameScene;
    NewWorldGameScene->Create();
}