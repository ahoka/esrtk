#ifndef X86_PARAMETERS_HH
#define X86_PARAMETERS_HH

#ifdef __ASSEMBLER__
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
// ...
// 0xffffeffc -- kernel stack start
// ...
// 0xc0501000 -- kernel heap start
// ...
// 0xc0000000 -- kernel start
// ...
// 0xbffffffc -- user stack start (grows down)
// ...
// 0x40000000 -- user heap start
// ...
// 0x10000000 -- user code start
//

#define KernelCodeSegment 0x08
#define KernelDataSegment 0x10
#define UserCodeSegment 0x18
#define UserDataSegment 0x20

#define KernelVirtualBase U(0xc0000000)
#define KernelLoadAddress U(0x100000)
#define KernelPageNumber (KernelVirtualBase >> 22)

#define PageSize U(4096)
#define PageMask (PageSize - 1)

#define PageTableBase U(0xffc00000)
#define PageDirectoryBase U(0xfffff000)

#define KernelStackStart U(0xffffeffc)
#define KernelStackSize U(0x10000)
#define KernelHeapStart U(0xc0501000)

#define UserStackStart U(0xbffffffc)
#define UserHeapStart U(0x40000000)

#define CodeStart U(0x10000000)
#define CodeSize U(0x400000)

#define MapStart (KernelStackStart - KernelStackSize)

#define InitialStackSize U(0x2000)

#endif
