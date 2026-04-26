#pragma once

extern "C"
{
    #include <Uefi.h>
    #include <Library/UefiLib.h>
    #include <Library/UefiBootServicesTableLib.h>
}

#include "cstdint"

namespace globals
{
    EFI_SYSTEM_TABLE* system_table = 0;
    uint64_t winload_base = 0;
    uint64_t function_ptr = 0;
    uint64_t hook_address = 0;
    uint64_t ntoskrnl = 0;
}