#pragma once

#include <array>
#include <Windows.h>

struct TColorF
{
    float r, g, b, a;
    
    static TColorF White;
    static TColorF Black;
    static TColorF Red;
    static TColorF Green;
    static TColorF Blue;
    static TColorF Magenta;
    static TColorF Yellow;
    static TColorF Cyan;
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

    TColorF ToFloat() const { return {(float)r / 255.0f, (float)g / 255.0f, (float)b / 255.0f, 1.0f}; }
};
