extern "C"
{
#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/systm.h>
}

extern "C" struct cdev *
make_dev(struct cdevsw *devsw, int unit, uid_t uid, gid_t gid, int mode, const char *fmt, ...)
{
}

extern "C" void
destroy_dev(struct cdev *dev)
{
}
