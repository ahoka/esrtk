extern "C"
{
#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/systm.h>
#include <sys/param.h>
#include <sys/lock.h>
#include <sys/mutex.h>
#include <sys/sx.h>
}

#include <Debug.hh>

void
mtx_sysinit(void *arg)
{
}

void
_mtx_init(volatile uintptr_t *c, const char *name, const char *type, int opts)
{
   Debug::info("_mtx_init called!");
}

void
_mtx_destroy(volatile uintptr_t *c)
{
   Debug::info("_mtx_destroy called!");
}

void
sx_init_flags(struct sx *sx, const char *description, int opts)
{
   Debug::info("sx_init_flags called!");
}

void
sx_destroy(struct sx *sx)
{
   Debug::info("sx_destroy called!");
}

void
__mtx_unlock_sleep(volatile uintptr_t *c, int opts, const char *file, int line)
{
}

void
__mtx_lock_sleep(volatile uintptr_t *c, uintptr_t tid, int opts, const char *file, int line)
{
}

void
_sx_xunlock_hard(struct sx *sx, uintptr_t tid, const char *file, int line)
{
}

int
_sx_xlock_hard(struct sx *sx, uintptr_t tid, int opts, const char *file, int line)
{
   return 0;
}
