#include "TakumiScene.h"

#include "../../Engine/Math.h"
#include "../../Engine/AssetList.h"

constexpr int NbInstancedMesh = 6000;

//---------------------------------------------------------------------------------------------------------------------
void CTakumiScene::OnCreate()
{
    InstancedMesh = CTakumiScene::AddInstancedMeshToDraw({}, JU_ASSET_TAKUMI);
    const TVector3f MinPosition = TVector3f(-100.0f, -100.00f, -100.0f);
    const TVector3f MaxPosition = TVector3f(100.0f, 100.00f, 100.0f);
    for (int i = 1; i < NbInstancedMesh; ++i)
    {
        const float Yaw = MMath::RandomNumberIntegerInRange(-10.0f, 10.0f);
        TTransform NewTransform = { MMath::RandomVectorIntegerInRange(MinPosition, MaxPosition), Yaw, 0.0f, 0.0f };
        InstancedMesh->Instances.push_back(NewTransform);
    }
}
//---------------------------------------------------------------------------------------------------------------------
void CTakumiScene::OnUpdate(const float& _dt)
{
    for (int i = 0; i < NbInstancedMesh; ++i)
    {
        InstancedMesh->Instances[i].Rotator.Yaw += (i % 8 + 1) * _dt;
        InstancedMesh->Instances[i].Rotator.Pitch += (i % 8 + 1) * _dt;
        InstancedMesh->Instances[i].Rotator.Roll += (i % 8 + 1) * _dt;
    }
}
//---------------------------------------------------------------------------------------------------------------------
void CTakumiScene::OnDestroy()
{
    
}

void CTakumiScene::OnKeyPressed(EKeyCode _key)
{
    if (_key == EKeyCode::KEY_ESCAPE)
    {
        PostQuitMessage(1);
    }
    else if (_key == EKeyCode::KEY_BACKSPACE)
    {
        ChangeGameScene(EGameSceneType::GridScene);
    }
}

//---------------------------------------------------------------------------------------------------------------------
