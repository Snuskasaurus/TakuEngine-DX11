#pragma once

#include <Windows.h>

struct SColor
{
    UINT8 r, g, b, a;
    
    static SColor Red;
    static SColor Green;
    static SColor Blue;
};
