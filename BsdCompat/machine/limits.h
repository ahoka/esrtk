#ifndef _MACHINE_LIMITS_H_
#define _MACHINE_LIMITS_H_

#ifdef __i386__
# include <i386/limits.h>
#else
# error Unsupported platform!
#endif

#endif
