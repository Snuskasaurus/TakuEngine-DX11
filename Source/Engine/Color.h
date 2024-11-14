#pragma once

#include <Windows.h>

struct TFloatColor;
struct TInt8Color;

struct TFloatColor
{
    float r = 0.0f;
    float g = 0.0f;
    float b = 0.0f;
    float a = 0.0f;
    
    TFloatColor(float _r, float _g, float _b, float _a) : r(_r),  g(_r),  b(_r),  a(_a) {}
    TFloatColor(float _r, float _g, float _b) : r(_r),  g(_r),  b(_r),  a(1.0f) {}
    
    static const TFloatColor White;
    static const TFloatColor Black;
    static const TFloatColor Red;
    static const TFloatColor Green;
    static const TFloatColor Blue;
    static const TFloatColor Magenta;
    static const TFloatColor Yellow;
    static const TFloatColor Cyan;
};

struct TInt8Color
{
    UINT8 r = 0u;
    UINT8 g = 0u;
    UINT8 b = 0u;
    UINT8 a = 0u;
    
    TInt8Color(UINT8 _r, UINT8 _g, UINT8 _b, UINT8 _a) : r(_r),  g(_r),  b(_r),  a(_a) {}
    TInt8Color(UINT8 _r, UINT8 _g, UINT8 _b) : r(_r),  g(_r),  b(_r),  a(255u) {}
    
    static const TInt8Color White;
    static const TInt8Color Black;
    static const TInt8Color Red;
    static const TInt8Color Green;
    static const TInt8Color Blue;
    static const TInt8Color Magenta;
    static const TInt8Color Yellow;
    static const TInt8Color Cyan;
    
    static inline TFloatColor ToFloatColor(TInt8Color Color) { return {(float)Color.r / 255.0f, (float)Color.g / 255.0f, (float)Color.b / 255.0f, 1.0f}; }
};

