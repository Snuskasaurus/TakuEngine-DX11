#pragma once

#include <d3dcommon.h>
#include <Windows.h>

#define CHECK_HRESULT(func) HandleHResultError(__LINE__, __FILE__, func)

bool HandleHResultError(const int line, const char* filename, HRESULT HResult) noexcept;
