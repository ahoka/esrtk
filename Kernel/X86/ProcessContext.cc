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
   set_cr3(pageDirectoryM);
}
