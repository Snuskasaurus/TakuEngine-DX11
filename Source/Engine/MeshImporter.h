#pragma once

#include <sal.h>

#define MAX_VERTEX_BUFFER_VERTICES 4096
#define MAX_INDEX_BUFFER_INDEXES 4096

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
    float3 vp;
    float2 vt;
    float3 vn;
};

struct SVertexBuffer
{
    SVertex Vertices[MAX_VERTEX_BUFFER_VERTICES];
};

struct IndexBuffer
{
    TVertexIndex Indexes[MAX_INDEX_BUFFER_INDEXES];
};

struct SMeshInfo
{
    SVertexBuffer VertexBuffer;
    int nbVertexBuffer = 0;
    IndexBuffer IndexBuffer;
    int nbIndexBuffer = 0;
};

bool TryToImportMeshInfoFromOBJFile(_In_ const wchar_t* _fileName, _Out_ SMeshInfo* SMeshInfo);