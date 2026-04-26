#pragma once

#include "global.hpp"
typedef int (__fastcall* blp_arch_switch_context)(int);


namespace context
{
    constexpr int application_context = 0;
    constexpr int firmware_context = 1;
    void switch_context(int context)
    {
        uint64_t context_switch_ptr = globals::winload_base + 0x002E01C;

        if (!context_switch_ptr)
        {
            log::print(L"context switch ptr is 0\n");
            return;
        }

        blp_arch_switch_context switch_context = (blp_arch_switch_context)context_switch_ptr;

        switch_context(context);
    }
}