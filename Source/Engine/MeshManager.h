#pragma once

#include <string>
#include <vector>

#include "Color.h"
#include "Math.h"

using TVertexIndex = unsigned short;

struct alignas(16) SVertex
{
    TVector3f vp;
    TVector2f vt;
    TVector3f vn;
};

struct SMeshData
{
    static constexpr UINT VertexBuffer_StructureByteStride = sizeof(SVertex);
    static constexpr UINT IndexBuffer_StructureByteStride = sizeof(TVertexIndex);
    
    UINT VertexBuffer_ByteWidth;
    UINT IndexBuffer_ByteWidth;
    
    std::vector<SVertex> VertexBuffer;
    std::vector<TVertexIndex> IndexBuffer;

    UINT IndexCount;
};

class MeshManager
{
public:
    static void InitializeMeshManager();
    static void DeinitializeMeshManager();
    static bool TryToImportOBJ(const char* Filename, SMeshData* MeshData);
};
