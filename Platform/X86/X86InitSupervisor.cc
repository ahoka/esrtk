#ifdef __i386__

#include <VgaConsole.hh>
#include <SerialConsole.hh>
#include <X86/Apic.hh>
#include <X86/PageDirectory.hh>

// namespace System {
//    VgaConsole console;
// }

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
   PageDirectory::init();
   apic.init();
}

#endif
