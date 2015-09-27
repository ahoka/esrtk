/*-
 * Copyright (c) 1980, 1986, 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 * (c) UNIX System Laboratories, Inc.
 * All or some portions of this file are derived from material licensed
 * to the University of California by American Telephone and Telegraph
 * Co. or Unix System Laboratories, Inc. and are reproduced herein with
 * the permission of UNIX System Laboratories, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)param.c	8.3 (Berkeley) 8/20/94
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: releng/10.2/sys/kern/subr_param.c 278522 2015-02-10 16:34:42Z jhb $");

/* #include "opt_param.h" */
/* #include "opt_msgbuf.h" */
/* #include "opt_maxusers.h" */
#define MAXUSERS 20

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/kernel.h>
#include <sys/limits.h>
#include <sys/msgbuf.h>
#include <sys/sysctl.h>
//#include <sys/proc.h>

/* #include <vm/vm.h> */
/* #include <vm/vm_param.h> */
/* #include <vm/pmap.h> */

/*
 * System parameter formulae.
 */

#ifndef HZ
#  if defined(__mips__) || defined(__arm__)
#    define	HZ 100
#  else
#    define	HZ 1000
#  endif
#  ifndef HZ_VM
#    define	HZ_VM 100
#  endif
#else
#  ifndef HZ_VM
#    define	HZ_VM HZ
#  endif
#endif
#define	NPROC (20 + 16 * maxusers)
#ifndef NBUF
#define NBUF 0
#endif
#ifndef MAXFILES
#define	MAXFILES (maxproc * 2)
#endif

static int sysctl_kern_vm_guest(SYSCTL_HANDLER_ARGS);

int	hz;				/* system clock's frequency */
int	tick;				/* usec per tick (1000000 / hz) */
struct bintime tick_bt;			/* bintime per tick (1s / hz) */
sbintime_t tick_sbt;
int	maxusers;			/* base tunable */
int	maxproc;			/* maximum # of processes */
int	maxprocperuid;			/* max # of procs per user */
int	maxfiles;			/* sys. wide open files limit */
int	maxfilesperproc;		/* per-proc open files limit */
int	msgbufsize;			/* size of kernel message buffer */
int	nbuf;
int	bio_transient_maxcnt;
int	ngroups_max;			/* max # groups per process */
int	nswbuf;
//pid_t	pid_max = PID_MAX;
long	maxswzone;			/* max swmeta KVA storage */
long	maxbcache;			/* max buffer cache KVA storage */
long	maxpipekva;			/* Limit on pipe KVA */

SYSCTL_INT(_kern, OID_AUTO, hz, CTLFLAG_RDTUN, &hz, 0,
    "Number of clock ticks per second");

/*
 * These have to be allocated somewhere; allocating
 * them here forces loader errors if this file is omitted
 * (if they've been externed everywhere else; hah!).
 */
struct	buf *swbuf;

/*
 * Boot time overrides that are not scaled against main memory
 */
void
init_param1(void)
{
   hz = 100;
   tick = 1000000 / hz;
   tick_sbt = SBT_1S / hz;
   tick_bt = sbttobt(tick_sbt);
}
