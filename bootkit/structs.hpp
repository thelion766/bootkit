#pragma once

extern "C"
{
    #include <Uefi.h>
    #include <Library/UefiLib.h>
    #include <Library/UefiBootServicesTableLib.h>
}


//0x10 bytes (sizeof)
struct unicode_string_t
{
    unsigned short length;                                                          //0x0
    unsigned short max_length;                                                   //0x2
    CHAR16* buffer;                                                          //0x8
}; 

struct kldr_data_table_entry_t
{
    struct _LIST_ENTRY in_load_order_links;                        //0x0
    void* exception_table;                                          //0x10
    unsigned long exception_table_size;                             //0x18
    void* gp_value;                                                 //0x20
    struct _NON_PAGED_DEBUG_INFO* non_paged_debug_info;            //0x28
    void* dll_base;                                                  //0x30
    void* entry_point;                                              //0x38
    unsigned long size_of_image;                                    //0x40
    struct unicode_string_t full_dll_name;                          //0x48
    struct unicode_string_t base_dll_name;                          //0x58
    unsigned long flags;                                            //0x68
    unsigned short load_count;                                      //0x6c
    union
    {
        unsigned short signature_level:4;                           //0x6e
        unsigned short signature_type:3;                            //0x6e
        unsigned short unused:9;                                    //0x6e
        unsigned short entire_field;                                //0x6e
    } u1;                                                           //0x6e
    void* section_pointer;                                          //0x70
    unsigned long check_sum;                                        //0x78
    unsigned long coverage_section_size;                            //0x7c
    void* coverage_section;                                         //0x80
    void* loaded_imports;                                           //0x88
    void* spare;                                                    //0x90
    unsigned long size_of_image_not_rounded;                        //0x98
    unsigned long time_date_stamp;                                  //0x9c
};

//0x160 bytes (sizeof)
struct loader_parameter_block_t
{
    unsigned long os_major_version;                                                   //0x0
    unsigned long os_minor_version;                                                   //0x4
    unsigned long size;                                                             //0x8
    unsigned long os_loader_security_version;                                          //0xc
    struct _LIST_ENTRY load_order_list_head;                                   //0x10
    struct _LIST_ENTRY memory_descriptor_list_head;                            //0x20
    struct _LIST_ENTRY boot_driver_list_head;                                  //0x30
    struct _LIST_ENTRY elam_list_head;                                 //0x40
    struct _LIST_ENTRY core_driver_list_head;                                  //0x50
    struct _LIST_ENTRY core_extensions_driver_list_head;                        //0x60
    struct _LIST_ENTRY tpm_core_driver_list_head;                               //0x70
    unsigned long long kernel_stack;                                                  //0x80
    unsigned long long prcb;                                                         //0x88
    unsigned long long process;                                                      //0x90
    unsigned long long thread;                                                       //0x98
    unsigned long kstack_size;                                                  //0xa0
    unsigned long registry_len;                                                   //0xa4
    void* registry_base;                                                     //0xa8
    struct _CONFIGURATION_COMPONENT_DATA* configuration_root;                //0xb0
    char* arc_boot_device_name;                                                //0xb8
    char* arc_hal_device_name;                                                 //0xc0
    char* nt_boot_path_name;                                                   //0xc8
    char* nt_hal_path_name;                                                    //0xd0
    char* load_options;                                                      //0xd8
    struct _NLS_DATA_BLOCK* nls_data;                                        //0xe0
    struct _ARC_DISK_INFORMATION* arc_disk_info;                       //0xe8
    struct _LOADER_PARAMETER_EXTENSION* extension;                          //0xf0

    char* os_boot_stat_path_name;                                               //0x148
    char* arc_os_data_device_name;                                              //0x150
    char* arc_windows_sys_part_name;                                            //0x158
}; 


