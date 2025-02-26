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

     SLightInfo lightInfo;
     {
        lightInfo.Ambient = Ambient;
        lightInfo.Intensity = Intensity;
        lightInfo.Direction = lightDirection;
        lightInfo.Diffuse = DiffuseColor;
     }
     return lightInfo;
}
void CSceneLight::SetYaw(float _yaw)
{
    CurrentYaw = _yaw;
}

void CSceneLight::SetPitch(float _pitch)
 {
     CurrentPitch = MMath::Clamp(_pitch, 10.0f, 170.0f);
}

void CSceneLight::Initialize()
{
}

void CSceneLight::Uninitialize()
{
}
