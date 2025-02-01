#include "TimeManager.h"

#include <Windows.h>
#include <assert.h>

LONGLONG CounterStart;
LONGLONG CounterLastFrame;

float CountsPerSecond;

int frameCount = 0;
int fps = 0;

float frameTime;

///---------------------------------------------------------------------------------------------------------------------
void JuProject::InitializeTime()
{
    LARGE_INTEGER QueryHolder;
    QueryPerformanceFrequency(&QueryHolder);

    CountsPerSecond = (float)QueryHolder.QuadPart;

    QueryPerformanceCounter(&QueryHolder);
    CounterStart = QueryHolder.QuadPart;
    CounterLastFrame = CounterStart;
}
///---------------------------------------------------------------------------------------------------------------------
float JuProject::GetDeltaTime()
{
    LARGE_INTEGER QueryHolder;
    QueryPerformanceCounter(&QueryHolder);

    LONGLONG CounterSinceLastFrame = QueryHolder.QuadPart - CounterLastFrame;
    CounterLastFrame = QueryHolder.QuadPart;

    assert(CounterSinceLastFrame >= 0);

    return (float)(CounterSinceLastFrame) / CountsPerSecond;
}
///---------------------------------------------------------------------------------------------------------------------
float JuProject::GetTime()
{
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);
    return (float)(currentTime.QuadPart-CounterStart)/CountsPerSecond;
}
///---------------------------------------------------------------------------------------------------------------------