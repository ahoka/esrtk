#ifndef NULL

#ifdef __GNUG__
# define NULL __null
#elif defined(__cplusplus)
# define NULL 0L
#else
# define NULL ((void *)0)
#endif

#endif
