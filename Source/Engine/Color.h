#pragma once

#include <Windows.h>

struct SColor
{
    UINT8 r, g, b, a;
    
    static SColor White;
    static SColor Black;
    static SColor Red;
    static SColor Green;
    static SColor Blue;
    static SColor Magenta;
    static SColor Yellow;
    static SColor Cyan;
};
