#ifndef PARAMETERS_HH
#define PARAMETERS_HH

#define KernelVirtualBase 0xc0000000
#define KernelPageNumber (KernelVirtualBase >> 22)

#define PageSize 4096
#define PageMask (PageSize - 1)

#define VTOPHYS(x) ((x) - KernelVirtualBase)
#define PHYSTOV(x) ((x) + KernelVirtualBase)

#endif
