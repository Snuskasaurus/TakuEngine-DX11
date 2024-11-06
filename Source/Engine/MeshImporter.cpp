#include "MeshImporter.h"

// Highly inspired by http://www.opengl-tutorial.org/beginners-tutorials/tutorial-7-model-loading/

#define TRIGGER_ERROR() assert(false) // TODO Julien Rogel (06/11/2024): Replace with MessageBox

#include <assert.h>
#include <cstdio>
#include <cstring>

bool TryToImportMeshInfoFromOBJFile(_In_ const wchar_t* _fileName, _Out_ SMeshInfo* SMeshInfo)
{
    FILE* fileStream = nullptr;

    const errno_t resultFOpen = _wfopen_s(&fileStream, _fileName, L"r");
    if(resultFOpen != 0 || fileStream == nullptr)
    {
        TRIGGER_ERROR(); // Failed to open file
        return false;
    }
    
    int nbVertexPos = 0;
    float3 tempVertexPos[MAX_VERTEX_BUFFER_VERTICES] = {};
    
    int nbTexCoord = 0;
    float2 tempTexCoord[MAX_VERTEX_BUFFER_VERTICES] = {};
    
    int nbFace = 0;
    TVertexIndex tempVertexIndex[MAX_INDEX_BUFFER_INDEXES] = {};
    TVertexIndex tempUVIndex[MAX_INDEX_BUFFER_INDEXES] = {};
    TVertexIndex tempNormalIndex[MAX_INDEX_BUFFER_INDEXES] = {};

    bool ReadingFile = true;
    while(ReadingFile)
    {
        char lineHeader[128] = {};
        const int resultScanfVertexStart = fscanf_s(fileStream, "%s", lineHeader, (unsigned int)sizeof(lineHeader));
        if (resultScanfVertexStart == EOF)
        {
            ReadingFile = false; // EOF = End Of File. Quit the loop.
        }
        
        if (strcmp(lineHeader, "v") == 0) // Read vertex position
        {
            const int matches = fscanf_s(fileStream, "%f %f %f\n",
                &tempVertexPos[nbVertexPos].x,
                &tempVertexPos[nbVertexPos].y,
                &tempVertexPos[nbVertexPos].z);
            nbVertexPos++;
            if (nbVertexPos >= MAX_VERTEX_BUFFER_VERTICES)
            {
                TRIGGER_ERROR();
                return false;
            }
        }
        else if (strcmp(lineHeader, "vt") == 0) // Read texture coordinates
        {
            const int matches = fscanf_s(fileStream, "%f %f\n",
                &tempTexCoord[nbTexCoord].x,
                &tempTexCoord[nbTexCoord].y);
            nbTexCoord++;
        }
        else if (strcmp(lineHeader, "f") == 0) // Read faces informations
        {
            int matches = fscanf_s(fileStream, "%hu/%hu/%hu %hu/%hu/%hu %hu/%hu/%hu\n",
                &tempVertexIndex[nbFace * 3],     &tempUVIndex[nbFace * 3],     &tempNormalIndex[nbFace * 3],
                &tempVertexIndex[nbFace * 3 + 1], &tempUVIndex[nbFace * 3 + 1], &tempNormalIndex[nbFace * 3 + 1],
                &tempVertexIndex[nbFace * 3 + 2], &tempUVIndex[nbFace * 3 + 2], &tempNormalIndex[nbFace * 3 + 2] );
            if (matches != 9)
            {
                TRIGGER_ERROR(); // TODO Julien Rogel (06/11/2024): Need to re-write this parser if it happens
                return false;
            }
            nbFace++;
            if (nbFace * 3 >= MAX_INDEX_BUFFER_INDEXES)
            {
                TRIGGER_ERROR();
                return false;
            }
        }
    }

    int SuccessFClose = fclose(fileStream);
    if (SuccessFClose)
    {
        TRIGGER_ERROR();
        return false;
    }
    
    // Copy vertex buffer
    for (int iVertex = 0; iVertex < nbVertexPos; ++iVertex)
    {
        memcpy(&SMeshInfo->VertexBuffer.Vertices[iVertex].Position, &tempVertexPos[iVertex], sizeof(float3));
        memcpy(&SMeshInfo->VertexBuffer.Vertices[iVertex].TextureCoordinate, &tempTexCoord[iVertex], sizeof(float2));
    }
    SMeshInfo->nbVertexBuffer = nbVertexPos;
    
    // Copy face buffer | Texture Coordinate
    int nbIndex = nbFace * 3;
    size_t sizeFaceBuffer = sizeof(TVertexIndex) * nbIndex;
    memcpy(SMeshInfo->IndexBuffer.Indexes, tempVertexIndex, sizeFaceBuffer);
    for (int iIndexBuffer = 0; iIndexBuffer < nbIndex; ++iIndexBuffer)
    {
        SMeshInfo->IndexBuffer.Indexes[iIndexBuffer] -= 1;
    }
    SMeshInfo->nbIndexBuffer = nbIndex;
    
    return true;
}
