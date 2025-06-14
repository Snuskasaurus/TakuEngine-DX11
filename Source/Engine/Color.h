#pragma once

#include <array>
#include <Windows.h>

struct TColorf
{
    float r, g, b, a;
    
    static TColorf White;
    static TColorf Black;
    static TColorf Red;
    static TColorf Green;
    static TColorf Blue;
    static TColorf Magenta;
    static TColorf Yellow;
    static TColorf Cyan;
};

struct TColor
{
    UINT8 r, g, b, a;
    
    static TColor White;
    static TColor Black;
    static TColor Red;
    static TColor Green;
    static TColor Blue;
    static TColor Magenta;
    static TColor Yellow;
    static TColor Cyan;

    TColorf ToFloat() const { return {(float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f}; }
};
