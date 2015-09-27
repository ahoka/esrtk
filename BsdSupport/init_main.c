/*-
 * Copyright (c) 1995 Terrence R. Lambert
 * All rights reserved.
 *
 * Copyright (c) 1982, 1986, 1989, 1991, 1992, 1993
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
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
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
 *	@(#)init_main.c	8.9 (Berkeley) 1/21/94
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: releng/10.2/sys/kern/init_main.c 281847 2015-04-22 10:25:08Z pluknet $");

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/malloc.h>
#include <sys/kernel.h>
#include <sys/sysctl.h>
#include <sys/reboot.h>
#include <sys/copyright.h>

char version[] = "10.2";

#ifndef BOOTHOWTO
#define	BOOTHOWTO	0
#endif
int	boothowto = BOOTHOWTO;	/* initialized so that it can be patched */
SYSCTL_INT(_debug, OID_AUTO, boothowto, CTLFLAG_RD, &boothowto, 0,
	"Boot control flags, passed from loader");

#ifndef BOOTVERBOSE
#define	BOOTVERBOSE	0
#endif
int	bootverbose = BOOTVERBOSE;
SYSCTL_INT(_debug, OID_AUTO, bootverbose, CTLFLAG_RW, &bootverbose, 0,
	"Control the output of verbose kernel messages");

/*
 * This ensures that there is at least one entry so that the sysinit_set
 * symbol is not undefined.  A sybsystem ID of SI_SUB_DUMMY is never
 * executed.
 */
SYSINIT(placeholder, SI_SUB_DUMMY, SI_ORDER_ANY, NULL, NULL);

/*
 * The sysinit table itself.  Items are checked off as the are run.
 * If we want to register new sysinit types, add them to newsysinit.
 */
SET_DECLARE(sysinit_set, struct sysinit);
struct sysinit **sysinit, **sysinit_end;
struct sysinit **newsysinit, **newsysinit_end;

/*
 * Merge a new sysinit set into the current set, reallocating it if
 * necessary.  This can only be called after malloc is running.
 */
void
sysinit_add(struct sysinit **set, struct sysinit **set_end)
{
	struct sysinit **newset;
	struct sysinit **sipp;
	struct sysinit **xipp;
	int count;

	count = set_end - set;
	if (newsysinit)
		count += newsysinit_end - newsysinit;
	else
		count += sysinit_end - sysinit;
	newset = malloc(count * sizeof(*sipp), M_TEMP, M_NOWAIT);
	if (newset == NULL)
		panic("cannot malloc for sysinit");
	xipp = newset;
	if (newsysinit)
		for (sipp = newsysinit; sipp < newsysinit_end; sipp++)
			*xipp++ = *sipp;
	else
		for (sipp = sysinit; sipp < sysinit_end; sipp++)
			*xipp++ = *sipp;
	for (sipp = set; sipp < set_end; sipp++)
		*xipp++ = *sipp;
	if (newsysinit)
		free(newsysinit, M_TEMP);
	newsysinit = newset;
	newsysinit_end = newset + count;
}

/*
 * System startup; initialize the world, create process 0, mount root
 * filesystem, and fork to create init and pagedaemon.  Most of the
 * hard work is done in the lower-level initialization routines including
 * startup(), which does memory initialization and autoconfiguration.
 *
 * This allows simple addition of new kernel subsystems that require
 * boot time initialization.  It also allows substitution of subsystem
 * (for instance, a scheduler, kernel profiler, or VM system) by object
 * module.  Finally, it allows for optional "kernel threads".
 */
void
mi_startup(void)
{

	register struct sysinit **sipp;		/* system initialization*/
	register struct sysinit **xipp;		/* interior loop of sort*/
	register struct sysinit *save;		/* bubble*/

#if defined(VERBOSE_SYSINIT)
	int last;
	int verbose;
#endif

	if (boothowto & RB_VERBOSE)
		bootverbose++;

	if (sysinit == NULL) {
		sysinit = SET_BEGIN(sysinit_set);
		sysinit_end = SET_LIMIT(sysinit_set);
	}

restart:
	/*
	 * Perform a bubble sort of the system initialization objects by
	 * their subsystem (primary key) and order (secondary key).
	 */
	for (sipp = sysinit; sipp < sysinit_end; sipp++) {
		for (xipp = sipp + 1; xipp < sysinit_end; xipp++) {
			if ((*sipp)->subsystem < (*xipp)->subsystem ||
			     ((*sipp)->subsystem == (*xipp)->subsystem &&
			      (*sipp)->order <= (*xipp)->order))
				continue;	/* skip*/
			save = *sipp;
			*sipp = *xipp;
			*xipp = save;
		}
	}

#if defined(VERBOSE_SYSINIT)
	last = SI_SUB_COPYRIGHT;
	verbose = 0;
#if !defined(DDB)
	printf("VERBOSE_SYSINIT: DDB not enabled, symbol lookups disabled.\n");
#endif
#endif

	/*
	 * Traverse the (now) ordered list of system initialization tasks.
	 * Perform each task, and continue on to the next task.
	 */
	for (sipp = sysinit; sipp < sysinit_end; sipp++) {

		if ((*sipp)->subsystem == SI_SUB_DUMMY)
			continue;	/* skip dummy task(s)*/

		if ((*sipp)->subsystem == SI_SUB_DONE)
			continue;

#if defined(VERBOSE_SYSINIT)
		if ((*sipp)->subsystem > last) {
			verbose = 1;
			last = (*sipp)->subsystem;
			printf("subsystem %x\n", last);
		}
		if (verbose) {
#if defined(DDB)
			const char *func, *data;

			func = symbol_name((vm_offset_t)(*sipp)->func,
			    DB_STGY_PROC);
			data = symbol_name((vm_offset_t)(*sipp)->udata,
			    DB_STGY_ANY);
			if (func != NULL && data != NULL)
				printf("   %s(&%s)... ", func, data);
			else if (func != NULL)
				printf("   %s(%p)... ", func, (*sipp)->udata);
			else
#endif
				printf("   %p(%p)... ", (*sipp)->func,
				    (*sipp)->udata);
		}
#endif

		/* Call function */
		(*((*sipp)->func))((*sipp)->udata);

#if defined(VERBOSE_SYSINIT)
		if (verbose)
			printf("done.\n");
#endif

		/* Check off the one we're just done */
		(*sipp)->subsystem = SI_SUB_DONE;

		/* Check if we've installed more sysinit items via KLD */
		if (newsysinit != NULL) {
			if (sysinit != SET_BEGIN(sysinit_set))
				free(sysinit, M_TEMP);
			sysinit = newsysinit;
			sysinit_end = newsysinit_end;
			newsysinit = NULL;
			newsysinit_end = NULL;
			goto restart;
		}
	}

	/* mtx_assert(&Giant, MA_OWNED | MA_NOTRECURSED); */
	/* mtx_unlock(&Giant); */

	/*
	 * Now hand over this thread to swapper.
	 */
//	swapper();
	/* NOTREACHED*/
}


/*
 ***************************************************************************
 ****
 **** The following SYSINIT's belong elsewhere, but have not yet
 **** been moved.
 ****
 ***************************************************************************
 */
static void
print_caddr_t(void *data)
{
	printf("%s", (char *)data);
}

SYSINIT(announce, SI_SUB_COPYRIGHT, SI_ORDER_FIRST, print_caddr_t,
    copyright);
SYSINIT(trademark, SI_SUB_COPYRIGHT, SI_ORDER_SECOND, print_caddr_t,
    trademark);
