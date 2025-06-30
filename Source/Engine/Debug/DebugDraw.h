#pragma once
#include <DirectXMath.h>
#include <vector>

#include "../Color.h"
#include "../Math.h"


struct SDebugLine
{
    TTransform Transforms[2];
    TColorf Color;
};
struct SDrawDebugHolder
{
    SDebugLine DebugLine;
    float TimeRemaining = 0.0f;
};

class MDebugDraw
{
    friend class MGraphic;
public:
    static void UpdateDebugDraws(const float _dt);
    static void Prepare();
    static void Line(TVector3f _positionStart, TVector3f _positionEnd, TColor _color, const float _time);
    static std::vector<SDrawDebugHolder>& GetDebugLines();
};
