extern "C"
{
#include <sys/cdefs.h>
#include <sys/types.h>
#include <sys/systm.h>
#include <sys/param.h>
#include <sys/callout.h>
}

#include <Debug.hh>

void
_callout_init_lock(struct callout *c, struct lock_object *lock, int flags)
{
   Debug::info("_callout_init_lock called!\n");
}
