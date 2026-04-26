#pragma once
#include "tools.hpp"

namespace hook
{
    void create_hook(uint64_t function_ptr, uint64_t hook_handler)
    {
        uint8_t shellcode[12] = {
            0x48, 0xB8,// mov rax, imm64
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, // address
            0xFF, 0xE0// jmp rax
        };



        CopyMem(shellcode + 2, (uint8_t*)&hook_handler, 8);
        CopyMem((uint8_t*)function_ptr, shellcode, sizeof(shellcode));
        
    }
}