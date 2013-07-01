#ifdef __i386__

#include <VgaConsole.hh>
#include <SerialConsole.hh>
#include <Apic.hh>

namespace System {
   VgaConsole console;
}

extern "C" void initGdt();
extern "C" void initInterrupts();

extern "C" void
initConsole()
{
  //   System::console.init();
  // System::console.clearScreen();
}

extern "C" void
initSupervisor()
{
   //  initGdt();
   initInterrupts();

   apic.init();
}

#endif
