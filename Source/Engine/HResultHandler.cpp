#include "HResultHandler.h"

#include <string>
#include <system_error>
#include <codecvt>
#include <locale>

void HandleHResultError(const int line, const char* filename, const HRESULT HResult) noexcept
{
    if (HResult == 0)
        return;

    size_t size = strlen(filename) + 1;  
    wchar_t* FilenameWideChar = new wchar_t[size]; 
    size_t outSize;
    mbstowcs_s(&outSize, FilenameWideChar, size, filename, size-1);

    std::string message = std::system_category().message(HResult);
    std::wstring StringError = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(message) + L" (" + std::to_wstring((const unsigned long int)HResult) + L")"
    + L"\n" + FilenameWideChar + L" at line " + std::to_wstring(line);
    
    const wchar_t* StringErrorWideChar = StringError.c_str();
    
    delete[] FilenameWideChar;

    MessageBox(nullptr, StringErrorWideChar,L"JuProject HResultError Handler" ,MB_OK | MB_ICONEXCLAMATION);

}
