#include "MeshResources.h"

#include "GamePath.h"
#include "../IncludesExternal.h"

#include "ShadersResources.h"

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

//--------------------------------------------------------------------------------------
SMeshData* TryToImportMesh(const std::string& _filename);
//--------------------------------------------------------------------------------------
TVector3f ConvertAssimpVec3(aiVector3D _vec) { return { _vec.x, _vec.y, _vec.z }; }
//--------------------------------------------------------------------------------------
TVector2f ConvertAssimpVec2(aiVector3D _vec) { return { _vec.x, _vec.y }; }
//--------------------------------------------------------------------------------------
static std::map<std::string, SMeshData*> G_MESH_DATA_MAP;
//--------------------------------------------------------------------------------------
SMeshData* MMeshResources::CreateMeshDataFromFileName(const char* _filename)
{
#ifdef _DEBUG
    assert(MMeshResources::GetMeshDataFromFileName(_filename) == nullptr);
#endif
    
    const std::string fileNameAsString = _filename;
    
    std::stringstream OBJFilenameStream;
    OBJFilenameStream << GAME_DATA_PATH << _filename << ".obj";

    
    SMeshData* NewMeshData = TryToImportMesh(OBJFilenameStream.str());
    if (NewMeshData == nullptr)
    {
        assert(false);
        return nullptr;
    }
    
    // const bool successImporting = MMeshResources::TryToImportOBJ(OBJFilenameStream.str().c_str(), NewMeshData);
    // if (successImporting == false)
    // {
    //     delete NewMeshData;
    //     return nullptr;
    // }
    
    G_MESH_DATA_MAP.insert({ fileNameAsString, NewMeshData });
    return NewMeshData;
}
//--------------------------------------------------------------------------------------
SMeshData* MMeshResources::GetMeshDataFromFileName(const char* _filename)
{
    const std::string fileNameAsString = _filename;
    const auto result = G_MESH_DATA_MAP.find(fileNameAsString);
    if (result == G_MESH_DATA_MAP.end())
        return nullptr;
    return result->second;
}
//--------------------------------------------------------------------------------------
SMeshData* MMeshResources::GetOrCreateMeshDataFromFileName(const char* _filename)
{
    SMeshData* textureData = GetMeshDataFromFileName(_filename);
    if (textureData == nullptr) textureData = CreateMeshDataFromFileName(_filename);
    return textureData;
}
//--------------------------------------------------------------------------------------
SMeshData* TryToImportMesh(const std::string& _filename)
{
    static Assimp::Importer importer;
    const aiScene* assimpScene = importer.ReadFile(_filename,
      aiProcess_FlipUVs                             |
      aiProcess_Triangulate                         |
      aiProcess_JoinIdenticalVertices               |
      aiProcess_GenNormals                          |
      aiProcess_CalcTangentSpace                    |
      aiProcess_SortByPType);

    if (nullptr == assimpScene)
        return nullptr;
    if (assimpScene->mNumMeshes <= 0)
        return nullptr;
    const aiMesh* assimpMesh = assimpScene->mMeshes[0];

    SMeshData* meshData = new SMeshData;

    unsigned int nbVertices = assimpMesh->mNumVertices;
    meshData->VertexBuffer.resize(nbVertices);
    for (unsigned int i = 0; i < nbVertices; ++i)
    {
        meshData->VertexBuffer[i].position = ConvertAssimpVec3(assimpMesh->mVertices[i]);
        meshData->VertexBuffer[i].normal = ConvertAssimpVec3(assimpMesh->mNormals[i]);
        meshData->VertexBuffer[i].tangent = ConvertAssimpVec3(assimpMesh->mTangents[i]);
        
        assert(assimpMesh->mTextureCoords[0] != nullptr);
        meshData->VertexBuffer[i].uv = ConvertAssimpVec2(assimpMesh->mTextureCoords[0][i]); // TODO Julien Rogel (25/02/2025): DANGEROUS !!!
    }

    unsigned int nbIndex = assimpMesh->mNumFaces * 3;
    meshData->IndexBuffer.resize(nbIndex);
    
    for (UINT face = 0; face < assimpMesh->mNumFaces; face++)
    {
        for (UINT idx = 0; idx < 3; idx++)
        {
            meshData->IndexBuffer[face * 3 + idx] = (TVertexIndex)assimpMesh->mFaces[face].mIndices[idx];
        }
    }
    
    meshData->VertexBuffer_ByteWidth = SMeshData::VertexBuffer_StructureByteStride * static_cast<UINT>(meshData->VertexBuffer.size());
    meshData->IndexBuffer_ByteWidth = SMeshData::IndexBuffer_StructureByteStride * static_cast<UINT>(meshData->IndexBuffer.size());
    meshData->IndexCount = static_cast<UINT>(meshData->IndexBuffer.size());
    meshData->DebugName = assimpMesh->mName.C_Str();
    
    return meshData;
}
//--------------------------------------------------------------------------------------