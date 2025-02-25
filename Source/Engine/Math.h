#pragma once

#define FORCE_INLINE __forceinline

#define MATH_EPSILON_FLOAT 1.19209290E-07F
#define MATH_SMALL_NUMBER 0.005
#define MATH_PI 3.1415926535f

//---------------------------------------------------------------------------------------------------------------------------------------------------------
struct TVector2f;
struct TVector3f;
struct TVector4f;
struct TMatrix4f;
//---------------------------------------------------------------------------------------------------------------------------------------------------------
class MMath
{
public:
    static float Square(float _f);
    static float Sin(float _f);
    static float Tan(float _f);
    static float Cos(float _f);
    static float Abs(float _f);
    
    template<typename T> FORCE_INLINE static T Clamp(T _f, T _min, T _max) { return (_f < _min) ? _min : ((_f > _max) ? _max : _f); }
    template<typename T> FORCE_INLINE static T Max(T _t1, T _t2) { return (_t1 > _t2) ? _t1 : _t2; }
    template<typename T> FORCE_INLINE static T Min(T _t1, T _t2) { return (_t1 < _t2) ? _t1 : _t2; }
    
    FORCE_INLINE static float Deg2Rad(float _deg) { return (_deg) * MATH_PI / 180.0f; }
    FORCE_INLINE static float Rad2Deg(float _rad) { return _rad * (180.0f / MATH_PI); }
    
    static int RandomNumberIntegerInRange(int _min, int _max);
    static float RandomNumberIntegerInRange(float _min, float _max);
    static TVector3f RandomVectorIntegerInRange(TVector3f _min, TVector3f _max);
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------
struct TVector2f
{
    float x, y;

    FORCE_INLINE static float Dot(const TVector2f& _v1, const TVector2f& _v2)
    {
        return _v1.x * _v2.x + _v1.y * _v2.y;
    }
};
//---------------------------------------------------------------------------------------------------------------------------------------------------------
struct __declspec(align(16)) TVector3f
{
    float x, y, z;

    static const TVector3f Zero;
    static const TVector3f Forward;
    static const TVector3f Backward;
    static const TVector3f Right;
    static const TVector3f Left;
    static const TVector3f Up;
    static const TVector3f Down;
    
    TVector3f() {}
    TVector3f(float _x, float _y) : x(_x), y(_y), z(0.0f) {}
    TVector3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

#pragma region operator_region
    
    FORCE_INLINE TVector3f operator-() const
    {
        return { -x, -y, -z };
    }
    
    FORCE_INLINE TVector3f& operator*=(const float& _f)
    {
        this->x *= _f;
        this->y *= _f;
        this->z *= _f;
        return *this;
    }
    
    FORCE_INLINE TVector3f operator+(const TVector3f& _v) const
    {
        return { this->x + _v.x, this->y + _v.y, this->z + _v.z};
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

    TVector3f operator*(float _f) const
    {
        return { this->x * _f, this->y * _f, this->z * _f };
    }
    
    FORCE_INLINE friend TVector3f operator*(float _f, const TVector3f& _v)
    {
        return { _v.x *_f, _v.y * _f, _v.z *_f };
    }

#pragma endregion // operator_region
    
    static TVector3f TransformCoord(const TVector3f& _v, const TMatrix4f& _m);
    
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
        return MMath::Square(SquareLength(_v));
    }
    FORCE_INLINE static TVector3f Normalize(const TVector3f& _v)
    {
        const float length = TVector3f::Length(_v);
        return {_v.x / length, _v.y / length, _v.z / length };
    }
    FORCE_INLINE bool IsNormalized() const
    {
        const float squareLength = TVector3f::SquareLength(*this);
        return MMath::Abs(squareLength) - MATH_SMALL_NUMBER <= 1.0f;
    }
};
//---------------------------------------------------------------------------------------------------------------------------------------------------------
struct TVector4f
{
    float x, y, z, w;

    TVector4f() {}
    TVector4f(const float _x, const float _y, const float _z, const float _w) : x(_x), y(_y), z(_z), w(_w) {}

#pragma region operator_region

    FORCE_INLINE friend bool operator==(const TVector4f& _v1, const TVector4f& _v2)
    {
        const float f = MMath::Abs(_v1.x - _v2.x)
                      + MMath::Abs(_v1.y - _v2.y)
                      + MMath::Abs(_v1.z - _v2.z)
                      + MMath::Abs(_v1.w - _v2.w);
        return f < MATH_SMALL_NUMBER;
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
    
#pragma endregion // operator_region 
    
    FORCE_INLINE static float Dot(TVector4f _v1, TVector4f _v2)
    {
        return _v1.x * _v2.x
             + _v1.y * _v2.y
             + _v1.z * _v2.z
             + _v1.w * _v2.w;
    }
};
//---------------------------------------------------------------------------------------------------------------------------------------------------------
struct alignas(16) TMatrix4f
{
    TVector4f x, y, z, w;

#pragma region operator_region

    FORCE_INLINE friend bool operator==(const TMatrix4f& _m1, const TMatrix4f& _m2)
    {
        return _m1.x == _m2.x
            && _m1.y == _m2.y
            && _m1.z == _m2.z
            && _m1.w == _m2.w;
    }
    
    TMatrix4f& operator*=(const TMatrix4f& _m)
    {
        const TMatrix4f mt = TMatrix4f::Transpose(_m);
        this->x = { TVector4f::Dot(this->x, mt.x),    TVector4f::Dot(this->x, mt.y),    TVector4f::Dot(this->x, mt.z),    TVector4f::Dot(this->x, mt.w) };
        this->y = { TVector4f::Dot(this->y, mt.x),    TVector4f::Dot(this->y, mt.y),    TVector4f::Dot(this->y, mt.z),    TVector4f::Dot(this->y, mt.w) };
        this->z = { TVector4f::Dot(this->z, mt.x),    TVector4f::Dot(this->z, mt.y),    TVector4f::Dot(this->z, mt.z),    TVector4f::Dot(this->z, mt.w) };
        this->w = { TVector4f::Dot(this->w, mt.x),    TVector4f::Dot(this->w, mt.y),    TVector4f::Dot(this->w, mt.z),    TVector4f::Dot(this->w, mt.w) };
        return *this;
    }

    FORCE_INLINE friend TMatrix4f operator*(TMatrix4f _m1, const TMatrix4f& _m2)
    {
        _m1 *= _m2;
        return _m1;
    }
    
#pragma endregion // operator_region
    
    static const TMatrix4f Identity;
    static const TMatrix4f World;
    
    static TMatrix4f MatrixTranslation(const TVector3f& _translation);
    static TMatrix4f MatrixRotationPitch(const float _pitch);
    static TMatrix4f MatrixRotationRoll(const float _roll);
    static TMatrix4f MatrixRotationYaw(const float _yaw);
    static TMatrix4f MatrixRotationPitchRollYaw(const float _pitch, const float _roll, const float _yaw);
    static TMatrix4f MatrixScale(const float _scale);
    static TMatrix4f MatrixPerspectiveFovRH(const float _fovAngleY, const float _aspectRatio, const float _nearZ, const float _farZ);
    static TMatrix4f Transpose(const TMatrix4f& _m);
    static TMatrix4f Inverse(const TMatrix4f& _m);
};
//---------------------------------------------------------------------------------------------------------------------------------------------------------
struct TRotator
{
    float Yaw = 0.0f;
    float Pitch = 0.0f;
    float Roll = 0.0f;
};
//---------------------------------------------------------------------------------------------------------------------------------------------------------
struct TTransform
{
    static const TTransform Identity;
    
    static TMatrix4f ToMatrix(const TTransform& _t)
    {
        // TODO Julien Rogel (01/02/2025): Optimize by removing unnecessary matrix multiplication when rotation values are 0
        return TMatrix4f::Identity
             * TMatrix4f::MatrixRotationPitch(_t.Rotator.Pitch)
             * TMatrix4f::MatrixRotationYaw(_t.Rotator.Yaw)
             * TMatrix4f::MatrixRotationRoll(_t.Rotator.Roll)
             * TMatrix4f::MatrixTranslation(_t.Position);
    }
    TVector3f Position = TVector3f::Zero;
    TRotator Rotator = { 0.0f, 0.0f, 0.0f };
};
//---------------------------------------------------------------------------------------------------------------------------------------------------------
