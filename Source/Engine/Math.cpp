#include "Math.h"

#include <cmath>
#include <cassert>

//----------------------------------------------------------------------------------------------------------------------
float Math::Square(const float _f)
{
    return sqrtf(_f);
}
//----------------------------------------------------------------------------------------------------------------------
float Math::Sin(const float _f)
{
    return sinf(_f);
}
//----------------------------------------------------------------------------------------------------------------------
float Math::Tan(const float _f)
{
    return tanf(_f);
}
//----------------------------------------------------------------------------------------------------------------------
float Math::Cos(const float _f)
{
    return cosf(_f);
}
//----------------------------------------------------------------------------------------------------------------------
float Math::Abs(const float _f)
{
    return fabsf(_f);
}
//----------------------------------------------------------------------------------------------------------------------
const TMatrix4f TMatrix4f::Identity =
{
    { 1.0f, 0.0f, 0.0f, 0.0f },
    { 0.0f, 1.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 1.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f, 1.0f },
};
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::FromDirectXMatrix(const DirectX::XMMATRIX& _matrix)
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
DirectX::XMMATRIX TMatrix4f::ToDirectXMatrix(const TMatrix4f& _matrix)
{
    return DirectX::XMMatrixSet(_matrix.x.x, _matrix.x.y, _matrix.x.z, _matrix.x.w,
                                _matrix.y.x, _matrix.y.y, _matrix.y.z, _matrix.y.w,
                                _matrix.z.x, _matrix.z.y, _matrix.z.z, _matrix.z.w,
                                _matrix.w.x, _matrix.w.y, _matrix.w.z, _matrix.w.w);
}
//----------------------------------------------------------------------------------------------------------------------
bool TMatrix4f::CompareMatrixToDirectXMatrix(const TMatrix4f& _m1, const DirectX::XMMATRIX& _m2)
{
    const TMatrix4f matrixDirectX = FromDirectXMatrix(_m2);
    const TMatrix4f matrixDirectXTransposed = Transpose(matrixDirectX);
    return _m1 == matrixDirectXTransposed;
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixTranslation(const TVector3f& _translation)
{
    return FromDirectXMatrix(DirectX::XMMatrixTranslation(_translation.x, _translation.y, _translation.z));
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixRotationX(const float _angle)
{
    return FromDirectXMatrix(DirectX::XMMatrixRotationX(_angle));
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixRotationY(const float _angle)
{
    return FromDirectXMatrix(DirectX::XMMatrixRotationY(_angle));
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixRotationZ(const float _angle)
{
    return FromDirectXMatrix(DirectX::XMMatrixRotationZ(_angle));
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixScale(const float _scale)
{
    return FromDirectXMatrix(DirectX::XMMatrixScaling(_scale, _scale, _scale));
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixLookAtRH(const TVector3f& _cameraPosition, const TVector3f& _lookAtPosition, const TVector3f& _up)
{
    const DirectX::FXMVECTOR eyePosition = { _cameraPosition.x, _cameraPosition.y, _cameraPosition.z, 0.0f};
    const DirectX::FXMVECTOR focusPosition = { _lookAtPosition.x, _lookAtPosition.y, _lookAtPosition.z, 0.0f};
    const DirectX::FXMVECTOR upDirection = { _up.x, _up.y, _up.z, 0.0f};
    return FromDirectXMatrix(DirectX::XMMatrixLookAtRH(eyePosition, focusPosition, upDirection));
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixPerspectiveFovRH(const float _fovAngleY, const float _aspectRatio, const float _nearZ, const float _farZ)
{
    return FromDirectXMatrix(DirectX::XMMatrixPerspectiveFovRH(_fovAngleY, _aspectRatio, _nearZ, _farZ));
}
//----------------------------------------------------------------------------------------------------------------------
