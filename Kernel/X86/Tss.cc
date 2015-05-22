#include <X86/Tss.hh>

static x86_tss tss;

void
x86_tss_init()
{
   auto tssd = x86_tssd_get();
   x86_tssd_init(tssd, &tss);
   x86_tssd_update();
}

void
x86_tss_set_kstack(uint32_t esp)
{
   tss.ss0 = 16;
   tss.esp0 = esp;
}

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
