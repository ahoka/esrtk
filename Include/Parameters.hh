#ifndef PARAMATERS_HH
#define PARAMATERS_HH

#ifdef __i386__
#include <X86/Parameters.hh>
#endif

#ifndef ASSEMBLER

enum SystemConstants
{
   PathMax = 256
};

enum SystemParameters
{
   PageFrameCount = 10
};

#endif

#endif
