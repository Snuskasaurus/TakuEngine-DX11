#include "InterestPoint.h"

#include "Engine/World.h"
#include "Engine/Debug/DebugDraw.h"

void FWorldMap::CreateWorldMap()
{
    const float distanceTile = 50.0f;
    
    for (int i = 0; i < 130; ++i)
    {
        const int row = i / 13;
        const int col = i % 13;
        const TVector3f TilePosition = { col * -distanceTile, 0.0f, row * -distanceTile };
        
        const std::string nameAsset = "WorldTile" + std::to_string(i);
        auto drawableInstancedMesh = MWorld::GetWorld()->GetCurrentScene()->AddInstancedMeshToDrawFromDrawableData(nameAsset.c_str());
        drawableInstancedMesh->Instances.push_back({TilePosition, TRotator()});
    }

    auto drawableInstancedMesh = MWorld::GetWorld()->GetCurrentScene()->AddInstancedMeshToDrawFromDrawableData("Table");
    drawableInstancedMesh->Instances.push_back({TVector3f::Zero, TRotator()});
    
}

void FWorldMap::UpdateWorldMap(const float _dt, const TVector3f& _worldPosition)
{
    for (int i = 0; i < InterestPoints.size(); ++i)
    {
        FInterestPoint& interestPoint = InterestPoints[i];
        
        if (TVector3f::SquareDistance(interestPoint.Position, _worldPosition) <= 20.0f)
        {
            MDebugDraw::Line(interestPoint.Position, interestPoint.Position + TVector3f::Up * 20.0f, TColor::Green, 0.0f);
        }
        else
        {
            MDebugDraw::Line(interestPoint.Position, interestPoint.Position + TVector3f::Up * 20.0f, TColor::White, 0.0f);
        }
    }
}

FInterestPoint* FWorldMap::FindInterestPoint(TVector3f _worldPosition)
{
    for (int i = 0; i < InterestPoints.size(); ++i)
    {
        if (TVector3f::SquareDistance(InterestPoints[i].Position, _worldPosition) >= 20.0f)
        {
            return &InterestPoints[i];
        }
    }
    return nullptr;
}

void FWorldMap::CreateInterestPoint(TVector3f _worldPosition)
{
    FInterestPoint newInterestPoint;
    newInterestPoint.Position = _worldPosition;
    
    InterestPoints.push_back(newInterestPoint);
}
