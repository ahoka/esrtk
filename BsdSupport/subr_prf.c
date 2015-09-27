/*-
 * Copyright (c) 1986, 1988, 1991, 1993
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
 *	@(#)subr_prf.c	8.3 (Berkeley) 1/21/94
 */

#include <sys/cdefs.h>
__FBSDID("$FreeBSD: releng/10.2/sys/kern/subr_prf.c 284435 2015-06-16 02:31:11Z ken $");

#include <sys/param.h>
#ifdef _KERNEL
#include <sys/systm.h>
#include <sys/lock.h>
#include <sys/kdb.h>
#include <sys/mutex.h>
#include <sys/sx.h>
#include <sys/kernel.h>
#include <sys/msgbuf.h>
#include <sys/malloc.h>
#include <sys/priv.h>
//#include <sys/proc.h>
#include <sys/stddef.h>
#include <sys/sysctl.h>
#include <sys/tty.h>
#include <sys/syslog.h>
#include <sys/cons.h>
#include <sys/uio.h>
#endif
#include <sys/ctype.h>
#include <sys/sbuf.h>

/*
 * Note that stdarg.h and the ANSI style va_start macro is used for both
 * ANSI and traditional C compilers.
 */
#include <machine/stdarg.h>

#ifdef _KERNEL

#define TOCONS	0x01
#define TOTTY	0x02
#define TOLOG	0x04

/* Max number conversion buffer length: a u_quad_t in base 2, plus NUL byte. */
#define MAXNBUF	(sizeof(intmax_t) * NBBY + 1)

struct putchar_arg {
	int	flags;
	int	pri;
	struct	tty *tty;
	char	*p_bufr;
	size_t	n_bufr;
	char	*p_next;
	size_t	remain;
};

struct snprintf_arg {
	char	*str;
	size_t	remain;
};

int log_open = false;

static void  msglogchar(int c, int pri);
static void  msglogstr(char *str, int pri, int filter_cr);
static void  putchar(int ch, void *arg);
static char *ksprintn(char *nbuf, uintmax_t num, int base, int *len, int upper);
static void  snprintf_func(int ch, void *arg);

/*
 * Put a NUL-terminated ASCII number (base <= 36) in a buffer in reverse
 * order; return an optional length and a pointer to the last character
 * written in the buffer (i.e., the first character of the string).
 * The buffer pointed to by `nbuf' must have length >= MAXNBUF.
 */
static char *
ksprintn(char *nbuf, uintmax_t num, int base, int *lenp, int upper)
{
	char *p, c;

	p = nbuf;
	*p = '\0';
	do {
		c = hex2ascii(num % base);
		*++p = upper ? toupper(c) : c;
	} while (num /= base);
	if (lenp)
		*lenp = p - nbuf;
	return (p);
}


static int msgbufmapped;		/* Set when safe to use msgbuf */
int msgbuftrigger;

/*
 * Warn that a system table is full.
 */
void
tablefull(const char *tab)
{

	log(LOG_ERR, "%s: table is full\n", tab);
}

/*
 * Uprintf prints to the controlling terminal for the current process.
 */
int
uprintf(const char *fmt, ...)
{
   return 0;
}

/*
 * tprintf and vtprintf print on the controlling terminal associated with the
 * given session, possibly to the log as well.
 */
void
tprintf(struct proc *p, int pri, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vtprintf(p, pri, fmt, ap);
	va_end(ap);
}

void
vtprintf(struct proc *p, int pri, const char *fmt, va_list ap)
{
}

/*
 * Ttyprintf displays a message on a tty; it should be used only by
 * the tty driver, or anything that knows the underlying tty will not
 * be revoke(2)'d away.  Other callers should use tprintf.
 */
int
ttyprintf(struct tty *tp, const char *fmt, ...)
{
	va_list ap;
	struct putchar_arg pca;
	int retval;

	va_start(ap, fmt);
	pca.tty = tp;
	pca.flags = TOTTY;
	pca.p_bufr = NULL;
	retval = kvprintf(fmt, putchar, &pca, 10, ap);
	va_end(ap);
	return (retval);
}

static int
_vprintf(int level, int flags, const char *fmt, va_list ap)
{
	struct putchar_arg pca;
	int retval;
#ifdef PRINTF_BUFR_SIZE
	char bufr[PRINTF_BUFR_SIZE];
#endif

	pca.tty = NULL;
	pca.pri = level;
	pca.flags = flags;
#ifdef PRINTF_BUFR_SIZE
	pca.p_bufr = bufr;
	pca.p_next = pca.p_bufr;
	pca.n_bufr = sizeof(bufr);
	pca.remain = sizeof(bufr);
	*pca.p_next = '\0';
#else
	/* Don't buffer console output. */
	pca.p_bufr = NULL;
#endif

	retval = kvprintf(fmt, putchar, &pca, 10, ap);

#ifdef PRINTF_BUFR_SIZE
	/* Write any buffered console/log output: */
	if (*pca.p_bufr != '\0') {
		if (pca.flags & TOLOG)
			msglogstr(pca.p_bufr, level, /*filter_cr*/1);

		if (pca.flags & TOCONS)
			cnputs(pca.p_bufr);
	}
#endif

	return (retval);
}

/*
 * Log writes to the log buffer, and guarantees not to sleep (so can be
 * called by interrupt routines).  If there is no process reading the
 * log yet, it writes to the console also.
 */
void
log(int level, const char *fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	(void)_vprintf(level, log_open ? TOLOG : TOCONS, fmt, ap);
	va_end(ap);

	msgbuftrigger = 1;
}

#define CONSCHUNK 128

void
log_console(struct uio *uio)
{
}

#if 0
int
printf(const char *fmt, ...)
{
	va_list ap;
	int retval;

	va_start(ap, fmt);
	retval = vprintf(fmt, ap);
	va_end(ap);

	return (retval);
}

int
vprintf(const char *fmt, va_list ap)
{
	int retval;

	retval = _vprintf(-1, TOCONS | TOLOG, fmt, ap);

	if (!panicstr)
		msgbuftrigger = 1;

	return (retval);
}

/*
 * Scaled down version of sprintf(3).
 */
int
sprintf(char *buf, const char *cfmt, ...)
{
	int retval;
	va_list ap;

	va_start(ap, cfmt);
	retval = kvprintf(cfmt, NULL, (void *)buf, 10, ap);
	buf[retval] = '\0';
	va_end(ap);
	return (retval);
}

/*
 * Scaled down version of vsprintf(3).
 */
int
vsprintf(char *buf, const char *cfmt, va_list ap)
{
	int retval;

	retval = kvprintf(cfmt, NULL, (void *)buf, 10, ap);
	buf[retval] = '\0';
	return (retval);
}

/*
 * Scaled down version of snprintf(3).
 */
int
snprintf(char *str, size_t size, const char *format, ...)
{
	int retval;
	va_list ap;

	va_start(ap, format);
	retval = vsnprintf(str, size, format, ap);
	va_end(ap);
	return(retval);
}

/*
 * Scaled down version of vsnprintf(3).
 */
int
vsnprintf(char *str, size_t size, const char *format, va_list ap)
{
	struct snprintf_arg info;
	int retval;

	info.str = str;
	info.remain = size;
	retval = kvprintf(format, snprintf_func, &info, 10, ap);
	if (info.remain >= 1)
		*info.str++ = '\0';
	return (retval);
}
#endif

/*
 * Kernel version which takes radix argument vsnprintf(3).
 */
int
vsnrprintf(char *str, size_t size, int radix, const char *format, va_list ap)
{
	struct snprintf_arg info;
	int retval;

	info.str = str;
	info.remain = size;
	retval = kvprintf(format, snprintf_func, &info, radix, ap);
	if (info.remain >= 1)
		*info.str++ = '\0';
	return (retval);
}

static void
snprintf_func(int ch, void *arg)
{
	struct snprintf_arg *const info = arg;

	if (info->remain >= 2) {
		*info->str++ = ch;
		info->remain--;
	}
}

/*
 * Scaled down version of printf(3).
 *
 * Two additional formats:
 *
 * The format %b is supported to decode error registers.
 * Its usage is:
 *
 *	printf("reg=%b\n", regval, "<base><arg>*");
 *
 * where <base> is the output base expressed as a control character, e.g.
 * \10 gives octal; \20 gives hex.  Each arg is a sequence of characters,
 * the first of which gives the bit number to be inspected (origin 1), and
 * the next characters (up to a control character, i.e. a character <= 32),
 * give the name of the register.  Thus:
 *
 *	kvprintf("reg=%b\n", 3, "\10\2BITTWO\1BITONE\n");
 *
 * would produce output:
 *
 *	reg=3<BITTWO,BITONE>
 *
 * XXX:  %D  -- Hexdump, takes pointer and separator string:
 *		("%6D", ptr, ":")   -> XX:XX:XX:XX:XX:XX
 *		("%*D", len, ptr, " " -> XX XX XX XX ...
 */
int
kvprintf(char const *fmt, void (*func)(int, void*), void *arg, int radix, va_list ap)
{
#define PCHAR(c) {int cc=(c); if (func) (*func)(cc,arg); else *d++ = cc; retval++; }
	char nbuf[MAXNBUF];
	char *d;
	const char *p, *percent, *q;
	u_char *up;
	int ch, n;
	uintmax_t num;
	int base, lflag, qflag, tmp, width, ladjust, sharpflag, neg, sign, dot;
	int cflag, hflag, jflag, tflag, zflag;
	int dwidth, upper;
	char padc;
	int stop = 0, retval = 0;

	num = 0;
	if (!func)
		d = (char *) arg;
	else
		d = NULL;

	if (fmt == NULL)
		fmt = "(fmt null)\n";

	if (radix < 2 || radix > 36)
		radix = 10;

	for (;;) {
		padc = ' ';
		width = 0;
		while ((ch = (u_char)*fmt++) != '%' || stop) {
			if (ch == '\0')
				return (retval);
			PCHAR(ch);
		}
		percent = fmt - 1;
		qflag = 0; lflag = 0; ladjust = 0; sharpflag = 0; neg = 0;
		sign = 0; dot = 0; dwidth = 0; upper = 0;
		cflag = 0; hflag = 0; jflag = 0; tflag = 0; zflag = 0;
reswitch:	switch (ch = (u_char)*fmt++) {
		case '.':
			dot = 1;
			goto reswitch;
		case '#':
			sharpflag = 1;
			goto reswitch;
		case '+':
			sign = 1;
			goto reswitch;
		case '-':
			ladjust = 1;
			goto reswitch;
		case '%':
			PCHAR(ch);
			break;
		case '*':
			if (!dot) {
				width = va_arg(ap, int);
				if (width < 0) {
					ladjust = !ladjust;
					width = -width;
				}
			} else {
				dwidth = va_arg(ap, int);
			}
			goto reswitch;
		case '0':
			if (!dot) {
				padc = '0';
				goto reswitch;
			}
		case '1': case '2': case '3': case '4':
		case '5': case '6': case '7': case '8': case '9':
				for (n = 0;; ++fmt) {
					n = n * 10 + ch - '0';
					ch = *fmt;
					if (ch < '0' || ch > '9')
						break;
				}
			if (dot)
				dwidth = n;
			else
				width = n;
			goto reswitch;
		case 'b':
			num = (u_int)va_arg(ap, int);
			p = va_arg(ap, char *);
			for (q = ksprintn(nbuf, num, *p++, NULL, 0); *q;)
				PCHAR(*q--);

			if (num == 0)
				break;

			for (tmp = 0; *p;) {
				n = *p++;
				if (num & (1 << (n - 1))) {
					PCHAR(tmp ? ',' : '<');
					for (; (n = *p) > ' '; ++p)
						PCHAR(n);
					tmp = 1;
				} else
					for (; *p > ' '; ++p)
						continue;
			}
			if (tmp)
				PCHAR('>');
			break;
		case 'c':
			PCHAR(va_arg(ap, int));
			break;
		case 'D':
			up = va_arg(ap, u_char *);
			p = va_arg(ap, char *);
			if (!width)
				width = 16;
			while(width--) {
				PCHAR(hex2ascii(*up >> 4));
				PCHAR(hex2ascii(*up & 0x0f));
				up++;
				if (width)
					for (q=p;*q;q++)
						PCHAR(*q);
			}
			break;
		case 'd':
		case 'i':
			base = 10;
			sign = 1;
			goto handle_sign;
		case 'h':
			if (hflag) {
				hflag = 0;
				cflag = 1;
			} else
				hflag = 1;
			goto reswitch;
		case 'j':
			jflag = 1;
			goto reswitch;
		case 'l':
			if (lflag) {
				lflag = 0;
				qflag = 1;
			} else
				lflag = 1;
			goto reswitch;
		case 'n':
			if (jflag)
				*(va_arg(ap, intmax_t *)) = retval;
			else if (qflag)
				*(va_arg(ap, quad_t *)) = retval;
			else if (lflag)
				*(va_arg(ap, long *)) = retval;
			else if (zflag)
				*(va_arg(ap, size_t *)) = retval;
			else if (hflag)
				*(va_arg(ap, short *)) = retval;
			else if (cflag)
				*(va_arg(ap, char *)) = retval;
			else
				*(va_arg(ap, int *)) = retval;
			break;
		case 'o':
			base = 8;
			goto handle_nosign;
		case 'p':
			base = 16;
			sharpflag = (width == 0);
			sign = 0;
			num = (uintptr_t)va_arg(ap, void *);
			goto number;
		case 'q':
			qflag = 1;
			goto reswitch;
		case 'r':
			base = radix;
			if (sign)
				goto handle_sign;
			goto handle_nosign;
		case 's':
			p = va_arg(ap, char *);
			if (p == NULL)
				p = "(null)";
			if (!dot)
				n = strlen (p);
			else
				for (n = 0; n < dwidth && p[n]; n++)
					continue;

			width -= n;

			if (!ladjust && width > 0)
				while (width--)
					PCHAR(padc);
			while (n--)
				PCHAR(*p++);
			if (ladjust && width > 0)
				while (width--)
					PCHAR(padc);
			break;
		case 't':
			tflag = 1;
			goto reswitch;
		case 'u':
			base = 10;
			goto handle_nosign;
		case 'X':
			upper = 1;
		case 'x':
			base = 16;
			goto handle_nosign;
		case 'y':
			base = 16;
			sign = 1;
			goto handle_sign;
		case 'z':
			zflag = 1;
			goto reswitch;
handle_nosign:
			sign = 0;
			if (jflag)
				num = va_arg(ap, uintmax_t);
			else if (qflag)
				num = va_arg(ap, u_quad_t);
			else if (tflag)
				num = va_arg(ap, ptrdiff_t);
			else if (lflag)
				num = va_arg(ap, u_long);
			else if (zflag)
				num = va_arg(ap, size_t);
			else if (hflag)
				num = (u_short)va_arg(ap, int);
			else if (cflag)
				num = (u_char)va_arg(ap, int);
			else
				num = va_arg(ap, u_int);
			goto number;
handle_sign:
			if (jflag)
				num = va_arg(ap, intmax_t);
			else if (qflag)
				num = va_arg(ap, quad_t);
			else if (tflag)
				num = va_arg(ap, ptrdiff_t);
			else if (lflag)
				num = va_arg(ap, long);
			else if (zflag)
				num = va_arg(ap, ssize_t);
			else if (hflag)
				num = (short)va_arg(ap, int);
			else if (cflag)
				num = (char)va_arg(ap, int);
			else
				num = va_arg(ap, int);
number:
			if (sign && (intmax_t)num < 0) {
				neg = 1;
				num = -(intmax_t)num;
			}
			p = ksprintn(nbuf, num, base, &n, upper);
			tmp = 0;
			if (sharpflag && num != 0) {
				if (base == 8)
					tmp++;
				else if (base == 16)
					tmp += 2;
			}
			if (neg)
				tmp++;

			if (!ladjust && padc == '0')
				dwidth = width - tmp;
			width -= tmp + imax(dwidth, n);
			dwidth -= n;
			if (!ladjust)
				while (width-- > 0)
					PCHAR(' ');
			if (neg)
				PCHAR('-');
			if (sharpflag && num != 0) {
				if (base == 8) {
					PCHAR('0');
				} else if (base == 16) {
					PCHAR('0');
					PCHAR('x');
				}
			}
			while (dwidth-- > 0)
				PCHAR('0');

			while (*p)
				PCHAR(*p--);

			if (ladjust)
				while (width-- > 0)
					PCHAR(' ');

			break;
		default:
			while (percent < fmt)
				PCHAR(*percent++);
			/*
			 * Since we ignore a formatting argument it is no 
			 * longer safe to obey the remaining formatting
			 * arguments as the arguments will no longer match
			 * the format specs.
			 */
			stop = 1;
			break;
		}
	}
#undef PCHAR
}

void
msgbufinit(void *ptr, int size)
{
}

void
hexdump(const void *ptr, int length, const char *hdr, int flags)
{
	int i, j, k;
	int cols;
	const unsigned char *cp;
	char delim;

	if ((flags & HD_DELIM_MASK) != 0)
		delim = (flags & HD_DELIM_MASK) >> 8;
	else
		delim = ' ';

	if ((flags & HD_COLUMN_MASK) != 0)
		cols = flags & HD_COLUMN_MASK;
	else
		cols = 16;

	cp = ptr;
	for (i = 0; i < length; i+= cols) {
		if (hdr != NULL)
			printf("%s", hdr);

		if ((flags & HD_OMIT_COUNT) == 0)
			printf("%04x  ", i);

		if ((flags & HD_OMIT_HEX) == 0) {
			for (j = 0; j < cols; j++) {
				k = i + j;
				if (k < length)
					printf("%c%02x", delim, cp[k]);
				else
					printf("   ");
			}
		}

		if ((flags & HD_OMIT_CHARS) == 0) {
			printf("  |");
			for (j = 0; j < cols; j++) {
				k = i + j;
				if (k >= length)
					printf(" ");
				else if (cp[k] >= ' ' && cp[k] <= '~')
					printf("%c", cp[k]);
				else
					printf(".");
			}
			printf("|");
		}
		printf("\n");
	}
}
#endif /* _KERNEL */

void
sbuf_hexdump(struct sbuf *sb, const void *ptr, int length, const char *hdr,
	     int flags)
{
	int i, j, k;
	int cols;
	const unsigned char *cp;
	char delim;

	if ((flags & HD_DELIM_MASK) != 0)
		delim = (flags & HD_DELIM_MASK) >> 8;
	else
		delim = ' ';

	if ((flags & HD_COLUMN_MASK) != 0)
		cols = flags & HD_COLUMN_MASK;
	else
		cols = 16;

	cp = ptr;
	for (i = 0; i < length; i+= cols) {
		if (hdr != NULL)
			sbuf_printf(sb, "%s", hdr);

		if ((flags & HD_OMIT_COUNT) == 0)
			sbuf_printf(sb, "%04x  ", i);

		if ((flags & HD_OMIT_HEX) == 0) {
			for (j = 0; j < cols; j++) {
				k = i + j;
				if (k < length)
					sbuf_printf(sb, "%c%02x", delim, cp[k]);
				else
					sbuf_printf(sb, "   ");
			}
		}

		if ((flags & HD_OMIT_CHARS) == 0) {
			sbuf_printf(sb, "  |");
			for (j = 0; j < cols; j++) {
				k = i + j;
				if (k >= length)
					sbuf_printf(sb, " ");
				else if (cp[k] >= ' ' && cp[k] <= '~')
					sbuf_printf(sb, "%c", cp[k]);
				else
					sbuf_printf(sb, ".");
			}
			sbuf_printf(sb, "|");
		}
		sbuf_printf(sb, "\n");
	}
}

