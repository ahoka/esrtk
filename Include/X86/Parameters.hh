#ifndef X86_PARAMETERS_HH
#define X86_PARAMETERS_HH

#ifdef ASSEMBLER
#  define UNSIGNED(x) x
#else
#  define UNSIGNED(x) x##u
#endif

#define KernelVirtualBase UNSIGNED(0xc0000000)
#define KernelLoadAddress UNSIGNED(0x100000)
#define KernelPageNumber (KernelVirtualBase >> 22)

#define PageSize UNSIGNED(4096)
#define PageMask (PageSize - 1)

#define StackStart UNSIGNED(0xffc00000)
#define HeapStart UNSIGNED(0xc0501000)

#define StackSize UNSIGNED(0x10000)
#define MapStart (StackStart - StackSize)

#define InitialStackSize UNSIGNED(0x2000)

#endif
