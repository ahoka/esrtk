#ifndef TSS_HH
#define TSS_HH

struct x86_tss
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

   void
   init(uint32_t base, uint32_t size)
   {
      limit_low = size & 0xffff;
      base_address_low = base & 0xffff;
      base_address_mid = (base >> 16) & 0xff;
      type = 0b1001;
      zero_0 = 0;
      privilege_level = 0;
      present = 1;
      limit_high = (size >> 16) & 0xf;
      available = 0;
      zero_1 = 0;
      zero_2 = 0;
      granuality = 0;
      base_high = (base >> 24) & 0xff;
   }
} __attribute__((packed));

#endif
