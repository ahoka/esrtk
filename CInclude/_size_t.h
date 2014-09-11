#ifndef _SIZE_T_H
#define _SIZE_T_H

#ifdef __i386__

typedef long unsigned int size_t;
typedef long int ssize_t;

#else
#  error unuspported architecture
#endif

#endif

