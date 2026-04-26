#pragma once

extern "C" {
    #include <Uefi.h>
    #include <Library/UefiLib.h>
}

namespace log
{
    template<typename... Args>
    void print(const wchar_t* fmt, Args... args)
    {
        Print((const CHAR16*)L"[ Twizzy Bootkit ] ");
        Print((const CHAR16*)fmt, args...);
    }
}