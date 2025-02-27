#pragma once

#include "../IncludesExternal.h"

#include "../Color.h"
#include "../Math.h"

using TVertexIndex = unsigned short;

struct SVertex
{
    TVector3f position;
    TVector3f normal;
    TVector3f tangent;
    TVector2f uv;
};

struct SMeshData
{
    static constexpr UINT VertexBuffer_StructureByteStride = sizeof(SVertex);
    static constexpr UINT IndexBuffer_StructureByteStride = sizeof(TVertexIndex);
    
    UINT VertexBuffer_Size;
    UINT IndexBuffer_Size;
    
    std::vector<SVertex> VertexBuffer;
    std::vector<TVertexIndex> IndexBuffer;

    UINT IndexCount;

    std::string DebugName;
};

class MMeshResources
{
public:
    static SMeshData* CreateMeshDataFromFileName(const char*);
    static SMeshData* GetMeshDataFromFileName(const char*);
    static SMeshData* GetOrCreateMeshDataFromFileName(const char* _filename);
};
