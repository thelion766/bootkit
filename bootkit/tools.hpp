#pragma once
#include "global.hpp"
#include "structs.hpp"

extern "C" 
{
    #include <Library/BaseMemoryLib.h>
}

#define CONTAINING_RECORD(address, type, field) ((type *)( \
                                                  (char*)(address) - \
                                                  (UINT64)(&((type *)0)->field)))

namespace tools
{
    uint64_t get_base_from_address(uint64_t address)
    {
        address &= ~0xfffull;
    
        while (true)
        {
            if (*(uint16_t*)address == 0x5A4D)
                return address;
            address -= 0x1000;
        }
    }

    kldr_data_table_entry_t get_base_from_list(LIST_ENTRY* list, const CHAR16* name)
    {
        for (LIST_ENTRY* entry = list->ForwardLink; entry != list; entry = entry->ForwardLink)
        {
            kldr_data_table_entry_t* module = CONTAINING_RECORD(entry, kldr_data_table_entry_t, in_load_order_links);

            if (module && StrnCmp(name, module->base_dll_name.buffer, module->base_dll_name.length) == 0)
                return *module;
        }

        kldr_data_table_entry_t result;
        SetMem(&result, sizeof(result), 0);
        return result;
    }

}