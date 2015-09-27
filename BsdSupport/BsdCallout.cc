extern "C"
{
#include <sys/types.h>
#include <sys/callout.h>
}

volatile time_t time_second = 1;

int
callout_reset_sbt_on(struct callout *c, sbintime_t sbt, sbintime_t precision,
                     void (*ftn)(void *), void *arg, int cpu, int flags)
{
   return 0;
}

int
_callout_stop_safe(struct callout *c, int safe)
{
   return 0;
}
