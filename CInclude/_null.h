#ifndef _NULL_H
#define _NULL_H

#ifndef __cplusplus
#  define NULL ((void *)0)
#else
//#  if __cplusplus >= 201103L
//#    define NULL std::nullptr
//#  else
#    define NULL 0
//#  endif
#endif

#endif

