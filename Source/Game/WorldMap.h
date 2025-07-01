#pragma once
#include <vector>

#include "Engine/Math.h"

struct FInterestPoint
{
    TVector3f Position;
};

class FWorldMap
{
public:
    void UpdateWorldMap(const float _dt, const TVector3f& _worldPosition);
    void CreateInterestPoint(TVector3f _worldPosition);
    FInterestPoint* FindInterestPoint(TVector3f _worldPosition);

private:
    std::vector<FInterestPoint> InterestPoints;
};

