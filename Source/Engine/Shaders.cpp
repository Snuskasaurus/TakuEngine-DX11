#include "Shaders.h"

#include <d3dcompiler.h>
#include <intsafe.h>

#include "HResultHandler.h"


void CompileShader(LPCWSTR FileName, const char* Target, ID3DBlob** ShaderOut)
{
    UINT flags1 = 0;
    UINT flags2 = 0;

    flags1 |= D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR;
    
#ifdef _DEBUG
    flags1 |= D3DCOMPILE_DEBUG;
#endif
    
    // const D3D_SHADER_MACRO defines[] = 
    // {
    //     "EXAMPLE_DEFINE", "1",
    //     NULL, NULL
    // };
    
    ID3DBlob* ErrorBlob = nullptr;
    
    HRESULT HResult = D3DCompileFromFile(FileName, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE,
                                     "Main", Target,
                                     flags1, flags2, ShaderOut, &ErrorBlob);
    if (HResult != 0)
    {
        const char* errorMsg = (const char*)ErrorBlob->GetBufferPointer();
        
        MessageBoxA(nullptr, errorMsg, "Shader Compilation Error", MNC_IGNORE);
    }
}
