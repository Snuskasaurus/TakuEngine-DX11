#include "Math.h"

#include <corecrt_math.h>
#include <cstdlib>
#include <DirectXMath.h>
#include <format>

#include "GameWindow.h"
#include "World.h"
#include "Debug/DebugGUIManager.h"

//----------------------------------------------------------------------------------------------------------------------
//------------------------------------------- DirectX Conversions
//----------------------------------------------------------------------------------------------------------------------
static DirectX::XMVECTOR ConvertVec3ToDXDVec_Position(const TVector3f& _v)
{
    return { _v.x, _v.y, _v.z, 1.0f };
}
//----------------------------------------------------------------------------------------------------------------------
static DirectX::XMVECTOR ConvertVec3ToDXDVec_Direction(const TVector3f& _v)
{
    return { _v.x, _v.y, _v.z, 0.0f };
}
//----------------------------------------------------------------------------------------------------------------------
static DirectX::XMVECTOR ConvertVec4ToDXVec(const TVector4f& _v)
{
    return DirectX::XMVectorSet(_v.x, _v.y, _v.z, _v.w);
}
//----------------------------------------------------------------------------------------------------------------------
static TVector4f ConvertDXVecToVec4(DirectX::XMVECTOR _v)
{
    return {
        DirectX::XMVectorGetX(_v),
        DirectX::XMVectorGetY(_v),
        DirectX::XMVectorGetZ(_v),
        DirectX::XMVectorGetW(_v)
    };
}
//----------------------------------------------------------------------------------------------------------------------
static TVector3f ConvertDXVecToVec3(const DirectX::FXMVECTOR& _v)
{
    TVector3f result;
    result.x = DirectX::XMVectorGetX(_v);
    result.y = DirectX::XMVectorGetY(_v);
    result.z = DirectX::XMVectorGetZ(_v);
    return result;
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f ConvertDXMatrixToMatrix(const DirectX::XMMATRIX& _m)
{
    return TMatrix4f
    {
        { DirectX::XMVectorGetX(_m.r[0]), DirectX::XMVectorGetY(_m.r[0]), DirectX::XMVectorGetZ(_m.r[0]), DirectX::XMVectorGetW(_m.r[0]) },
        { DirectX::XMVectorGetX(_m.r[1]), DirectX::XMVectorGetY(_m.r[1]), DirectX::XMVectorGetZ(_m.r[1]), DirectX::XMVectorGetW(_m.r[1]) },
        { DirectX::XMVectorGetX(_m.r[2]), DirectX::XMVectorGetY(_m.r[2]), DirectX::XMVectorGetZ(_m.r[2]), DirectX::XMVectorGetW(_m.r[2]) },
        { DirectX::XMVectorGetX(_m.r[3]), DirectX::XMVectorGetY(_m.r[3]), DirectX::XMVectorGetZ(_m.r[3]), DirectX::XMVectorGetW(_m.r[3]) }
    };
}
//----------------------------------------------------------------------------------------------------------------------
DirectX::XMMATRIX ConvertMatrixToDXMatrix(const TMatrix4f& _m)
{
    return DirectX::XMMatrixSet(
        _m.row0.x, _m.row0.y, _m.row0.z, _m.row0.w,
        _m.row1.x, _m.row1.y, _m.row1.z, _m.row1.w,
        _m.row2.x, _m.row2.y, _m.row2.z, _m.row2.w,
        _m.row3.x, _m.row3.y, _m.row3.z, _m.row3.w
    );
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
float MMath::Lerp(const float _start, const float _end, const float _alpha)
{
    return _start + (_end - _start) * _alpha;
}
//----------------------------------------------------------------------------------------------------------------------
float MMath::MoveTowards(float _current, float _target, float _maxDelta)
{
    float delta = _target - _current;
    if (MMath::Abs(delta) <= _maxDelta)
        return _target;
    return _current + (delta > 0 ? _maxDelta : -_maxDelta);
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
    return (float)(rand() % (int)(_max - _min) + (int)_min);
}
const TVector2f TVector2f::Zero = { 0.0f, 0.0f };
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
RayTraceResult MMath::RayTrace(const TVector2f& _screenPosition)
{
    RayTraceResult Result;

    const DirectX::XMMATRIX projectionMatrix = ConvertMatrixToDXMatrix(MWorld::GetProjectionMatrix());
    const DirectX::XMMATRIX viewMatrix = ConvertMatrixToDXMatrix(MWorld::GetViewMatrix());
    const DirectX::XMMATRIX worldMatrix = DirectX::XMMatrixIdentity();

    const DirectX::XMVECTOR cursorPosition = DirectX::XMVectorSet(_screenPosition.x, _screenPosition.y, 0.0f, 1.0f);
    
    D3D11_VIEWPORT viewport = {};
    {
        viewport.TopLeftX = 0.0f;
        viewport.TopLeftY = 0.0f;
        viewport.Width = (float)MGameWindow::GetGameWindowWidth();
        viewport.Height = (float)MGameWindow::GetGameWindowHeight();
        viewport.MinDepth = 0.0f;
        viewport.MaxDepth = 1.0f;
    }
    
    // Unproject from screen to world space
    const DirectX::XMVECTOR nearPoint = DirectX::XMVector3Unproject(
        cursorPosition,
        viewport.TopLeftX, viewport.TopLeftY,
        viewport.Width, viewport.Height,
        viewport.MinDepth, viewport.MaxDepth,
        projectionMatrix, viewMatrix, worldMatrix
    );

    const DirectX::XMVECTOR farPoint = DirectX::XMVector3Unproject(DirectX::XMVectorSet(_screenPosition.x, _screenPosition.y, 1.0f, 1.0f),
        viewport.TopLeftX, viewport.TopLeftY,
        viewport.Width, viewport.Height,
        viewport.MinDepth, viewport.MaxDepth,
        projectionMatrix, viewMatrix, worldMatrix
    );

    Result.Start = ConvertDXVecToVec3(nearPoint);
    Result.End = ConvertDXVecToVec3(farPoint);
    return Result;
}
//----------------------------------------------------------------------------------------------------------------------
CollisionMeshResult MMath::CollideRayWithMesh(const TVector3f& _start, const TVector3f& _end, const TTransform& _meshTransform, const struct SMeshData& _meshData)
{
    CollisionMeshResult result;

    std::vector<CollisionMeshResult> collisionResults;
    const UINT triangleCount = _meshData.IndexCount / 3;
    for (size_t i = 0; i < triangleCount; ++i)
    {
        const TVertexIndex i0 = _meshData.IndexBuffer[i * 3 + 0];
        const TVertexIndex i1 = _meshData.IndexBuffer[i * 3 + 1];
        const TVertexIndex i2 = _meshData.IndexBuffer[i * 3 + 2];

        if (i0 >= _meshData.VertexBuffer.size() || i1 >= _meshData.VertexBuffer.size() || i2 >= _meshData.VertexBuffer.size())
            continue;

        const TVector3f triangle[3] = { _meshData.VertexBuffer[i0].position, _meshData.VertexBuffer[i1].position, _meshData.VertexBuffer[i2].position };
        const auto newCollisionResult = MMath::DoCollisionSegmentTriangle(_start, _end, triangle);
        if (newCollisionResult.Success == true)
            collisionResults.push_back(newCollisionResult);
    }

    // Find the closest intersection from start
    result.Success = collisionResults.size() > 0;
    if (result.Success == false)
        return result;
    
    int indexClosestCollision = 0;
    if (collisionResults.size() > 1)
    {
        float closestSquaredDistance = 9999999999999.0f;
        for (int i = 0; i < collisionResults.size(); ++i)
        {
            TVector3f distanceFromStart = collisionResults[i].Intersection - _start;
            float distanceSquared = TVector3f::SquareLength(distanceFromStart);
            if (distanceSquared < closestSquaredDistance)
            {
                closestSquaredDistance = distanceSquared;
                indexClosestCollision = i;
            }
        }
    }
    return collisionResults[indexClosestCollision];
}

CollisionMeshResult MMath::DoCollisionSegmentTriangle(const TVector3f& _segmentStart, const TVector3f& _segmentEnd, const TVector3f _triangle[3])
{
    CollisionMeshResult result;
    
    TVector3f segment = _segmentEnd - _segmentStart;
    
    TVector3f edge1 = _triangle[1] - _triangle[0];
    TVector3f edge2 = _triangle[2] - _triangle[0];

    // Vector perpendicular to lineDir and edge02
    const TVector3f crossSegmentEdge2 = TVector3f::Cross(segment, edge2);
    const float determinant = TVector3f::Dot(edge1, crossSegmentEdge2);

    if (fabs(determinant) < G_MATH_EPSILON_FLOAT)
        return result; // If determinant is near 0, the segment is parallel to the triangle: no intersection.

    const float invertDeterminant = 1.0f / determinant;

    // Vector from triangle[0] to lineStart
    const TVector3f triangleToSegmentStart = _segmentStart - _triangle[0];

    // Barycentric Coordinate U = how far from A toward B
    // Barycentric Coordinate V = how far from A toward C
    //   A --------- B
    //    \         /
    //     \   V   /
    //      \     /
    //       \   /
    //         C
    
    // Compute barycentric coordinate u
    const float barycentricCoordinateU = invertDeterminant * TVector3f::Dot(triangleToSegmentStart, crossSegmentEdge2);
    if (barycentricCoordinateU < 0.0f || barycentricCoordinateU > 1.0f)
        return result; 

    // Compute barycentric coordinate v
    TVector3f crossTriangleToStartEdge1 = TVector3f::Cross(triangleToSegmentStart, edge1);
    float barycentricCoordinateV = invertDeterminant * TVector3f::Dot(segment, crossTriangleToStartEdge1);
    if (barycentricCoordinateV < 0.0f || (barycentricCoordinateU + barycentricCoordinateV) > 1.0f)
        return result; // Representing how far the intersection lies along edge1, if < 0 or > 1, intersection is outside the triangle along edge1.

    // Compute intersection distance along the line segment
    float intersectionDistance = invertDeterminant * TVector3f::Dot(edge2, crossTriangleToStartEdge1);
    if (intersectionDistance < 0.0f || intersectionDistance > 1.0f)
        return result; // Intersection point is outside the segment

    // Compute intersection position and normal
    result.Intersection = _segmentStart + segment * intersectionDistance;
    result.Normal = TVector3f::Normalize(TVector3f::Cross(edge1, edge2));
    result.Success = true;

    // Create rotation
    const TVector3f tangent = TVector3f::Normalize(edge1);
    const TVector3f forward = TVector3f::Normalize(TVector3f::Cross(result.Normal, tangent));
    const TVector3f right = TVector3f::Normalize(TVector3f::Cross(forward, result.Normal));
    const DirectX::XMMATRIX rotationMatrix = DirectX::XMMATRIX(
        {right.x, right.y, right.z},
        {result.Normal.x, result.Normal.y, result.Normal.z},
        {forward.x, forward.y, forward.z},
        DirectX::XMVectorSet(0.f, 0.f, 0.f, 1.f)
    );
    result.Rotation = TRotator::CreateFromOrthogonal(result.Normal, forward, right);
    
    return result;
}

//----------------------------------------------------------------------------------------------------------------------
//------------------------------------------- TVector3f
//----------------------------------------------------------------------------------------------------------------------
const TVector3f TVector3f::Zero     = { 0.0f, 0.0f, 0.0f };
const TVector3f TVector3f::Forward  = { 0.0f, 0.0f, 1.0f };  // +Z
const TVector3f TVector3f::Backward = { 0.0f, 0.0f, -1.0f }; // -Z
const TVector3f TVector3f::Right    = { 1.0f, 0.0f, 0.0f };  // +X
const TVector3f TVector3f::Left     = { -1.0f, 0.0f, 0.0f }; // -X
const TVector3f TVector3f::Up       = { 0.0f, 1.0f, 0.0f };  // +Y
const TVector3f TVector3f::Down     = { 0.0f, -1.0f, 0.0f }; // -Y
//----------------------------------------------------------------------------------------------------------------------
void TVector3f::PrintDebugVector(const TVector3f _v)
{
    OutputDebugStringA(("[x=" + std::to_string(_v.x) + "|y=" + std::to_string(_v.y) + "|z=" + std::to_string(_v.z) + "]\n").c_str());
}

bool TVector3f::operator==(const TVector3f& _v)
{
    return fabsf(x - _v.x) < G_MATH_EPSILON_FLOAT
        && fabsf(y - _v.y) < G_MATH_EPSILON_FLOAT
        && fabsf(z - _v.z) < G_MATH_EPSILON_FLOAT;
}
//----------------------------------------------------------------------------------------------------------------------
TVector3f TVector3f::TransformDirection(const TVector3f& _v, const TMatrix4f& _m)
{
    return ConvertDXVecToVec3(DirectX::XMVector3TransformNormal(ConvertVec3ToDXDVec_Direction(_v), ConvertMatrixToDXMatrix(_m)));
}
//----------------------------------------------------------------------------------------------------------------------
TVector3f TVector3f::TransformPosition(const TVector3f& _v, const TMatrix4f& _m)
{
    return ConvertDXVecToVec3(DirectX::XMVector3TransformCoord(ConvertVec3ToDXDVec_Position(_v), ConvertMatrixToDXMatrix(_m)));
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

void TMatrix4f::DisplayOnDebugHUD(const std::string& _name, const TMatrix4f& _m)
{
    ImGui::Text(_name.c_str());
    static ImGuiTableFlags TableFlags = ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_ContextMenuInBody | ImGuiTableFlags_RowBg | ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_NoHostExtendX;

    ImGui::BeginTable(_name.c_str(), 4, TableFlags, ImVec2(0.0f, 0.0f));

    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_NoHide, 40.0f, 0);
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_NoHide, 40.0f, 1);
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_NoHide, 40.0f, 2);
    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_NoHide, 40.0f, 3);
    
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Text("%.02f", _m.row0.x);
    ImGui::TableNextColumn();
    ImGui::Text("%.02f", _m.row0.y);
    ImGui::TableNextColumn();
    ImGui::Text("%.02f", _m.row0.z);
    ImGui::TableNextColumn();
    ImGui::Text("%.02f", _m.row0.w);
    
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Text("%.02f", _m.row1.x);
    ImGui::TableNextColumn();
    ImGui::Text("%.02f", _m.row1.y);
    ImGui::TableNextColumn();
    ImGui::Text("%.02f", _m.row1.z);
    ImGui::TableNextColumn();
    ImGui::Text("%.02f", _m.row1.w);
    
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Text("%.02f", _m.row2.x);
    ImGui::TableNextColumn();
    ImGui::Text("%.02f", _m.row2.y);
    ImGui::TableNextColumn();
    ImGui::Text("%.02f", _m.row2.z);
    ImGui::TableNextColumn();
    ImGui::Text("%.02f", _m.row2.w);
    
    ImGui::TableNextRow();
    ImGui::TableNextColumn();
    ImGui::Text("%.02f", _m.row3.x);
    ImGui::TableNextColumn();
    ImGui::Text("%.02f", _m.row3.y);
    ImGui::TableNextColumn();
    ImGui::Text("%.02f", _m.row3.z);
    ImGui::TableNextColumn();
    ImGui::Text("%.02f", _m.row3.w);
    
    ImGui::EndTable();
}

//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixTranslation(const TVector3f& _translation)
{
    return ConvertDXMatrixToMatrix(DirectX::XMMatrixTranslation(_translation.x, _translation.y, _translation.z));
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixRotationPitch(const float _pitch)
{
    return ConvertDXMatrixToMatrix(DirectX::XMMatrixRotationX(-_pitch));
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixRotationYaw(const float _yaw)
{
    return ConvertDXMatrixToMatrix(DirectX::XMMatrixRotationY(-_yaw));
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixRotationRoll(const float _roll)
{
    return ConvertDXMatrixToMatrix(DirectX::XMMatrixRotationZ(-_roll));
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixRotationPitchYawRoll(const float _pitch, const float _yaw, const float _roll)
{
    return ConvertDXMatrixToMatrix(DirectX::XMMatrixRotationRollPitchYaw(-_pitch, -_yaw, -_roll));
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixScale(const float _scale)
{
    return ConvertDXMatrixToMatrix(DirectX::XMMatrixScaling(_scale, _scale, _scale));
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixPerspectiveFov(const float _fovAngleY, const float _aspectRatio, const float _nearZ, const float _farZ)
{
    return ConvertDXMatrixToMatrix(DirectX::XMMatrixPerspectiveFovRH(_fovAngleY, _aspectRatio, _nearZ, _farZ));
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixOrthographic(float _viewWidth, float _viewHeight, float _nearZ, float _farZ)
{
     return ConvertDXMatrixToMatrix(DirectX::XMMatrixOrthographicRH(_viewWidth, _viewHeight, _nearZ, _farZ));
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::MatrixLookTo(TVector3f _position, TVector3f _forward, TVector3f _up)
{
    return ConvertDXMatrixToMatrix(DirectX::XMMatrixLookToRH(
        ConvertVec3ToDXDVec_Position(_position),
        ConvertVec3ToDXDVec_Direction(_forward),
        ConvertVec3ToDXDVec_Direction(_up)));
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::Transpose(const TMatrix4f& _m)
{
    const DirectX::XMMATRIX matrix = ConvertMatrixToDXMatrix(_m);
    return ConvertDXMatrixToMatrix(DirectX::XMMatrixTranspose(matrix));
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TMatrix4f::Inverse(const TMatrix4f& _m)
{
    const DirectX::XMMATRIX matrix = ConvertMatrixToDXMatrix(_m);
    return ConvertDXMatrixToMatrix(DirectX::XMMatrixInverse(nullptr, matrix));
}
//----------------------------------------------------------------------------------------------------------------------
TRotator TRotator::CreateFromOrthogonal(const TVector3f& _up, const TVector3f& _forward, const TVector3f& _right)
{
    TRotator rotator;

    rotator.Pitch = asinf(-_forward.y);
    const float cosPitch = cosf(rotator.Pitch);
    if (fabsf(cosPitch) > 1e-6f)
    {
        rotator.Yaw = atan2f(_forward.x, _forward.z);
        rotator.Roll = atan2f(_right.y, _up.y);
    }
    else
    {
        rotator.Yaw = atan2f(-_right.z, _right.x);
        rotator.Roll = 0.0f;
    }
    return rotator;
}
//----------------------------------------------------------------------------------------------------------------------
//------------------------------------------- TTransform
//----------------------------------------------------------------------------------------------------------------------
const TTransform TTransform::Identity =
{
    { 0.0f, 0.0f, 0.0f },
    { 0.0f, 0.0f, 0.0f}
};
//----------------------------------------------------------------------------------------------------------------------
void TTransform::DebugPrintTransform(const TTransform& _t)
{
    TVector3f::PrintDebugVector(_t.Position);
    OutputDebugStringA(("[Pitch=" + std::to_string(_t.Rotator.Pitch)
        + "|Roll=" + std::to_string(_t.Rotator.Roll)
        + "|Yaw=" + std::to_string(_t.Rotator.Yaw)
        + "]\n").c_str());
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TTransform::WorldMatrix() const
{
    return TMatrix4f::Identity * RotationMatrix() * PositionMatrix();
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TTransform::PositionMatrix() const
{
    return TMatrix4f::MatrixTranslation(Position);
}
//----------------------------------------------------------------------------------------------------------------------
TMatrix4f TTransform::RotationMatrix() const
{
    return TMatrix4f::MatrixRotationPitchYawRoll(Rotator.Pitch, Rotator.Yaw, Rotator.Roll);
}
//----------------------------------------------------------------------------------------------------------------------
TVector3f TTransform::Forward() const
{
    const TMatrix4f rotationMatrix = RotationMatrix();
    TVector3f forward = TVector3f::TransformDirection(TVector3f::Forward, rotationMatrix);
    forward = TVector3f::Normalize(forward);
    return forward;
}
//----------------------------------------------------------------------------------------------------------------------
TVector3f TTransform::Right() const
{
    const TMatrix4f rotationMatrix = RotationMatrix();
    TVector3f right = TVector3f::TransformDirection(TVector3f::Right, rotationMatrix);
    right = TVector3f::Normalize(right);
    return right;
}
//----------------------------------------------------------------------------------------------------------------------
TVector3f TTransform::Up() const
{
    const TMatrix4f rotationMatrix = RotationMatrix();
    TVector3f up = TVector3f::TransformDirection(TVector3f::Up, rotationMatrix);
    up = TVector3f::Normalize(up);
    return up;
}
//----------------------------------------------------------------------------------------------------------------------