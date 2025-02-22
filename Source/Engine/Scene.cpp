#include "Scene.h"

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
void CGameScene::Destroy()
{
    for (int i = 0; i < InstancedMeshes.size(); ++i)
    {
        delete InstancedMeshes[i];
    }
    InstancedMeshes.clear();
    
    OnDestroy();
}
//---------------------------------------------------------------------------------------------------------------------
CDrawable_InstancedMesh* CGameScene::AddInstancedMeshToDraw(const char* _meshName)
{
    CDrawable_InstancedMesh* InstancedMesh = new CDrawable_InstancedMesh;

    // Load the mesh data from file
    {
        InstancedMesh->MeshData = MMeshResources::GetMeshDataFromFileName(_meshName);
        assert(InstancedMesh->MeshData != nullptr);
    }

    // TODO Julien Rogel (14/02/2025): Rework texture loading
    {
        std::wstringstream TextureFilenameStream;
        TextureFilenameStream << GAME_DATA_PATH << _meshName << ".bmp";
        MGraphic::FillGraphicResources_Instanced(InstancedMesh, TextureFilenameStream.str().c_str());
    }
    
    InstancedMeshes.push_back(InstancedMesh);
    return InstancedMesh;
}
//---------------------------------------------------------------------------------------------------------------------
