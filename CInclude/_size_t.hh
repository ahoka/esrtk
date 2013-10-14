#ifndef _SIZE_T_H
#define _SIZE_T_H

namespace std
{

#ifdef __i386__

typedef unsigned int size_t;

#else
# error unuspported architecture
#endif

}

#endif

