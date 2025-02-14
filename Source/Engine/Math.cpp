#include "Math.h"

#include <corecrt_math.h>
#include <cstdlib>
#include <DirectXMath.h>

//----------------------------------------------------------------------------------------------------------------------
//------------------------------------------- DirectX Conversions
//----------------------------------------------------------------------------------------------------------------------
static DirectX::FXMVECTOR ToDirectXVector(const TVector3f& _v)
{
    return  { _v.x, _v.y, _v.z, 0.0f };
}
//----------------------------------------------------------------------------------------------------------------------
static auto FromDirectXVector(const DirectX::FXMVECTOR& _v) -> TVector3f
{
    TVector3f result;
    result.x = DirectX::XMVectorGetX(_v);
    result.y = DirectX::XMVectorGetY(_v);
    result.z = DirectX::XMVectorGetZ(_v);
    return result;
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f FromDirectXMatrix(const DirectX::XMMATRIX& _matrix)
{
#define DX_GET_X(v) DirectX::XMVectorGetX(_matrix.r[v])
#define DX_GET_Y(v) DirectX::XMVectorGetY(_matrix.r[v])
#define DX_GET_Z(v) DirectX::XMVectorGetZ(_matrix.r[v])
#define DX_GET_W(v) DirectX::XMVectorGetW(_matrix.r[v])
    return
    {
		    { DX_GET_X(0),   DX_GET_Y(0),   DX_GET_Z(0),   DX_GET_W(0) },
            { DX_GET_X(1),   DX_GET_Y(1),   DX_GET_Z(1),   DX_GET_W(1) },
            { DX_GET_X(2),   DX_GET_Y(2),   DX_GET_Z(2),   DX_GET_W(2) },
            { DX_GET_X(3),   DX_GET_Y(3),   DX_GET_Z(3),   DX_GET_W(3) }
    };
#undef DX_GET_W
#undef DX_GET_Z
#undef DX_GET_Y
#undef DX_GET_X
}
//----------------------------------------------------------------------------------------------------------------------
DirectX::XMMATRIX ToDirectXMatrix(const TMatrix4f& _matrix)
{
    return DirectX::XMMatrixSet(_matrix.x.x, _matrix.x.y, _matrix.x.z, _matrix.x.w,
                                _matrix.y.x, _matrix.y.y, _matrix.y.z, _matrix.y.w,
                                _matrix.z.x, _matrix.z.y, _matrix.z.z, _matrix.z.w,
                                _matrix.w.x, _matrix.w.y, _matrix.w.z, _matrix.w.w);
}
//----------------------------------------------------------------------------------------------------------------------
//------------------------------------------- Math Common
//----------------------------------------------------------------------------------------------------------------------
float MMath::Square(const float _f)
{
    return sqrtf(_f);
}
//----------------------------------------------------------------------------------------------------------------------
float MMath::Sin(const float _f)
{
    return sinf(_f);
}
//----------------------------------------------------------------------------------------------------------------------
float MMath::Tan(const float _f)
{
    return tanf(_f);
}
//----------------------------------------------------------------------------------------------------------------------
float MMath::Cos(const float _f)
{
    return cosf(_f);
}
//----------------------------------------------------------------------------------------------------------------------
float MMath::Abs(const float _f)
{
    return fabsf(_f);
}
//----------------------------------------------------------------------------------------------------------------------
float MMath::Clamp(const float _f, const float _min, const float _max)
{
    return (_f < _min) ? _min : ((_f > _max) ? _max : _f);
}
//----------------------------------------------------------------------------------------------------------------------
int MMath::RandomNumberIntegerInRange(int _min, int _max)
{
    if (_min == _max) return 0;
    return rand() % (_max - _min) + _min;
}
//----------------------------------------------------------------------------------------------------------------------
float MMath::RandomNumberIntegerInRange(float _min, float _max)
{
    if (_max - _min <= 1.0f) return 0.0f;
    return (float)(rand() % (int)((_max - _min) + _min));
}
//----------------------------------------------------------------------------------------------------------------------
TVector3f MMath::RandomVectorIntegerInRange(TVector3f _min, TVector3f _max)
{
    return
    {
        RandomNumberIntegerInRange(_min.x, _max.x),
        RandomNumberIntegerInRange(_min.y, _max.y),
        RandomNumberIntegerInRange(_min.z, _max.z),
    };
}
//----------------------------------------------------------------------------------------------------------------------
//------------------------------------------- TVector3f
//----------------------------------------------------------------------------------------------------------------------
const TVector3f TVector3f::Zero = { 0.0f, 0.0f, 0.0f };
const TVector3f TVector3f::Forward = { 0.0f, 1.0f, 0.0f };
const TVector3f TVector3f::Backward = { 0.0f, -1.0f, 0.0f };
const TVector3f TVector3f::Right = { 1.0f, 0.0f, 0.0f };
const TVector3f TVector3f::Left = { -1.0f, 0.0f, 0.0f };
const TVector3f TVector3f::Up = { 0.0f, 0.0f, 1.0f };
const TVector3f TVector3f::Down = { 0.0f, 0.0f, -1.0f };

TVector3f TVector3f::TransformCoord(const TVector3f& _v, const TMatrix4f& _m)
{
    return FromDirectXVector(DirectX::XMVector3TransformCoord(ToDirectXVector(_v), ToDirectXMatrix(_m)));
}
//----------------------------------------------------------------------------------------------------------------------
//------------------------------------------- TMatrix4f
//----------------------------------------------------------------------------------------------------------------------
const TMatrix4f TMatrix4f::Identity =
{
    { 1.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 1.0f },
};
const TMatrix4f TMatrix4f::View =
{
    { 1.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, -1.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 1.0f },
};
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixTranslation(const TVector3f& _translation)
{
    return FromDirectXMatrix(DirectX::XMMatrixTranslation(_translation.x, _translation.y, _translation.z));
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixRotationPitch(const float _pitch)
{
    return FromDirectXMatrix(DirectX::XMMatrixRotationX(_pitch));
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixRotationRoll(const float _roll)
{
    return FromDirectXMatrix(DirectX::XMMatrixRotationY(_roll));
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixRotationYaw(const float _yaw)
{
    return FromDirectXMatrix(DirectX::XMMatrixRotationZ(_yaw));
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixRotationPitchRollYaw(float _pitch, float _roll, float _yaw)
{
    return FromDirectXMatrix(DirectX::XMMatrixRotationRollPitchYaw(_pitch, _roll, _yaw)); // NOLINT(Inverted to match my View matrix, readability-suspicious-call-argument)
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixScale(const float _scale)
{
    return FromDirectXMatrix(DirectX::XMMatrixScaling(_scale, _scale, _scale));
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixPerspectiveFovRH(const float _fovAngleY, const float _aspectRatio, const float _nearZ, const float _farZ)
{
    return FromDirectXMatrix(DirectX::XMMatrixPerspectiveFovRH(_fovAngleY, _aspectRatio, _nearZ, _farZ));
}

TMatrix4f TMatrix4f::Transpose(const TMatrix4f& _m)
{
    const DirectX::XMMATRIX matrix = ToDirectXMatrix(_m);
    return FromDirectXMatrix(DirectX::XMMatrixTranspose(matrix));
}

TMatrix4f TMatrix4f::Inverse(const TMatrix4f& _m)
{
    const DirectX::XMMATRIX matrix = ToDirectXMatrix(_m);
    return FromDirectXMatrix(DirectX::XMMatrixInverse(nullptr, matrix));
}

//----------------------------------------------------------------------------------------------------------------------
