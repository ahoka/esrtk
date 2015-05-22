#ifndef TSS_HH
#define TSS_HH

#include <stdint.h>

struct x86_tss_descriptor
{
   unsigned int limit_low : 16;
   unsigned int base_address_low : 16;
   unsigned int base_address_mid : 8;
   unsigned int type : 4;
   unsigned int zero_0 : 1;
   unsigned int privilege_level : 2;
   unsigned int present : 1;
   unsigned int limit_high : 4;
   unsigned int available : 1;
   unsigned int zero_1 : 1;
   unsigned int zero_2 : 1;
   unsigned int granuality : 1;
   unsigned int base_high : 8;
} __attribute__((packed));

struct x86_tss
{
   uint16_t link;
   uint16_t reserved_0;
   uint32_t esp0;
   uint16_t ss0;
   uint16_t reserved_1;
   uint32_t esp1;
   uint16_t ss1;
   uint16_t reserved_2;
   uint32_t esp2;
   uint16_t ss2;
   uint16_t reserved_3;
   uint32_t cr3;
   uint32_t eip;
   uint32_t eflags;
   uint32_t eax;
   uint32_t ecx;
   uint32_t edx;
   uint32_t ebx;
   uint32_t esp;
   uint32_t ebp;
   uint32_t esi;
   uint32_t edi;
   uint16_t es;
   uint16_t reserved_4;
   uint16_t cs;
   uint16_t reserved_5;
   uint16_t ss;
   uint16_t reserved_6;
   uint16_t ds;
   uint16_t reserved_7;
   uint16_t fs;
   uint16_t reserved_8;
   uint16_t gs;
   uint16_t reserved_9;
   uint32_t ldt;
   uint16_t trap;
   uint16_t iomap;
} __attribute__((packed));

void
x86_tssd_init(struct x86_tss_descriptor* d, struct x86_tss* tss)
{
   uint32_t size = sizeof(struct x86_tss);
   uint32_t base = (uint32_t)tss;

   d->limit_low = size & 0xffff;
   d->base_address_low = base & 0xffff;
   d->base_address_mid = (base >> 16) & 0xff;
   d->type = 0b1001;
   d->zero_0 = 0;
   d->privilege_level = 0;
   d->present = 1;
   d->limit_high = (size >> 16) & 0xf;
   d->available = 0;
   d->zero_1 = 0;
   d->zero_2 = 0;
   d->granuality = 0;
   d->base_high = (base >> 24) & 0xff;
}

extern "C" void x86_tssd_update();
extern "C" struct x86_tss_descriptor* x86_tssd_get();

#endif
