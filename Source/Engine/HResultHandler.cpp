#include "HResultHandler.h"

#include <system_error>
#include <codecvt>
#include <locale>

#include "IncludesExternal.h"

bool HandleHResultError(const int line, const char* filename, const HRESULT HResult) noexcept
{
    if (HResult == 0)
        return true;

    const std::string message = std::system_category().message(HResult);
    const std::string StringError = message + " (" + std::to_string(static_cast<const unsigned long int>(HResult)) + ")\n" + filename + " at line " + std::to_string(line);
    const char* lpText = StringError.c_str();
    MessageBox(nullptr, lpText,"TakuEngine HResultError Handler" ,MB_OK | MB_ICONEXCLAMATION);
    
    return false;
}
