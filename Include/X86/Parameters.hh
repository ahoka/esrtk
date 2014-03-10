#ifndef X86_PARAMETERS_HH
#define X86_PARAMETERS_HH

#ifdef ASSEMBLER
#  define U(x) x
#else
#  define U(x) x##u
#endif

//
// Memory map:
//
// ----------
// 0xffffffff -- recursive page directory
// ...
// 0xffc00000 -- end of page directory mapping
// 0xffbffc00 -- secondary page directory (for create process)
// ...
// 0xff800000 -- end of secondary pd, stack start
// ...
// 0xc0501000 -- kernel heap start
// ...
// 0xc0000000
//

#define KernelCodeSegment 0x08
#define KernelDataSegment 0x10

#define KernelVirtualBase U(0xc0000000)
#define KernelLoadAddress U(0x100000)
#define KernelPageNumber (KernelVirtualBase >> 22)

#define PageSize U(4096)
#define PageMask (PageSize - 1)

#define PageTableBase U(0xffc00000)
#define PageDirectoryBase U(0xfffff000)
#define SecondaryPageTableBase U(0xff800000)
#define SecondaryPageDirectoryBase U(0xffbffc0)

#define StackStart SecondaryPageTableBase
#define HeapStart U(0xc0501000)

#define StackSize U(0x10000)
#define MapStart (StackStart - StackSize)

#define InitialStackSize U(0x2000)

#endif
