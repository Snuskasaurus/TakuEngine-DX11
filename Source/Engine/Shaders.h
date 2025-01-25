#pragma once

#include <Windows.h>
#include <d3d11.h>

void CompileShader(LPCWSTR pFileName, const char* Target, ID3DBlob** ShaderOut);
