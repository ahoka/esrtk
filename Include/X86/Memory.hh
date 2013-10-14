#ifndef X86_MEMORY_HH
#define X86_MEMORY_HH

#include <Multiboot.hh>
#include <X86/Idt.hh>
#include <X86/Parameters.hh>

extern void* __start_text;
extern void* __end_text;
extern void* __start_rodata;
extern void* __end_rodata;
extern void* __start_data;
extern void* __end_data;
extern void* __start_bss;
extern void* __end_bss;
extern void* __end_kernel;

#endif
