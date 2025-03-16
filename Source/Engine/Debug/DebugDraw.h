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

class MDebugDraw
{
    friend class MGraphic;
public:
    static void Line(TVector3f _positionStart, TVector3f _positionEnd, TColor _color);
private:
    static std::vector<SDebugLine>& GetDebugLines();
};
