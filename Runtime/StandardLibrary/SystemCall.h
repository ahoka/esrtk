#ifndef SYSTEMCALL_H
#define SYSTEMCALL_H

#ifdef __cplusplus
extern "C" {
#endif

int syscall(unsigned int nr, void *arg);

#ifdef __cplusplus
}
#endif

#endif
