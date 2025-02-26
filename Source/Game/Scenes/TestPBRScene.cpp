#include "TestPBRScene.h"

#include "../../Engine/Math.h"
#include "../../Engine/Resources/AssetList.h"

CDrawable_InstancedMesh* MachineMesh1;
CDrawable_InstancedMesh* MachineMesh2;
CDrawable_InstancedMesh* MachineMesh3;

CDrawable_InstancedMesh* MedicalBedMesh1;
CDrawable_InstancedMesh* MedicalBedMesh2;
CDrawable_InstancedMesh* MedicalBedMesh3;
CDrawable_InstancedMesh* MedicalBedMesh4;
CDrawable_InstancedMesh* MedicalBedMesh5;

constexpr int NbInstancedMesh = 6000;

//---------------------------------------------------------------------------------------------------------------------
void CTestPBRScene::OnCreate()
{
    SceneLight.SetYaw(135.0f);
    SceneLight.SetPitch(40.0f);
    
    MachineMesh1 = CGameScene::AddInstancedMeshToDrawFromDrawableData(G_DRAWABLE_MACHINE);
    MachineMesh1->Instances.push_back(TTransform::Identity);
    MachineMesh2 = CGameScene::AddInstancedMeshToDrawFromDrawableData(G_DRAWABLE_MACHINE_BUTTONS);
    MachineMesh2->Instances.push_back(TTransform::Identity);
    MachineMesh3 = CGameScene::AddInstancedMeshToDrawFromDrawableData(G_DRAWABLE_MACHINE_PANELS);
    MachineMesh3->Instances.push_back(TTransform::Identity);
}
//---------------------------------------------------------------------------------------------------------------------
void CTestPBRScene::OnUpdate(const float& _dt)
{
    if (pauseLight == false)
    {
        SceneLight.AddYaw(30.0f * _dt);
    }
}
//---------------------------------------------------------------------------------------------------------------------
void CTestPBRScene::OnDestroy()
{
    
}
//---------------------------------------------------------------------------------------------------------------------
void CTestPBRScene::OnKeyPressed(EKeyCode _key)
{
    if (_key == EKeyCode::KEY_P)
    {
        pauseLight = !pauseLight;
    }
}

//---------------------------------------------------------------------------------------------------------------------
