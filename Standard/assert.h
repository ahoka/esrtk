#ifndef ASSERT_H
#define ASSERT_H

#include <stdlib.h>
#include <stdio.h>

#define assert(x) if (!(x)) { printf("Assertion (" #x ") failed at " __FILE__ ":%d\n", __LINE__); abort(); }

//#ifdef _NETBSD_SOURCE
#define _DIAGASSERT(x) if (!(x)) { printf("Assertion (" #x ") failed at " __FILE__ ":%d\n", __LINE__); abort(); }
//#endif

#endif

