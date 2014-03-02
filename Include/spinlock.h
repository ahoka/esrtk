#ifndef __SPINLOCK_H__
#define __SPINLOCK_H__

#ifdef __cplusplus
extern "C"
{
#endif

typedef long spinlock_t;
typedef long irq_flags_t;

#define SPINLOCK_STATIC_INITIALIZER 0

long spinlock_enter(spinlock_t* lock);
long spinlock_exit(spinlock_t* lock);
long spinlock_try(spinlock_t* lock);

// Spinlocks callable from interrupt routine
// Disables local interrupts on lock, restores
// on unlock.
//
long spinlock_softirq_enter(spinlock_t* lock, irq_flags_t* flags);
long spinlock_softirq_exit(spinlock_t* lock, irq_flags_t flags);

#ifdef __cplusplus
}
#endif

#endif
