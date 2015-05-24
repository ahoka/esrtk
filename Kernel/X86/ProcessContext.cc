#include <Kernel/ProcessContext.hh>

#include <X86/Gdt.hh>
#include <X86/Tss.hh>

void
Kernel::ProcessContext::init()
{
   x86_gdt_init();
   x86_tss_init();
}

void
Kernel::ProcessContext::setKernelStack(uintptr_t stack)
{
   x86_tss_set_kstack(stack);
}
