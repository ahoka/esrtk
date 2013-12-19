#ifndef __SPINLOCK_H__
#define __SPINLOCK_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef long spinlock_t;

#define SPINLOCK_STATIC_INITIALIZER 0

long spinlock_lock(spinlock_t* lock);
long spinlock_unlock(spinlock_t* lock);
long spinlock_try(spinlock_t* lock);

#ifdef __cplusplus
}
#endif

#endif
