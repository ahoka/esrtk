#ifndef _SIZE_T_H
#define _SIZE_T_H

#ifdef __i386__

#ifdef CROSS_COMPILER
typedef long unsigned int size_t;
#else
typedef unsigned int size_t;
#endif

#else
#  error unuspported architecture
#endif

#endif

