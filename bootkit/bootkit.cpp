#include "global.hpp"
#include "ebs.hpp"
#include "virtual_address_map_handler.hpp"


EFI_STATUS EFIAPI CuteMain(EFI_HANDLE image_handle, EFI_SYSTEM_TABLE* efi_system_table)
{
    globals::system_table = efi_system_table;

    ebs::create_hook();
    va_map_handler::create_handler();

    return EFI_SUCCESS;
}