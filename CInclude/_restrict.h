#ifndef _RESTRICT_H
#define _RESTRICT_H

#ifdef __cplusplus
# undef __restrict
# define __restrict
#else
# undef __restrict
# define __restrict restrict
#endif

#endif

