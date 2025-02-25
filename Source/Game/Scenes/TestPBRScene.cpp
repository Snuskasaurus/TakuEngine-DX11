#include "TestPBRScene.h"

#include "../../Engine/Math.h"
#include "../../Engine/Resources/AssetList.h"

CDrawable_InstancedMesh* Mesh1;
CDrawable_InstancedMesh* Mesh2;
CDrawable_InstancedMesh* Mesh3;

constexpr int NbInstancedMesh = 6000;

//---------------------------------------------------------------------------------------------------------------------
void CTestPBRScene::OnCreate()
{
    SceneLight.SetYaw(135.0f);
    SceneLight.SetPitch(40.0f);
    
    Mesh1 = CGameScene::AddInstancedMeshToDrawFromDrawableData(G_DRAWABLE_MACHINE);
    Mesh1->Instances.push_back(TTransform::Identity);
    Mesh2 = CGameScene::AddInstancedMeshToDrawFromDrawableData(G_DRAWABLE_MACHINE_BUTTONS);
    Mesh2->Instances.push_back(TTransform::Identity);
    Mesh3 = CGameScene::AddInstancedMeshToDrawFromDrawableData(G_DRAWABLE_MACHINE_PANELS);
    Mesh3->Instances.push_back(TTransform::Identity);
}
//---------------------------------------------------------------------------------------------------------------------
void CTestPBRScene::OnUpdate(const float& _dt)
{
    //SceneLight.AddYaw(15.0f * _dt);
}
//---------------------------------------------------------------------------------------------------------------------
void CTestPBRScene::OnDestroy()
{
    
}
//---------------------------------------------------------------------------------------------------------------------
