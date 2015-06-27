#include <Kernel/ProcessContext.hh>

#include <X86/Gdt.hh>
#include <X86/Tss.hh>

#include <X86/PageDirectory.hh>
#include <X86/Processor.hh>

#include <Debug.hh>

using namespace Kernel;

void
ProcessContext::init()
{
   x86_gdt_init();
   x86_tss_init();
}

void
ProcessContext::setKernelStack(uintptr_t stack)
{
   x86_tss_set_kstack(stack);
}

ProcessContext::ProcessContext()
   : pageDirectoryM(PageDirectory::createPageDirectory())
{
   Debug::info("Creating ProcessContext: NEW PD: %p\n", (void*)pageDirectoryM);
}

ProcessContext::ProcessContext(uintptr_t pd)
   : pageDirectoryM(pd)
{
   Debug::info("Creating ProcessContext: EXISTING PD: %p\n", (void*)pageDirectoryM);
}

void
ProcessContext::switchContext()
{
   printf("Switching PD to %p\n", (void*)pageDirectoryM);
   uint32_t flags = x86_get_eflags();
   x86_cli();
   uint32_t cr3 = x86_get_cr3();
   printf("Old CR3: %p\n", (void*)cr3);
   x86_set_cr3(pageDirectoryM);
   x86_set_eflags(flags);
   printf("Switch OK\n");
}
