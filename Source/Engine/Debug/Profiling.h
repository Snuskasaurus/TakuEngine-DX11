#pragma once

#include <intsafe.h>

#ifndef __FUNCTION_NAME__
    #ifdef WIN32   //WINDOWS
        #define __FUNCTION_NAME__   __FUNCTION__  
    #else          //*NIX
        #define __FUNCTION_NAME__   __func__ 
    #endif
#endif

#define PROFILING_ENABLED 1

#if PROFILING_ENABLED
    #define PROFILE_FUNCTION CDebugProfiler profiler = CDebugProfiler(__FUNCTION_NAME__);
#else
    #define PROFILE_FUNCTION
#endif

class CDebugProfiler
{
public:
    CDebugProfiler() = delete;
    CDebugProfiler(const char*);
    ~CDebugProfiler();
private:
    LONGLONG CounterStart;
    float CountsPerSecond;
};
