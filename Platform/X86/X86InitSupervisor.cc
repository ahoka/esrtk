#ifdef __i386__

#include <VgaConsole.hh>
#include <Apic.hh>

namespace System {
   VgaConsole console;
}

extern "C" void initGdt();
extern "C" void initInterrupts();

extern "C" void
initSupervisor()
{
   //  initGdt();
   initInterrupts();

   apic.init();
}

#endif
