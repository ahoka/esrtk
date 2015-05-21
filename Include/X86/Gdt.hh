#ifndef __GDT_H__
#define __GDT_H__

extern "C" void x86_gdt_init();
extern "C" uint32_t x86_gdt_get();
extern "C" void x86_tss_load();
extern "C" uint32_t x86_tss_get();

struct x86_tss
{
   
};

#endif
