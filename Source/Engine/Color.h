#pragma once

#include <array>
#include <Windows.h>

struct __declspec(align(16)) TColorf
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

struct TColorI
{
    UINT8 r, g, b, a;
    
    static TColorI White;
    static TColorI Black;
    static TColorI Red;
    static TColorI Green;
    static TColorI Blue;
    static TColorI Magenta;
    static TColorI Yellow;
    static TColorI Cyan;

    TColorf ToFloat() const { return {(float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f}; }
};
