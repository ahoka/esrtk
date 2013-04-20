#include <Idt.h>

#define NUMIDT 256

// Idt descriptors
idtEntry idtEntries[NUMIDT];

extern "C" void
initIsr(int n, void* fn)
{
      idtEntries[n].baseLow = 0;
      idtEntries[n].baseHigh = 0;

      idtEntries[n].selector = 0x08;
      idtEntries[n].flags = 0x8e;

      // zero filled
      idtEntries[n].reserved = 0;
}
