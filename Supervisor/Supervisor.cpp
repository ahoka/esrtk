/* 
 * Main class for the supervisor
 */

#include "Supervisor.h"
#include "Debug.h"
#include <Assembly.h>
#include <stdio.h>
#include <Pci.h>
#include <System.h>
#include <IoPort.h>

Supervisor::Supervisor()
{
}

Supervisor::Supervisor(const Supervisor& /*orig*/)
{
}

Supervisor::~Supervisor()
{
}

void
Supervisor::run()
{
//   VgaConsole console;
  
   printf("Supervisor started\n\n");

#if 0

   const char* s = "Hello World!";
   unsigned int uint = 123456789u;
   unsigned short ushort = 12345;
   unsigned char uchar = 213;
   unsigned int ulong = 123456789ul;

   int sint = -123456789;
   short sshort = -12345;
   char schar = -123;
   int slong = -123456789l;
   
   unsigned long long ulonglong = 123456789123456789ull;

   console.printf("uint:\t\t%u\n", 0);
   console.printf("uint:\t\t%u\n", uint);
   console.printf("ushort:\t\t%hu\n", ushort);
   console.printf("uchar:\t\t%hhu\n", uchar);
   console.printf("ulong:\t\t%lu\n\n", ulong);

   console.printf("sint:\t\t%d\n", 0);
   console.printf("sint:\t\t%d\n", sint);
   console.printf("sshort:\t\t%hd\n", sshort);
   console.printf("schar:\t\t%hhd\n", schar);
   console.printf("slong:\t\t%ld\n\n", slong);

   console.printf("uint:\t\t0x%x\n", 0);
   console.printf("uint:\t\t0x%x\n", uint);
   console.printf("ushort:\t\t0x%hx\n", ushort);
   console.printf("uchar:\t\t0x%hhx\n", uchar);
   console.printf("ulong:\t\t0x%lx\n", ulong);
#if 0
   console.printf("uint:\t\t0X%X\n", 0);
   console.printf("uint:\t\t0X%X\n", uint);
   console.printf("ushort:\t\t0X%hX\n", ushort);
   console.printf("uchar:\t\t0X%hhX\n", uchar);
   console.printf("ulong:\t\t0X%lX\n", ulong);
#endif

   console.printf("string: %s\n", s);

   console.printf("%c%c%c", 'a', '3', '\n');

   console.printf("uint:\t\t0%o\n", 0);
   console.printf("uint:\t\t0%o\n", uint);
   console.printf("ushort:\t\t0%ho\n", ushort);
   console.printf("uchar:\t\t0%hho\n", uchar);
   console.printf("ulong:\t\t0%lo\n", ulong);

   console.printf("ptr:\t\t%p\n", (void *)s);

   console.printf("%+d\n", sint);
   console.printf("%+d\n", uint);
   console.printf("% d\n", sint);
   console.printf("% d\n", uint);

   console.printf("%#x\n", 12346);
   console.printf("%#x\n", 0);
   console.printf("%#o\n", 12346);
   console.printf("%#o\n", 0);
#endif
   char id[13];
   uint32_t level = cpuid0(id);

   printf("CPU Vendor ID: %s, Largest Standard Function: %d\n",
	  id, level);
   
   // uint32_t pciid = Pci::readConfigurationRegister32(0, 0, 0, 0);
//   uint32_t v;
//   uint32_t d;
   
//   v = Pci::getDeviceId(0, 0, 0);
//   d = Pci::getVendorId(0, 0, 0);

//   printf("Vendor ID: 0x%x\n", v);
//   printf("Device ID: 0x%x\n", d);

   // v = Pci::getVendorId(0, 1, 0);
   // d = Pci::getDeviceId(0, 1, 0);

   // printf("Vendor ID: 0x%x\n", v);
   // printf("Device ID: 0x%x\n", d);

#if 1
#define	PCI_MODE1_ENABLE	0x80000000UL
#define	PCI_MODE1_ADDRESS_REG	0x0cf8
#define	PCI_MODE1_DATA_REG	0x0cfc

   outl(PCI_MODE1_ADDRESS_REG, PCI_MODE1_ENABLE);
   outb(PCI_MODE1_ADDRESS_REG + 3, 0);
   outw(PCI_MODE1_ADDRESS_REG + 2, 0);
   uint32_t val = inl(PCI_MODE1_ADDRESS_REG);
   if ((val & 0x80fffffc) != PCI_MODE1_ENABLE) {
      printf("not mode1?\n");
   } else {
      printf("PCI OK.\n");
   }
   
   printf("\nListing PCI devices on system:\n");
   for (int bus = 0; bus < 256; bus++)
   {
      for (int dev = 0; dev < 32; dev++)
      {
	 for (int fun = 0; fun < 8; fun++)
	 {
	 
	    int vid = Pci::getVendorId(bus, dev, fun);
	    if (vid != 0xffff)
	    {
	       int did = Pci::getDeviceId(bus, dev, fun);
	       printf("PCI%d: D%d:F%d: 0x%x:0x%x\n", bus, dev, fun, vid, did);
	    }
	 }
      }
   }
#endif

   // printf("Vendor/Device ID: 0x%x\n", pciid);
   // printf("Vendor ID: 0x%x\n", vendor);
   // printf("Device ID: 0x%x\n", device);

   //   int eflags = getEflags();
   //   printf("eflags: 0x%x\n", eflags);

   printf("\n\nCPU will now halt.\n");
   for (;;)
   {
      asm volatile("hlt");
   }

   //   KASSERT(1 == 1);
}
