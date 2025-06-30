#include "DebugDraw.h"

static std::vector<SDrawDebugHolder> G_DEBUG_LINES;

void MDebugDraw::UpdateDebugDraws(const float _dt)
{
    for (int i = (int)G_DEBUG_LINES.size() - 1; i > 0 ; --i)
    {
        G_DEBUG_LINES[i].TimeRemaining -= _dt;
        if (G_DEBUG_LINES[i].TimeRemaining <= 0.0f)
        {
            G_DEBUG_LINES.erase(G_DEBUG_LINES.begin() + i);
        }
    }
}

void MDebugDraw::Prepare()
{
    G_DEBUG_LINES.reserve(1500);
}

void MDebugDraw::Line(TVector3f _positionStart, TVector3f _positionEnd, TColor _color, const float _time)
{
    SDrawDebugHolder NewDebug;
    NewDebug.DebugLine.Transforms[0] = { _positionStart };
    NewDebug.DebugLine.Transforms[1] = { _positionEnd };
    NewDebug.DebugLine.Color = _color.ToFloat();
    NewDebug.TimeRemaining = _time;
    G_DEBUG_LINES.push_back(NewDebug);
}

std::vector<SDrawDebugHolder>& MDebugDraw::GetDebugLines()
{
    return G_DEBUG_LINES;
}
