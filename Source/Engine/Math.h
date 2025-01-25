#pragma once

#define FORCE_INLINE __forceinline
#define EPSILON_FLOAT 1.19209290E-07F
#define VEC_PRECISION 0.005

#include <DirectXMath.h>

//---------------------------------------------------------------------------------------------------------------------------------------------------------
namespace Math // To avoid having CMath included everywhere
{
    FORCE_INLINE float Square(const float _f);
    FORCE_INLINE float Sin(const float _f);
    FORCE_INLINE float Tan(const float _f);
    FORCE_INLINE float Cos(const float _f);
    FORCE_INLINE float Abs(const float _f);
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
struct TVector2f
{
    float x = 0.0f;
    float y = 0.0f;

    FORCE_INLINE static float Dot(const TVector2f& _v1, const TVector2f& _v2)
    {
        return _v1.x * _v2.x + _v1.y * _v2.y;
    }
};
//---------------------------------------------------------------------------------------------------------------------------------------------------------
struct TVector3f
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    TVector3f() : x(0.0f), y(0.0f), z(0.0f) {}
    TVector3f(float _x, float _y) : x(_x), y(_y), z(0.0f) {}
    TVector3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}
    
    DirectX::FXMVECTOR ToDXVec() const
    {
        DirectX::FXMVECTOR NewVec = { x, y, z, 0.0f };
        return NewVec;
    }
    
    FORCE_INLINE static float Dot(const TVector3f& _v1, const TVector3f& _v2)
    {
        return _v1.x * _v2.x + _v1.y * _v2.y + _v1.z * _v2.z;
    }
    FORCE_INLINE static TVector3f Cross(const TVector3f& _v1, const TVector3f& _v2)
    {
        return
        {
            _v1.y * _v2.z - _v1.z * _v2.y,
            _v1.x * _v2.z - _v1.z * _v2.x,
            _v1.x * _v2.y - _v1.y * _v2.x,
        };
    }
    FORCE_INLINE static float SquareLength(const TVector3f& _v)
    {
        return _v.x * _v.x
             + _v.y * _v.y
             + _v.z * _v.z;
    }
    FORCE_INLINE static float Length(const TVector3f& _v)
    {
        return Math::Square(SquareLength(_v));
    }
    FORCE_INLINE static TVector3f Normalize(const TVector3f& _v)
    {
        const float length = TVector3f::Length(_v);
        return {_v.x / length, _v.y / length, _v.z / length };
    }
    FORCE_INLINE bool IsNormalized() const
    {
        const float squareLength = TVector3f::SquareLength(*this);
        return Math::Abs(squareLength) - VEC_PRECISION <= 1.0f;
    }
    FORCE_INLINE TVector3f operator-() const
    {
        return { -x, -y, -z };
    }
    FORCE_INLINE TVector3f& operator+=(const TVector3f& _v)
    {
        this->x += _v.x;
        this->y += _v.y;
        this->z += _v.z;
        return *this;
    }
    FORCE_INLINE TVector3f& operator-=(const TVector3f& _v)
    {
        this->x -= _v.x;
        this->y -= _v.y;
        this->z -= _v.z;
        return *this;
    }
    FORCE_INLINE friend TVector3f operator+(TVector3f _v1, const TVector3f& _v2)
    {
        _v1 += _v2;
        return _v1;
    }
    FORCE_INLINE friend TVector3f operator-(TVector3f _v1, const TVector3f& _v2)
    {
        _v1 += _v2;
        return _v1;
    }
};
//---------------------------------------------------------------------------------------------------------------------------------------------------------
struct TVector4f
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
    float w = 0.0f;

    TVector4f() = default;
    TVector4f(const float _x, const float _y, const float _z, const float _w) : x(_x), y(_y), z(_z), w(_w) {}
    ~TVector4f() = default;
    
    FORCE_INLINE static float Dot(TVector4f _v1, TVector4f _v2)
    {
        return _v1.x * _v2.x
             + _v1.y * _v2.y
             + _v1.z * _v2.z
             + _v1.w * _v2.w;
    }
    
    FORCE_INLINE friend bool operator==(const TVector4f& _v1, const TVector4f& _v2)
    {
        const float f = Math::Abs(_v1.x - _v2.x)
                      + Math::Abs(_v1.y - _v2.y)
                      + Math::Abs(_v1.z - _v2.z)
                      + Math::Abs(_v1.w - _v2.w);
        return f < VEC_PRECISION;
    }
    FORCE_INLINE TVector4f operator-() const
    {
        return { -x, -y, -z, -w };
    }
    FORCE_INLINE TVector4f& operator+=(const TVector4f& _v)
    {
        this->x += _v.x;
        this->y += _v.y;
        this->z += _v.z;
        this->w += _v.w;
        return *this;
    }
    FORCE_INLINE TVector4f& operator-=(const TVector4f& _v)
    {
        this->x -= _v.x;
        this->y -= _v.y;
        this->z -= _v.z;
        this->w -= _v.w;
        return *this;
    }
    FORCE_INLINE friend TVector4f operator+(TVector4f _v1, const TVector4f& _v2)
    {
        _v1 += _v2;
        return _v1;
    }
    FORCE_INLINE friend TVector4f operator-(TVector4f _v1, const TVector4f& _v2)
    {
        _v1 += _v2;
        return _v1;
    }
};
//---------------------------------------------------------------------------------------------------------------------------------------------------------
struct alignas(16) TMatrix4f
{
    TVector4f x = { 0.0f, 0.0f, 0.0f, 0.0f };
    TVector4f y = { 0.0f, 0.0f, 0.0f, 0.0f };
    TVector4f z = { 0.0f, 0.0f, 0.0f, 0.0f };
    TVector4f w = { 0.0f, 0.0f, 0.0f, 0.0f };

    static const TMatrix4f Identity;

    static TMatrix4f FromDirectXMatrix(const DirectX::XMMATRIX& _matrix);
    static DirectX::XMMATRIX ToDirectXMatrix(const TMatrix4f& _matrix);
    static bool CompareMatrixToDirectXMatrix(const TMatrix4f& _m1, const DirectX::XMMATRIX& _m2);
    
    static TMatrix4f MatrixTranslation(const TVector3f& _translation);
    static TMatrix4f MatrixRotationX(float _angle); 
    static TMatrix4f MatrixRotationY(float _angle); 
    static TMatrix4f MatrixRotationZ(float _angle); 
    static TMatrix4f MatrixScale(float _scale);
    static TMatrix4f MatrixLookAtRH(const TVector3f& _cameraPosition, const TVector3f& _lookAtPosition, const TVector3f& _up);
    static TMatrix4f MatrixPerspectiveFovRH(float _fovAngleY, float _aspectRatio, float _nearZ, float _farZ);
    
    FORCE_INLINE static TMatrix4f Transpose(const TMatrix4f& _m)
    {
        const DirectX::XMMATRIX matrix = ToDirectXMatrix(_m);
        return FromDirectXMatrix(DirectX::XMMatrixTranspose(matrix));
    }

    FORCE_INLINE friend bool operator==(const TMatrix4f& _m1, const TMatrix4f& _m2)
    {
        return _m1.x == _m2.x
			&& _m1.y == _m2.y
			&& _m1.z == _m2.z
			&& _m1.w == _m2.w;
    }
    FORCE_INLINE TMatrix4f& operator*=(const TMatrix4f& _m)
    {
        const DirectX::XMMATRIX matrix1 = ToDirectXMatrix(*this);
        const DirectX::XMMATRIX matrix2 = ToDirectXMatrix(_m);
        const TMatrix4f Result = FromDirectXMatrix(matrix1 * matrix2);
        this->x = Result.x;
        this->y = Result.y;
        this->z = Result.z;
        this->w = Result.w;
        return *this;
        
        // const TMatrix4f mt = TMatrix4f::Transpose(_m);
        // this->x = { TVector4f::Dot(this->x, mt.x),    TVector4f::Dot(this->x, mt.y),    TVector4f::Dot(this->x, mt.z),    TVector4f::Dot(this->x, mt.w) };
        // this->y = { TVector4f::Dot(this->y, mt.x),    TVector4f::Dot(this->y, mt.y),    TVector4f::Dot(this->y, mt.z),    TVector4f::Dot(this->y, mt.w) };
        // this->z = { TVector4f::Dot(this->z, mt.x),    TVector4f::Dot(this->z, mt.y),    TVector4f::Dot(this->z, mt.z),    TVector4f::Dot(this->z, mt.w) };
        // this->w = { TVector4f::Dot(this->w, mt.x),    TVector4f::Dot(this->w, mt.y),    TVector4f::Dot(this->w, mt.z),    TVector4f::Dot(this->w, mt.w) };
        // return *this;
    }
    FORCE_INLINE friend TMatrix4f operator*(TMatrix4f _m1, const TMatrix4f& _m2)
    {
        _m1 *= _m2;
        return _m1;
    }
	FORCE_INLINE TVector4f& operator*=(const TVector4f& _v)
	{
		
	}
	FORCE_INLINE friend TVector4f operator*(const TVector4f& _v)
	{
        
	}
};
//---------------------------------------------------------------------------------------------------------------------------------------------------------