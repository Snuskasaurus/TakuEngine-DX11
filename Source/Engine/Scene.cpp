#include "Scene.h"

#include "Resources/AssetList.h"
#include "Graphics/Graphic.h"
#include "World.h"
#include "Graphics/Drawable.h"
#include "Resources/MeshResources.h"

#include "../Game/Scenes/GridScene.h"
#include "../Game/Scenes/TakumiScene.h"
#include "Resources/GamePath.h"

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
    }
    
    MWorld::GetWorld()->CurrentGameScene = NewWorldGameScene;
    NewWorldGameScene->Create();
}
//---------------------------------------------------------------------------------------------------------------------
void CGameScene::Create()
{
    SceneLight.Initialize();

    CDrawable_InstancedMesh* Gizmo = AddInstancedMeshToDraw(TAKU_ASSET_MESH_DEBUG_GIZMO);
    Gizmo->Instances.push_back(TTransform::Identity);
    
    OnCreate();
}
//---------------------------------------------------------------------------------------------------------------------
void CGameScene::Destroy()
{
    OnDestroy();
    
    SceneLight.Uninitialize();
    
    for (int i = 0; i < InstancedMeshes.size(); ++i)
    {
        delete InstancedMeshes[i];
    }
    InstancedMeshes.clear();
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
