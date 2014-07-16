#ifndef PARAMATERS_HH
#define PARAMATERS_HH

#ifdef __i386__
# include <X86/Parameters.hh>
#else
# error Unsupported architecture!
#endif

#ifndef __ASSEMBLER__

enum SystemConstants
{
   PathMax = 256,
   BootCommandLineMax = 256,
   CpuMax = 255
};

enum SystemParameters
{
   PageFrameCount = 10
};

#endif

#endif
