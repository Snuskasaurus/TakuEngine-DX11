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

class MMeshResources
{
public:
    static void InitializeMeshResources();
    static void UninitializeMeshResources();
public:
    static SMeshData* CreateMeshDataFromFileName(const char*);
    static SMeshData* GetMeshDataFromFileName(const char*);
private:
    static bool TryToImportOBJ(const char*, SMeshData*);
};
