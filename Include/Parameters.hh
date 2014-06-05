#ifndef PARAMATERS_HH
#define PARAMATERS_HH

#ifdef __i386__
# include <X86/Parameters.hh>
#else
# error Unsupported architecture!
#endif

#ifndef ASSEMBLER

enum SystemConstants
{
   PathMax = 256,
   CpuMax = 255
};

enum SystemParameters
{
   PageFrameCount = 10
};

#endif

#endif
