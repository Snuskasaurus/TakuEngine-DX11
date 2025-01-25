#pragma once

#include <sal.h>
#include "Math.h"

#define MAX_VERTEX_BUFFER_VERTICES 4096
#define MAX_INDEX_BUFFER_INDEXES 4096

using TVertexIndex = unsigned short;

struct alignas(16) SVertex
{
    TVector3f vp;
    TVector2f vt;
    TVector3f vn;
};

struct alignas(16) SVertexBuffer
{
    SVertex Vertices[MAX_VERTEX_BUFFER_VERTICES];
};

struct alignas(16) IndexBuffer
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