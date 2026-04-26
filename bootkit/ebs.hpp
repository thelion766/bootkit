#pragma once
#include "hook_handler.hpp"
#include "log.hpp"
#include "tools.hpp"
#include "context.hpp"

namespace ebs
{
    EFI_EXIT_BOOT_SERVICES original_exit_boot_services = 0;

    EFI_STATUS EFIAPI exit_boot_services_hook(EFI_HANDLE image_handle, uint64_t map_key)
    {
        gBS->ExitBootServices = original_exit_boot_services;

        globals::system_table->ConOut->SetAttribute(globals::system_table->ConOut, EFI_TEXT_ATTR(EFI_WHITE, EFI_RED));
        globals::system_table->ConOut->ClearScreen(globals::system_table->ConOut);
        
        uint64_t return_address = (uint64_t)_ReturnAddress(); //winload calls ebs so we walk that bih back to mz header

        uint64_t winload = tools::get_base_from_address(return_address);
        
        globals::winload_base = winload;

        log::print(L"winload.efi -> 0x%llx\n", winload);
        
        uint64_t osl_loader_block = *(uint64_t*)(winload + 0x01F1898);

        log::print(L"OslLoaderBlock -> 0x%llx\n", osl_loader_block);

        loader_parameter_block_t* loader_parameter_block = (loader_parameter_block_t*)(osl_loader_block);
        log::print(L"loader parameter block -> 0x%llx\n", loader_parameter_block); 
        context::switch_context(context::application_context);
        kldr_data_table_entry_t ntoskrnl = tools::get_base_from_list(&loader_parameter_block->load_order_list_head, (const CHAR16*)L"ntoskrnl.exe");
        context::switch_context(context::firmware_context);
        
        log::print(L"ntoskrnl -> 0x%llx\n", ntoskrnl.dll_base);
        
        globals::function_ptr = (uint64_t)ntoskrnl.dll_base + 0x0731600; //NtAcquireProcessActivityReference
        globals::hook_address = (uint64_t)hook_handler::nt_acquire_process_activity_reference_hook;
        globals::ntoskrnl = (uint64_t)ntoskrnl.dll_base;
        
        log::print(L"windows is booting now\n");

        return original_exit_boot_services(image_handle, map_key);
    }

    void create_hook()
    {
        original_exit_boot_services = gBS->ExitBootServices;

        gBS->ExitBootServices = exit_boot_services_hook;

        return;
    }
}