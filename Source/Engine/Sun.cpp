#include "Sun.h"

#include "Resources/AssetList.h"
#include "Scene.h"
#include "World.h"
#include "Graphics/Drawable.h"
#include "GameWindow.h"


 SLightInfo CSceneLight::GetSceneLightInfo() const
{
     SLightInfo lightInfo;
     {
        lightInfo.AmbientIntensity = Ambient;
        lightInfo.ColorIntensity = Intensity;
        lightInfo.Direction = -Forward;
        lightInfo.Color = DiffuseColor;
     }
     return lightInfo;
}
void CSceneLight::SetYaw(float _yaw)
{
     CurrentYaw = _yaw;
     OnValueChanged();
}

void CSceneLight::SetPitch(float _pitch)
{
     CurrentPitch = MMath::Clamp(_pitch, 10.0f, 170.0f);
     OnValueChanged();
}

TMatrix4f CSceneLight::GetProjectionMatrix() const
{
     //return MGameWindow::GetPerspectiveMatrix();
      
     const TMatrix4f PerspectiveMatrix = TMatrix4f::MatrixOrthographic(120, 120, 0.1f, 300.0f);
     return PerspectiveMatrix;
}
void CSceneLight::OnValueChanged()
{
      const TMatrix4f LightRotationMatrix = TMatrix4f::MatrixRotationPitch(MMath::Deg2Rad(-CurrentPitch)) * TMatrix4f::MatrixRotationYaw(MMath::Deg2Rad(-CurrentYaw)) * TMatrix4f::MatrixRotationRoll(0.0f);

      Forward = TVector3f::TransformCoord(TVector3f::Forward, LightRotationMatrix);
      Forward = TVector3f::Normalize(Forward);
 
      TVector3f Right = TVector3f::TransformCoord(TVector3f::Right, LightRotationMatrix);
      Right = TVector3f::Normalize(Right);
 
      TVector3f up = TVector3f::Cross(Forward, Right);
      up = TVector3f::Normalize(up);

      TVector3f Position = TVector3f::Zero + Forward * -50.0f;

      const TMatrix4f LightPositionMatrix = TMatrix4f::MatrixTranslation(Position);
      LightMatrix = LightRotationMatrix * LightPositionMatrix;
}
