#ifndef ASSERT_H
#define ASSERT_H

#include <stdlib.h>
#include <stdio.h>

#define assert(x) if (!(x)) { printf("Assertion (%s) failed at %s:%d\n", (#x), __FILE__, __LINE__); abort(); }

//#ifdef _NETBSD_SOURCE
#define _DIAGASSERT(x) if (!(x)) { printf("Assertion (%s) failed at %s:%d\n", (#x), __FILE__, __LINE__); abort(); }
//#endif

#endif

