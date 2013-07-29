#include <Supervisor.hh>
#include <X86/Gdt.hh>

extern "C" void kmain(void);

void
kmain(void)
{
   extern unsigned int magic;
   //	extern void *mbd;
 
   if (magic != 0x2BADB002) {
      return;
   }
 
   /* You could either use multiboot.h */
   /* (http://www.gnu.org/software/grub/manual/multiboot/multiboot.html#multiboot_002eh) */
   /* or do your offsets yourself. The following is merely an example. */
   //char * boot_loader_name =(char*) ((long*)mbd)[16];

   Supervisor supervisor;
   supervisor.run();
 
   /* Print a letter to screen to see everything is working: */
//	unsigned char *vram = (unsigned char *) 0xb8000;
//
//	for (int i = 0; i < (80*25 - 1) * 2; i += 2) {
//		vram[i] = 65;
//		vram[i+1] = 0x07;
//	}
//	vram[0] = 65; /* character 'A' */
//	vram[1] = 0x07; /* light grey (7) on black (0). */
}
