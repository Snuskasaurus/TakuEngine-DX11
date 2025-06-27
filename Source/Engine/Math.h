#pragma once

#define FORCE_INLINE __forceinline

#define MATH_EPSILON_FLOAT 1.19209290E-07F
#define MATH_SMALL_NUMBER 0.005
#define MATH_PI 3.1415926535f
#include <string>

struct RayTraceResult;
struct CollisionMeshResult;
//---------------------------------------------------------------------------------------------------------------------------------------------------------
struct TVector2f;
struct TVector3f;
struct TVector4f;
struct TMatrix4f;
struct TTransform;
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

    static RayTraceResult RayTrace(const TVector2f& _screenPosition);
    static CollisionMeshResult CollideRayWithMesh(const TVector3f& _start, const TVector3f& _end, const TTransform& _meshTransform, const struct SMeshData& _meshData);
    static CollisionMeshResult DoCollisionSegmentTriangle(const TVector3f& _segmentStart, const TVector3f& _segmentEnd, const TVector3f _triangle[3]);
};

//---------------------------------------------------------------------------------------------------------------------------------------------------------
struct TVector2f
{
    float x, y;

    static const TVector2f Zero;

#pragma region operator_region
    
    FORCE_INLINE TVector2f operator-() const
    {
        return { -x, -y };
    }

    FORCE_INLINE TVector2f operator-(const TVector2f& _v2) const
    {
        return { x - _v2.x, y - _v2.y };
    }
    
    FORCE_INLINE TVector2f& operator*=(const float& _f)
    {
        this->x *= _f;
        this->y *= _f;
        return *this;
    }
    
    FORCE_INLINE TVector2f operator+(const TVector2f& _v) const
    {
        return { this->x + _v.x, this->y + _v.y };
    }
    
    FORCE_INLINE TVector2f& operator+=(const TVector2f& _v)
    {
        this->x += _v.x;
        this->y += _v.y;
        return *this;
    }
    
    FORCE_INLINE TVector2f& operator-=(const TVector2f& _v)
    {
        this->x -= _v.x;
        this->y -= _v.y;
        return *this;
    }

    TVector2f operator*(float _f) const
    {
        return { this->x * _f, this->y * _f };
    }
    
    FORCE_INLINE friend TVector2f operator*(float _f, const TVector2f& _v)
    {
        return { _v.x *_f, _v.y * _f };
    }

    TVector2f operator/(float _f) const
    {
        return { this->x / _f, this->y / _f };
    }
    
    FORCE_INLINE friend TVector2f operator/(float _f, const TVector2f& _v)
    {
        return { _v.x / _f, _v.y };
    }

#pragma endregion // operator_region
    
    FORCE_INLINE static float Dot(const TVector2f& _v1, const TVector2f& _v2)
    {
        return _v1.x * _v2.x + _v1.y * _v2.y;
    }
};
//---------------------------------------------------------------------------------------------------------------------------------------------------------
struct alignas(16) TVector3f
{
    float x, y, z;

    static const TVector3f Zero;
    static const TVector3f Forward;
    static const TVector3f Backward;
    static const TVector3f Right;
    static const TVector3f Left;
    static const TVector3f Up;
    static const TVector3f Down;

    static void PrintDebugVector(const TVector3f _v);
    
    TVector3f() {}
    TVector3f(float _x, float _y) : x(_x), y(_y), z(0.0f) {}
    TVector3f(float _x, float _y, float _z) : x(_x), y(_y), z(_z) {}

#pragma region operator_region

    bool operator==(const TVector3f& _v);

    FORCE_INLINE TVector3f operator-() const
    {
        return { -x, -y, -z };
    }

    FORCE_INLINE TVector3f operator-(const TVector3f& _v2) const
    {
        return TVector3f(x - _v2.x, y - _v2.y, z - _v2.z);
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

    TVector3f operator/(float _f) const
    {
        return { this->x / _f, this->y / _f, this->z / _f };
    }
    
    FORCE_INLINE friend TVector3f operator/(float _f, const TVector3f& _v)
    {
        return { _v.x / _f, _v.y / _f, _v.z / _f };
    }

#pragma endregion // operator_region
    
    static TVector3f TransformDirection(const TVector3f& _v, const TMatrix4f& _m);
    static TVector3f TransformPosition(const TVector3f& _v, const TMatrix4f& _m);

    FORCE_INLINE static float Dot(const TVector3f& _v1, const TVector3f& _v2)
    {
        return _v1.x * _v2.x + _v1.y * _v2.y + _v1.z * _v2.z;
    }
    FORCE_INLINE static TVector3f Cross(const TVector3f& _v1, const TVector3f& _v2)
    {
        return
        {
            _v1.y * _v2.z - _v1.z * _v2.y,  // X
            _v1.z * _v2.x - _v1.x * _v2.z,  // Y
            _v1.x * _v2.y - _v1.y * _v2.x   // Z
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
struct alignas(16) TVector4f
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
struct alignas(16) TMatrix4f // row-major matrices to stay consistent with  DirectX::XMMATRIX
{
    TVector4f row0;
    TVector4f row1;
    TVector4f row2;
    TVector4f row3;

#pragma region operator_region

    FORCE_INLINE friend bool operator==(const TMatrix4f& _m1, const TMatrix4f& _m2)
    {
        return _m1.row0 == _m2.row0 && _m1.row1 == _m2.row1 && _m1.row2 == _m2.row2 && _m1.row3 == _m2.row3;
    }
    
    TMatrix4f& operator*=(const TMatrix4f& _m)
    {
        const TMatrix4f mt = TMatrix4f::Transpose(_m);
        row0 = { TVector4f::Dot(row0, mt.row0), TVector4f::Dot(row0, mt.row1),
                 TVector4f::Dot(row0, mt.row2), TVector4f::Dot(row0, mt.row3) };
        row1 = { TVector4f::Dot(row1, mt.row0), TVector4f::Dot(row1, mt.row1),
                 TVector4f::Dot(row1, mt.row2), TVector4f::Dot(row1, mt.row3) };
        row2 = { TVector4f::Dot(row2, mt.row0), TVector4f::Dot(row2, mt.row1),
                 TVector4f::Dot(row2, mt.row2), TVector4f::Dot(row2, mt.row3) };
        row3 = { TVector4f::Dot(row3, mt.row0), TVector4f::Dot(row3, mt.row1),
                 TVector4f::Dot(row3, mt.row2), TVector4f::Dot(row3, mt.row3) };
        return *this;
    }

    FORCE_INLINE friend TMatrix4f operator*(TMatrix4f _m1, const TMatrix4f& _m2)
    {
        _m1 *= _m2;
        return _m1;
    }

    static void DisplayOnDebugHUD(const std::string& _name, const TMatrix4f& _m);
    
#pragma endregion // operator_region
    
    static const TMatrix4f Identity;
    static const TMatrix4f WorldIdentity;
    
    static TMatrix4f MatrixTranslation(const TVector3f& _translation);
    static TMatrix4f MatrixRotationPitch(const float _pitch);
    static TMatrix4f MatrixRotationYaw(const float _yaw);
    static TMatrix4f MatrixRotationRoll(const float _roll);
    static TMatrix4f MatrixRotationPitchYawRoll(const float _pitch, const float _yaw, const float _roll);
    static TMatrix4f MatrixScale(const float _scale);
    static TMatrix4f MatrixPerspectiveFov(const float _fovAngleY, const float _aspectRatio, const float _nearZ, const float _farZ);
    static TMatrix4f MatrixOrthographic(float _viewWidth, float _viewHeight, float _nearZ, float _farZ);
    static TMatrix4f MatrixLookTo(TVector3f _position, TVector3f _forward, TVector3f _up);
    static TMatrix4f Transpose(const TMatrix4f& _m);
    static TMatrix4f Inverse(const TMatrix4f& _m);
};
//---------------------------------------------------------------------------------------------------------------------------------------------------------
struct TRotator
{
    float Pitch = 0.0f;
    float Yaw = 0.0f;
    float Roll = 0.0f;

    static TRotator CreateFromUp(const TVector3f& _up);
};
//---------------------------------------------------------------------------------------------------------------------------------------------------------
struct TTransform
{
    TVector3f Position = TVector3f::Zero;
    TRotator Rotator = { 0.0f, 0.0f, 0.0f };
    
    static const TTransform Identity;

    static void DebugPrintTransform(const TTransform& _t);
    
    TMatrix4f WorldMatrix() const;
    TMatrix4f PositionMatrix() const;
    TMatrix4f RotationMatrix() const;
    TVector3f Forward() const;
    TVector3f Right() const;
    TVector3f Up() const;
};
//---------------------------------------------------------------------------------------------------------------------------------------------------------
struct RayTraceResult
{
    TVector3f Start;  
    TVector3f End; 
};
struct CollisionMeshResult
{
    bool Success = false;
    TVector3f Intersection;
    TVector3f Normal;
};