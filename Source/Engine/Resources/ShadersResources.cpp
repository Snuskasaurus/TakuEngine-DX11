#include "ShadersResources.h"

#include <d3dcompiler.h>
#include <intsafe.h>

#include "GamePath.h"
#include "../HResultHandler.h"

// TODO Julien Rogel (20/02/2025): Change this data structure to use an array and avoid allocation for each shader data
static std::map<std::string, ID3DBlob*> G_BLOB_MAP;

ID3DBlob* MShaderResources::CreateBlobFromFileName(const char* _filename, EShaderType _shaderType)
{
    ID3DBlob* NewBlob = nullptr;
    const std::string fileNameAsString = _filename;
    std::stringstream OBJFilenameStream;
    OBJFilenameStream << GAME_DATA_SHADER_PATH << _filename << ".hlsl";

    std::string target;
    switch (_shaderType)
    {
        case VERTEX_SHADER: target = "vs_5_0"; break;
        case PIXEL_SHADER: target = "ps_5_0"; break;
    }

    MShaderResources::CompileShader(OBJFilenameStream.str().c_str(), target.c_str(), &NewBlob);
    G_BLOB_MAP.insert({ fileNameAsString, NewBlob });
    return NewBlob;
}

ID3DBlob* MShaderResources::GetBlobFromFileName(const char* _filename)
{
    const std::string fileNameAsString = _filename;
    const auto result = G_BLOB_MAP.find(fileNameAsString);
    if (result == G_BLOB_MAP.end())
        return nullptr;
    return result->second;
}

void MShaderResources::DeleteAllBlobs()
{
    for (auto gBlobMap : G_BLOB_MAP)
    {
        gBlobMap.second->Release();
        delete gBlobMap.second;
    }
    bool success = G_BLOB_MAP.empty();
}

void MShaderResources::CompileShader(const char* _fileName, const char* _target, ID3DBlob** _shaderOut)
{
    UINT flags1 = 0;
    UINT flags2 = 0;

    flags1 |= D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
    
#ifdef _DEBUG
    flags1 |= D3DCOMPILE_DEBUG;
#endif
    
    ID3DBlob* ErrorBlob = nullptr;
    
    int wLen = MultiByteToWideChar(CP_ACP, 0, _fileName, -1, nullptr, 0);
    std::wstring wideFileName(wLen, 0);
    MultiByteToWideChar(CP_ACP, 0, _fileName, -1, &wideFileName[0], wLen);
    
    HRESULT HResult = D3DCompileFromFile(wideFileName.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                     "Main", _target,
                                     flags1, flags2, _shaderOut, &ErrorBlob);
    if (HResult != 0)
    {
        const char* errorMsg = (const char*)ErrorBlob->GetBufferPointer();
        MessageBoxA(nullptr, errorMsg, "Shader Compilation Error", MNC_IGNORE);
    }
}
