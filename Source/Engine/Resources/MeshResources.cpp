#include "MeshResources.h"

#include "../IncludesExternal.h"

#include "ShadersResources.h"

#define GAME_DATA_PATH "Data/"
#define TRIGGER_ERROR() assert(false) // TODO Julien Rogel (06/11/2024): Replace with MessageBox

// TODO Julien Rogel (11/02/2025): Change this data structure to use an array and avoid allocation for each mesh data
static std::map<std::string, SMeshData*> G_MESH_DATA_MAP;

struct SFaceInfo
{
    TVertexIndex GeometryIndex, TextureIndex, NormalIndex;
    void DecrementValues() {GeometryIndex -= 1; TextureIndex -= 1; NormalIndex -= 1; } // OBJ file have VertexIndex that start at 1 instead of 0
    bool operator==(const SFaceInfo& other) const 
    {
        return GeometryIndex == other.GeometryIndex && TextureIndex == other.TextureIndex && NormalIndex == other.NormalIndex;
    }
};

static std::vector<TVector3f> BufferVertexPosition;
static std::vector<TVector3f> BufferVertexNormals;
static std::vector<TVector2f> BufferTextureCoordinates;
static std::vector<SFaceInfo> BufferFaceInfos;
static std::vector<SFaceInfo> BufferUniqueFaceInfos;

void MMeshResources::InitializeMeshResources()
{
    BufferVertexPosition.reserve(1024);
    BufferVertexNormals.reserve(1024);
    BufferTextureCoordinates.reserve(1024);
    BufferFaceInfos.reserve(4024);
    BufferUniqueFaceInfos.reserve(4024);
}

void MMeshResources::UninitializeMeshResources()
{
    BufferUniqueFaceInfos.shrink_to_fit();
    BufferFaceInfos.shrink_to_fit();
    BufferTextureCoordinates.shrink_to_fit();
    BufferVertexNormals.shrink_to_fit();
    BufferVertexPosition.shrink_to_fit();
}

SMeshData* MMeshResources::CreateMeshDataFromFileName(const char* _filename)
{
#ifdef _DEBUG
    assert(MMeshResources::GetMeshDataFromFileName(_filename) == nullptr);
#endif
    
    const std::string fileNameAsString = _filename;
    
    SMeshData* NewMeshData = new SMeshData();
    std::stringstream OBJFilenameStream;
    OBJFilenameStream << GAME_DATA_PATH << _filename << ".obj";
    const bool successImporting = MMeshResources::TryToImportOBJ(OBJFilenameStream.str().c_str(), NewMeshData);
    if (successImporting == false)
    {
        delete NewMeshData;
        return nullptr;
    }
    
    G_MESH_DATA_MAP.insert({ fileNameAsString, NewMeshData });
    return NewMeshData;
}

SMeshData* MMeshResources::GetMeshDataFromFileName(const char* _filename)
{
    const std::string fileNameAsString = _filename;
    const auto result = G_MESH_DATA_MAP.find(fileNameAsString);
    if (result == G_MESH_DATA_MAP.end())
        return nullptr;
    return result->second;
}

bool CreateOrGetUniqueFaceIndex(const SFaceInfo& FaceInfo, int* UniqueIndex)
{
    for (size_t i = 0; i < BufferUniqueFaceInfos.size(); ++i)
    {
        const bool IsSameFace = BufferUniqueFaceInfos[i] == FaceInfo;
        if (IsSameFace == true)
        {
            *UniqueIndex = static_cast<int>(i);
            return false;
        }
    }
    BufferUniqueFaceInfos.emplace_back(FaceInfo);
    *UniqueIndex = static_cast<int>(BufferUniqueFaceInfos.size()) - 1;
    return true;
}

bool MMeshResources::TryToImportOBJ(const char* Filename, SMeshData* MeshData)
{
    // Open File
    FILE* fileStream = nullptr;
    const errno_t resultFileOpen = fopen_s(&fileStream, Filename, "r");
    if(resultFileOpen != 0 || fileStream == nullptr)
    {
        TRIGGER_ERROR(); // Failed to open file
        return false;
    }
    
    // Reset the buffers when reading file
    char bufferHeader[64] = {};
    BufferVertexPosition.clear();
    BufferVertexNormals.clear();
    BufferTextureCoordinates.clear();
    BufferFaceInfos.clear();
    BufferUniqueFaceInfos.clear();
    
    // Read file
    while(true)
    {
        // Read the first 
        {
            const int resultHeaderScan = fscanf_s(fileStream, "%s", bufferHeader, (unsigned int)sizeof(bufferHeader));
            if (resultHeaderScan == EOF)
            {
                break;
            }
        }

        // Read the rest of line depending on the header
        if (strcmp(bufferHeader, "v") == 0) // Read vertex position
        {
            TVector3f VertexPosition;
            const int matches = fscanf_s(fileStream, "%f %f %f\n",
                &VertexPosition.x,
                &VertexPosition.y,
                &VertexPosition.z);
            if (matches == 3)
            {
                BufferVertexPosition.emplace_back(VertexPosition);
            }
            else
            {
                TRIGGER_ERROR();
                return false;
            }
        }
        else if (strcmp(bufferHeader, "vn") == 0) // Read vertex normals
        {
            TVector3f VertexNormal;
            const int matches = fscanf_s(fileStream, "%f %f %f\n",
                &VertexNormal.x,
                &VertexNormal.y,
                &VertexNormal.z);
            if (matches == 3)
            {
                BufferVertexNormals.emplace_back(VertexNormal);
            }
            else
            {
                TRIGGER_ERROR();
                return false;
            }
        }
        else if (strcmp(bufferHeader, "vt") == 0) // Read vertex texture coordinates
        {
            TVector2f TextureCoordinate;
            const int matches = fscanf_s(fileStream, "%f %f\n",
                &TextureCoordinate.x,
                &TextureCoordinate.y);
            if (matches == 2)
            {
                TextureCoordinate.y = 1.0f - TextureCoordinate.y; // Convert to RH
                BufferTextureCoordinates.emplace_back(TextureCoordinate);
            }
            else
            {
                TRIGGER_ERROR();
                return false;
            }
        }
        else if (strcmp(bufferHeader, "f") == 0) // Read faces information
        {
            SFaceInfo faceInfo0, faceInfo1, faceInfo2;
            const int matches = fscanf_s(fileStream, "%hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu\n",
                  &faceInfo0.GeometryIndex, &faceInfo0.TextureIndex, &faceInfo0.NormalIndex,
                  &faceInfo1.GeometryIndex, &faceInfo1.TextureIndex, &faceInfo1.NormalIndex,
                  &faceInfo2.GeometryIndex, &faceInfo2.TextureIndex, &faceInfo2.NormalIndex);
            
            if (matches == 9)
            {
                faceInfo0.DecrementValues();
                BufferFaceInfos.emplace_back(faceInfo0);
                faceInfo1.DecrementValues();
                BufferFaceInfos.emplace_back(faceInfo1);
                faceInfo2.DecrementValues();
                BufferFaceInfos.emplace_back(faceInfo2);
            }
            else
            {
                TRIGGER_ERROR(); // TODO Julien Rogel (06/11/2024): Need to re-write this parser if it happens
                return false;
            }
        }
    }

    // Close file
    const int FailedToCloseFile = fclose(fileStream);
    if (FailedToCloseFile)
    {
        TRIGGER_ERROR();
        return false;
    }
    
    assert(BufferFaceInfos.size() > 1);
    assert(BufferTextureCoordinates.size() > 1);
    assert(BufferVertexNormals.size() > 1);
    assert(BufferVertexPosition.size() > 1);
    
    // Filter uniques faces because OBJ got duplicates of faces and fill the Mesh Data
    BufferUniqueFaceInfos.reserve(BufferFaceInfos.size());
    for (size_t i = 0; i < BufferFaceInfos.size(); ++i)
    {
        SFaceInfo FaceInfo = BufferFaceInfos[i];
        int UniqueFaceIndex;
        const bool IsNewFaceIndex = CreateOrGetUniqueFaceIndex(FaceInfo, &UniqueFaceIndex);
        if (IsNewFaceIndex)
        {
            SVertex NewVertex =
                {
                    BufferVertexPosition[FaceInfo.GeometryIndex],
                    BufferTextureCoordinates[FaceInfo.TextureIndex],
                    BufferVertexNormals[FaceInfo.NormalIndex]
                };
            MeshData->VertexBuffer.emplace_back(NewVertex);
        }
        MeshData->IndexBuffer.emplace_back(UniqueFaceIndex);
    }

    MeshData->VertexBuffer_ByteWidth = SMeshData::VertexBuffer_StructureByteStride * static_cast<UINT>(MeshData->VertexBuffer.size());
    MeshData->IndexBuffer_ByteWidth = SMeshData::IndexBuffer_StructureByteStride * static_cast<UINT>(MeshData->IndexBuffer.size());
    MeshData->IndexCount = static_cast<UINT>(MeshData->IndexBuffer.size());

    return true;
}
