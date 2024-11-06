#pragma once

#include <sal.h>

using TVertexIndex = unsigned short;

struct float2
{
    float x = 0.0f;
    float y = 0.0f;
};

struct float3
{
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;
};

struct SVertex
{
    float3 Position;
    float2 TextureCoordinate;
};

struct SVertexBuffer
{
    SVertex Vertices[1024];
};

struct IndexBuffer
{
    TVertexIndex Indexes[1024];
};

struct SMeshInfo
{
    SVertexBuffer VertexBuffer;
    int nbVertexBuffer = 0;
    IndexBuffer IndexBuffer;
    int nbIndexBuffer = 0;
};

bool TryToImportMeshInfoFromOBJFile(_In_ const wchar_t* _fileName, _Out_ SMeshInfo* SMeshInfo);