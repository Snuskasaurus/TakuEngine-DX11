#include "Sun.h"

#include "Resources/AssetList.h"
#include "Scene.h"
#include "World.h"
#include "Graphics/Drawable.h"


 SLightInfo CSceneLight::GetSceneLightInfo() const
{
    const TMatrix4f CamRotationMatrix = TMatrix4f::MatrixRotationPitch(MMath::Deg2Rad(CurrentPitch))
                                      * TMatrix4f::MatrixRotationYaw(MMath::Deg2Rad(CurrentYaw))
                                      * TMatrix4f::MatrixRotationRoll(0.0f);
    
    TVector3f lightDirection = TVector3f::TransformCoord(TVector3f::Forward, CamRotationMatrix);
    lightDirection = TVector3f::Normalize(lightDirection);

    return { lightDirection, DiffuseColor, Ambient };
}
void CSceneLight::SetYaw(float _yaw)
{
    CurrentYaw = _yaw;
    UpdateOnChanges();
}

void CSceneLight::SetPitch(float _pitch)
 {
     CurrentPitch = MMath::Clamp(_pitch, 10.0f, 170.0f);
     UpdateOnChanges();
}

void CSceneLight::Initialize()
{
    DebugArrowMesh = MWorld::GetWorld()->GetCurrentScene()->AddInstancedMeshToDrawFromDrawableData(G_ASSET_ARROW);
    DebugArrowMesh->Instances.push_back(TTransform({0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}));
    UpdateOnChanges();
}

void CSceneLight::Uninitialize()
{
}

void CSceneLight::UpdateOnChanges()
{
    DebugArrowMesh->Instances[0] = {{0.0f, 0.0f, 0.0f}, { MMath::Deg2Rad(CurrentYaw), MMath::Deg2Rad(CurrentPitch), 0.0f }};
}
