#ifndef MACHINE_ENDIAN_H
#define MACHINE_ENDIAN_H

#ifdef __i386__
#include <i386/endian.h>
#else
#error "Unsupported platform!"
#endif

#endif
