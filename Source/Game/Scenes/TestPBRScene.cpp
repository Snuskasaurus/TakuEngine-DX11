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
    
    // MedicalBedMesh1 = CGameScene::AddInstancedMeshToDrawFromDrawableData(G_DRAWABLE_MEDICAL_BED);
    // MedicalBedMesh1->Instances.push_back(TTransform(TVector3f::Right * 60.0f, TRotator(-5.0f, 0.0f, 0.0f)));
    // MedicalBedMesh2 = CGameScene::AddInstancedMeshToDrawFromDrawableData(G_DRAWABLE_MEDICAL_BED_CABLES);
    // MedicalBedMesh2->Instances.push_back(TTransform(TVector3f::Right * 60.0f, TRotator(-5.0f, 0.0f, 0.0f)));
    // MedicalBedMesh3 = CGameScene::AddInstancedMeshToDrawFromDrawableData(G_DRAWABLE_MEDICAL_BED_DECALS);
    // MedicalBedMesh3->Instances.push_back(TTransform(TVector3f::Right * 60.0f, TRotator(-5.0f, 0.0f, 0.0f)));
    // MedicalBedMesh4 = CGameScene::AddInstancedMeshToDrawFromDrawableData(G_DRAWABLE_MEDICAL_BED_SUPPORT);
    // MedicalBedMesh4->Instances.push_back(TTransform(TVector3f::Right * 60.0f, TRotator(-5.0f, 0.0f, 0.0f)));
    // MedicalBedMesh5 = CGameScene::AddInstancedMeshToDrawFromDrawableData(G_DRAWABLE_MEDICAL_BED_MACHINE);
    // MedicalBedMesh5->Instances.push_back(TTransform(TVector3f::Right * 60.0f, TRotator(-5.0f, 0.0f, 0.0f)));
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
