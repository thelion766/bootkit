#pragma once

#include "memory.hpp"
namespace hook_handler
{
    typedef struct cmd_t
    {
        enum operations : int
        {
            write_physical = 0x0,
            read_physical = 0x1,
            get_cr3 = 0x2,
            get_base = 0x3
        };

        int operation;
        uint64_t address;
        uint64_t value;
        uint64_t pid;
        uint64_t magic;
        uint64_t buffer;
        uint64_t size;

        uint64_t base;
        uint64_t cr3;
    };

    typedef long(*nt_acquire_process_activity_reference_t)(int64_t, void*, int);

    
    long nt_acquire_process_activity_reference_hook(int64_t a1, void* a2, int a3)
    {
        if (!a1)
            return 0x1;
        
        cmd_t* cmd = (cmd_t*)a1;

        if (!cmd)
            return 0x1;

        if (cmd->magic != 0x12E7A12D) //thanks sleepy
            return 0x1;
        
        switch (cmd->operation)
        {
            case cmd_t::operations::write_physical:
                memory::write_physical(cmd->address, cmd->size, cmd->value);
                break;

            case cmd_t::operations::read_physical:
                memory::read_physical((void*)cmd->buffer, cmd->address, cmd->size);
                break;

            case cmd_t::operations::get_cr3:
                cmd->cr3 = memory::get_cr3(cmd->pid);
                break;

            case cmd_t::operations::get_base:
                cmd->base = memory::get_base(cmd->pid);
                break;
        }

        return 0x1;
    }
}