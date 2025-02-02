#include "TimeManager.h"

#include <Windows.h>

///---------------------------------------------------------------------------------------------------------------------
LONGLONG CounterStart;
LONGLONG CounterLastFrame;
float CountsPerSecond;
///---------------------------------------------------------------------------------------------------------------------
void MTime::InitializeTime()
{
    LARGE_INTEGER QueryHolder;
    QueryPerformanceFrequency(&QueryHolder);

    CountsPerSecond = (float)QueryHolder.QuadPart;

    QueryPerformanceCounter(&QueryHolder);
    CounterStart = QueryHolder.QuadPart;
    CounterLastFrame = CounterStart;
}
///---------------------------------------------------------------------------------------------------------------------
float MTime::GetDeltaTime()
{
    LARGE_INTEGER QueryHolder;
    QueryPerformanceCounter(&QueryHolder);

    LONGLONG CounterSinceLastFrame = QueryHolder.QuadPart - CounterLastFrame;
    CounterLastFrame = QueryHolder.QuadPart;
    
    return (float)(CounterSinceLastFrame) / CountsPerSecond;
}
///---------------------------------------------------------------------------------------------------------------------
float MTime::GetTime()
{
    LARGE_INTEGER currentTime;
    QueryPerformanceCounter(&currentTime);
    return (float)(currentTime.QuadPart - CounterStart) / CountsPerSecond;
}
///---------------------------------------------------------------------------------------------------------------------
