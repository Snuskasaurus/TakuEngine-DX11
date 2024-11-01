#pragma once

#include <Windows.h>

struct SColorFloat
{
    float r, g, b, a;
};

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

    SColorFloat ToFloat() const { return {(float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f}; }
};
