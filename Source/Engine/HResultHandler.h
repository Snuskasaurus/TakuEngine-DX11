#pragma once

#include <Windows.h>

#define CHECK_HRESULT(func) HandleHResultError(__LINE__, __FILE__, func);

void HandleHResultError(const int line, const char* file, HRESULT HResult) noexcept;
