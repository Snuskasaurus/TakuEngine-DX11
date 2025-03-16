#include "DebugDraw.h"


static std::vector<SDebugLine> G_DEBUG_LINES;

void MDebugDraw::Prepare()
{
    G_DEBUG_LINES.reserve(1500);
}

void MDebugDraw::Line(TVector3f _positionStart, TVector3f _positionEnd, TColor _color)
{
    SDebugLine newLine;
    newLine.Transforms[0] = {_positionStart };
    newLine.Transforms[1] = {_positionEnd };
    G_DEBUG_LINES.push_back(newLine);
}

std::vector<SDebugLine>& MDebugDraw::GetDebugLines()
{
    return G_DEBUG_LINES;
}
