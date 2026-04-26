#include "tools.hpp"


namespace memory
{
    #define PAGE_MASK_4KB 0x000FFFFFFFFFF000ULL
    #define PAGE_MASK_2MB 0x000FFFFFFFE00000ULL
    #define PAGE_MASK_1GB 0x000FFFFFC0000000ULL

    #define PAGE_OFFSET_4KB 0x0000000000000FFFULL 
    #define PAGE_OFFSET_2MB 0x00000000001FFFFFULL
    #define PAGE_OFFSET_1GB 0x000000003FFFFFFFULL


    void read_physical(void* buffer, uint64_t address, uint64_t size)
    {
        if (!buffer || !address || !size)
            return;

        typedef int64_t(__fastcall* mm_copy_memory_t)(uint64_t, uint64_t, uint64_t, uint32_t, uint64_t*);

        mm_copy_memory_t mm_copy = (mm_copy_memory_t)((uint64_t)globals::ntoskrnl + 0x030C030);

        uint64_t transferred = 0;
        mm_copy((uint64_t)buffer, address, size, 0x1, &transferred);
    }

    bool mm_is_address_valid(void* a1)
    {
        typedef char (*mm_is_address_valid_t)(void* a1);
        uint64_t ptr = (uint64_t)globals::ntoskrnl + 0x028CB70;

        mm_is_address_valid_t valid = (mm_is_address_valid_t)ptr;

        return valid(a1);
    }

    void write_physical(uint64_t address, uint64_t size, uint64_t value)
    {
        if (!address || !size || !value)
            return;

        typedef uint64_t(__fastcall* mm_map_io_space_ex_t)(uint64_t physical_address, uint64_t size, uint32_t protect);

        
        uint64_t mm_map_io_space = (uint64_t)globals::ntoskrnl + 0x02E7FA0;

        mm_map_io_space_ex_t map_io = (mm_map_io_space_ex_t)mm_map_io_space;

        uint64_t mapped = map_io(address, size, 0x04);

        CopyMem((uint8_t*)mapped, (uint8_t*)value, size);
    }

    uint64_t get_base(uint64_t pid)
    {
        typedef long (*ps_lookup_proces_by_pid_t)(void* a1, void* a2);

        uint64_t ps_lookup = (uint64_t)globals::ntoskrnl + 0x0625CA0;

        ps_lookup_proces_by_pid_t lookup = (ps_lookup_proces_by_pid_t)ps_lookup;

        uint64_t eprocess = 0;
        lookup((void*)pid, &eprocess);

        if (!eprocess)
            return 0;

        typedef uint64_t (*ps_get_process_section_base_address_t)(int64_t a1);

        uint64_t get_base_ptr = (uint64_t)globals::ntoskrnl + 0x024FB50;

        ps_get_process_section_base_address_t ps_get_base = (ps_get_process_section_base_address_t)get_base_ptr;

        return ps_get_base(eprocess);
    }

    uint64_t translate_virtual(uint64_t va, uint64_t cr3)
	{
        if (!va || !cr3)
            return 0;
        uint64_t pml4_idx = (va >> 39) & 0x1FF;
        uint64_t pdpt_idx = (va >> 30) & 0x1FF;
        uint64_t pd_idx = (va >> 21) & 0x1FF;
        uint64_t pt_idx = (va >> 12) & 0x1FF;
        uint64_t page = va & 0xFFF;
        uint64_t pml4_base = cr3 & PAGE_MASK_4KB;
        uint64_t pml4e_raw = pml4_base + (pml4_idx * 8);
        uint64_t pml4e = 0;
        read_physical(&pml4e, pml4e_raw, sizeof(uint64_t));
        if (!(pml4e & 1))
            return 0;
        uint64_t pdpt_base = pml4e & PAGE_MASK_4KB;
        uint64_t pdpte_raw = pdpt_base + (pdpt_idx * 8);
        uint64_t pdpte = 0;
        read_physical(&pdpte, pdpte_raw, sizeof(uint64_t));
        if (!(pdpte & 1))
            return 0;
        if (pdpte & (1ULL << 7))
        {
            uint64_t physical = (pdpte & PAGE_MASK_1GB) | (va & PAGE_OFFSET_1GB);
            return physical;
        }
        uint64_t pd_base = pdpte & PAGE_MASK_4KB;
        uint64_t pde_raw = pd_base + (pd_idx * 8);
        uint64_t pde = 0;
        read_physical(&pde, pde_raw, sizeof(uint64_t));
        if (!(pde & 1))
            return 0;
        if (pde & (1ULL << 7))
        {
            uint64_t physical = (pde & PAGE_MASK_2MB) | (va & PAGE_OFFSET_2MB);
            return physical;
        }
        uint64_t pt_base = pde & PAGE_MASK_4KB;
        uint64_t pte_raw = pt_base + (pt_idx * 8);
        uint64_t pte = 0;
        read_physical(&pte, pte_raw, sizeof(uint64_t));
        if (!(pte & 1))
            return 0;
        uint64_t physical = (pte & PAGE_MASK_4KB) | (va & PAGE_OFFSET_4KB);
        return physical;
	}

    uint64_t get_cr3(uint64_t pid)
    {
        uint64_t base = get_base(pid);

        if (!base)
            return 0;
        
        typedef uint64_t(*get_physmem_ranges_t)(void);

        uint64_t get_physmem_ranges_ptr = globals::ntoskrnl + 0x07CCC30;

        get_physmem_ranges_t get_ranges = (get_physmem_ranges_t)get_physmem_ranges_ptr;

        uint64_t ranges = get_ranges();

        if (!ranges)
            return 0;

        for (uint32_t i = 0; ; i++)
        {
            uint64_t element_address = ranges + i * 16;
            uint64_t element_base = *(uint64_t*)element_address;
            uint64_t bytes = *(uint64_t*)(element_address + 8);

            if (!element_base && !bytes)
                break;

            if (!element_base || !bytes)
                continue;

            uint64_t physical = element_base;
            uint64_t pages = bytes / 0x1000;

            for (uint64_t tuff = 0; tuff < pages; tuff++, physical += 0x1000)
            {
                uint64_t translate = translate_virtual(base, physical);

                if (!translate)
                    continue;

                uint16_t mz = 0;
                read_physical(&mz, translate, sizeof(uint16_t));

                if (mz == 0x5A4D)
                {
                    return physical;
                }
            }
        }

        return 0;
    }
}