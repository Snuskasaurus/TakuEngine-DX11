#include "Scene.h"

#include "Resources/AssetList.h"
#include "Graphics/Graphic.h"
#include "World.h"
#include "Graphics/Drawable.h"
#include "Resources/MeshResources.h"

#include "Resources/DrawableResources.h"
#include "Resources/GamePath.h"

#include "../Game/Scenes/GridScene.h"
#include "../Game/Scenes/TakumiScene.h"
#include "../Game/Scenes/TestPBRScene.h"

bool G_DEBUG_ENABLED = false;
CDrawable_InstancedMesh* G_DEBUG_GIZMO_MESH = nullptr;
CDrawable_InstancedMesh* G_DEBUG_SCENE_LIGHT_MESH = nullptr;

//---------------------------------------------------------------------------------------------------------------------
void ToggleDebugs()
{
    G_DEBUG_ENABLED = !G_DEBUG_ENABLED;
    if (G_DEBUG_ENABLED == true)
    {
        if (G_DEBUG_GIZMO_MESH == nullptr)
            G_DEBUG_GIZMO_MESH = MWorld::GetWorld()->GetCurrentScene()->AddInstancedMeshToDrawFromDrawableData(G_ASSET_GIZMO);
        G_DEBUG_GIZMO_MESH->Instances.push_back(TTransform::Identity);
        
        if (G_DEBUG_SCENE_LIGHT_MESH == nullptr)
            G_DEBUG_SCENE_LIGHT_MESH = MWorld::GetWorld()->GetCurrentScene()->AddInstancedMeshToDrawFromDrawableData(G_ASSET_ARROW);
        G_DEBUG_SCENE_LIGHT_MESH->Instances.push_back(TTransform::Identity);
    }
    else
    {
        G_DEBUG_GIZMO_MESH->Instances.clear();
        G_DEBUG_SCENE_LIGHT_MESH->Instances.clear();
    }
}
//---------------------------------------------------------------------------------------------------------------------
void CGameScene::Create()
{
    OnCreate_Internal();
    OnCreate();
}

void CGameScene::Update(const float& _dt)
{
    OnUpdate_Internal(_dt);
    OnUpdate(_dt);
}

//---------------------------------------------------------------------------------------------------------------------
void CGameScene::OnCreate_Internal()
{
    SceneLight.Initialize();
}
//---------------------------------------------------------------------------------------------------------------------
void CGameScene::Destroy()
{
    OnDestroy();
    OnDestroy_Internal();
}

void CGameScene::NotifyKeyPressed(EKeyCode _keyCode)
{
    OnKeyPressed_Internal(_keyCode);
    OnKeyPressed(_keyCode);
}

//---------------------------------------------------------------------------------------------------------------------
void CGameScene::OnDestroy_Internal()
{
    SceneLight.Uninitialize();
    
    for (int i = 0; i < InstancedMeshes.size(); ++i)
    {
        delete InstancedMeshes[i];
    }
    InstancedMeshes.clear();
}
//---------------------------------------------------------------------------------------------------------------------
void CGameScene::OnUpdate_Internal(const float& _dt)
{
    if (G_DEBUG_ENABLED)
    {
        G_DEBUG_SCENE_LIGHT_MESH->Instances[0] = {{0.0f, 0.0f, 0.0f}, { MMath::Deg2Rad(SceneLight.CurrentYaw), MMath::Deg2Rad(SceneLight.CurrentPitch), 0.0f }};
    }
}
//---------------------------------------------------------------------------------------------------------------------
void CGameScene::OnKeyPressed_Internal(EKeyCode _key)
{
    if (_key == EKeyCode::KEY_ESCAPE)
    {
        PostQuitMessage(1);
    }
    else if (_key == EKeyCode::KEY_BACKSPACE)
    {
        ToggleDebugs();
    }
    else if (_key == EKeyCode::KEY_KEYPAD_8)
        SceneLight.AddPitch(10.0f);
    else if (_key == EKeyCode::KEY_KEYPAD_2)
        SceneLight.AddPitch(-10.0f);
    else if (_key == EKeyCode::KEY_KEYPAD_4)
        SceneLight.AddYaw(-10.0f);
    else if (_key == EKeyCode::KEY_KEYPAD_6)
        SceneLight.AddYaw(10.0f);
}
//---------------------------------------------------------------------------------------------------------------------
CDrawable_InstancedMesh* CGameScene::AddInstancedMeshToDraw(const char* _meshName)
{
    CDrawable_InstancedMesh* InstancedMesh = new CDrawable_InstancedMesh;
    
    InstancedMesh->MeshData = MMeshResources::GetMeshDataFromFileName(_meshName);
    assert(InstancedMesh->MeshData != nullptr);

    InstancedMesh->ColorTexture = MTextureResources::GetTextureDataFromFileName(_meshName);
    assert(InstancedMesh->ColorTexture != nullptr);
    
    MGraphic::CreateVertexBuffer(MGraphic::GetDXDevice(), MGraphic::GetDXDeviceContext(), &InstancedMesh->VertexBuffer, *InstancedMesh->MeshData);
    MGraphic::CreateIndexBuffer(MGraphic::GetDXDevice(), MGraphic::GetDXDeviceContext(), &InstancedMesh->IndexBuffer, *InstancedMesh->MeshData);
    MGraphic::CreateVertexShaderBuffer(MGraphic::GetDXDevice(), MGraphic::GetDXDeviceContext(), &InstancedMesh->VSConstantBuffer_InstancedObject, sizeof(SVSConstantBuffer_InstanceObject));  
    
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
    
    MGraphic::CreateVertexBuffer(MGraphic::GetDXDevice(), MGraphic::GetDXDeviceContext(), &InstancedMesh->VertexBuffer, *InstancedMesh->MeshData);
    MGraphic::CreateIndexBuffer(MGraphic::GetDXDevice(), MGraphic::GetDXDeviceContext(), &InstancedMesh->IndexBuffer, *InstancedMesh->MeshData);
    MGraphic::CreateVertexShaderBuffer(MGraphic::GetDXDevice(), MGraphic::GetDXDeviceContext(), &InstancedMesh->VSConstantBuffer_InstancedObject, sizeof(SVSConstantBuffer_InstanceObject));  
    
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
    case TakumiScene: NewWorldGameScene = new CTakumiScene; break;
    case GridScene: NewWorldGameScene = new CGridScene; break;
    case TestPBRScene: NewWorldGameScene = new CTestPBRScene; break;
    }
    
    MWorld::GetWorld()->CurrentGameScene = NewWorldGameScene;
    NewWorldGameScene->Create();
}