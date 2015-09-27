/*-
 * Copyright (c) 1982, 1986, 1989, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Mike Karels at Berkeley Software Design, Inc.
 *
 * Quite extensively rewritten by Poul-Henning Kamp of the FreeBSD
 * project, to make these variables more userfriendly.
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
 *	@(#)kern_sysctl.c	8.4 (Berkeley) 4/14/94
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: releng/10.2/sys/kern/kern_mib.c 280632 2015-03-25 20:57:54Z ian $");

//#include "opt_compat.h"
//#include "opt_posix.h"
//#include "opt_config.h"

#include <sys/param.h>
#include <sys/kernel.h>
#include <sys/sbuf.h>
#include <sys/systm.h>
#include <sys/sysctl.h>

SYSCTL_ROOT_NODE(0,	  sysctl, CTLFLAG_RW, 0,
	"Sysctl internal magic");
SYSCTL_ROOT_NODE(CTL_KERN,	  kern,   CTLFLAG_RW|CTLFLAG_CAPRD, 0,
	"High kernel, proc, limits &c");
SYSCTL_ROOT_NODE(CTL_VM,	  vm,     CTLFLAG_RW, 0,
	"Virtual memory");
SYSCTL_ROOT_NODE(CTL_VFS,	  vfs,     CTLFLAG_RW, 0,
	"File system");
SYSCTL_ROOT_NODE(CTL_NET,	  net,    CTLFLAG_RW, 0,
	"Network, (see socket.h)");
SYSCTL_ROOT_NODE(CTL_DEBUG,  debug,  CTLFLAG_RW, 0,
	"Debugging");
SYSCTL_NODE(_debug, OID_AUTO,  sizeof,  CTLFLAG_RW, 0,
	"Sizeof various things");
SYSCTL_ROOT_NODE(CTL_HW,	  hw,     CTLFLAG_RW, 0,
	"hardware");
SYSCTL_ROOT_NODE(CTL_MACHDEP, machdep, CTLFLAG_RW, 0,
	"machine dependent");
SYSCTL_ROOT_NODE(CTL_USER,	  user,   CTLFLAG_RW, 0,
	"user-level");
SYSCTL_ROOT_NODE(CTL_P1003_1B,  p1003_1b,   CTLFLAG_RW, 0,
	"p1003_1b, (see p1003_1b.h)");

SYSCTL_ROOT_NODE(OID_AUTO,  compat, CTLFLAG_RW, 0,
	"Compatibility code");
SYSCTL_ROOT_NODE(OID_AUTO, security, CTLFLAG_RW, 0, 
     	"Security");
#ifdef REGRESSION
SYSCTL_ROOT_NODE(OID_AUTO, regression, CTLFLAG_RW, 0,
     "Regression test MIB");
#endif

SYSCTL_INT(_kern, KERN_OSREV, osrevision, CTLFLAG_RD|CTLFLAG_CAPRD,
    SYSCTL_NULL_INT_PTR, BSD, "Operating system revision");

SYSCTL_STRING(_kern, KERN_VERSION, version, CTLFLAG_RD|CTLFLAG_MPSAFE,
    version, 0, "Kernel version");

