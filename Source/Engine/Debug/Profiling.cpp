#include "Profiling.h"

#include <Windows.h>
#include <string>

static char BufferFuncName[1000] = "";

CDebugProfiler::CDebugProfiler(const char* FuncName)
{
    int result = strcpy_s(BufferFuncName, sizeof(BufferFuncName), FuncName);

    LARGE_INTEGER QueryHolder;
    
    QueryPerformanceFrequency(&QueryHolder);
    CountsPerSecond = (float)QueryHolder.QuadPart;

    QueryPerformanceCounter(&QueryHolder);
    CounterStart = QueryHolder.QuadPart;
}

CDebugProfiler::~CDebugProfiler()
{
    LARGE_INTEGER QueryHolder;
    QueryPerformanceCounter(&QueryHolder);
    LONGLONG CounterNow = QueryHolder.QuadPart;
    const float TimeSpent = (float)(CounterNow - CounterStart) / CountsPerSecond * 1000.0f;
    
    OutputDebugStringA((std::string(BufferFuncName) + std::string(" took ") + std::to_string(TimeSpent) + std::string(" milliseconds.\n")).c_str());
}
