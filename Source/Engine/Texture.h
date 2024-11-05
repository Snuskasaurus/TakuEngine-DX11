#pragma once

#include <d3d11.h>

// https://learn.microsoft.com/en-us/windows/win32/direct3d11/overviews-direct3d-11-resources-textures-how-to

ID3D11Texture2D* CreateDirectXTextureFromFile(const wchar_t* FileName);