#pragma once
#include "tools.hpp"
#include "hook.hpp"
namespace va_map_handler
{
    EFI_EVENT set_va_map_handler_ptr = 0;
    void EFIAPI set_va_map_handler(EFI_EVENT event, void* ctx)
    {
        globals::system_table->RuntimeServices->ConvertPointer(0, (void**)&globals::hook_address);
        globals::system_table->RuntimeServices->ConvertPointer(0, (void**)&globals::function_ptr);
           
        hook::create_hook(globals::function_ptr, globals::hook_address);

        return;
    }

    void create_handler()
    {
        globals::system_table->BootServices->CreateEvent(EVT_SIGNAL_VIRTUAL_ADDRESS_CHANGE, TPL_NOTIFY, set_va_map_handler, 0, &set_va_map_handler_ptr);
    }
}