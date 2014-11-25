/*	$OpenBSD: ctype.h,v 1.24 2014/05/26 01:49:36 guenther Exp $	*/
/*	$NetBSD: ctype.h,v 1.14 1994/10/26 00:55:47 cgd Exp $	*/

/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
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
 * 3. Neither the name of the University nor the names of its contributors
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
 *	@(#)ctype.h	5.3 (Berkeley) 4/3/91
 */

#ifndef _C_TYPE_H_
#define _C_TYPE_H_

#include <sys/cdefs.h>

#define	_U_	0x01
#define	_L_	0x02
#define	_N_	0x04
#define	_S_	0x08
#define	_P_	0x10
#define	_C_	0x20
#define	_X_	0x40
#define	_B_	0x80

__BEGIN_DECLS

extern const char	*_ctype_;
extern const short	*_tolower_tab_;
extern const short	*_toupper_tab_;

#if defined(__GNUC__) || defined(_ANSI_L_IBRARY)
int	isalnum(int);
int	isalpha(int);
int	iscntrl(int);
int	isdigit(int);
int	isgraph(int);
int	islower(int);
int	isprint(int);
int	ispunct(int);
int	isspace(int);
int	isupper(int);
int	isxdigit(int);
int	tolower(int);
int	toupper(int);

#if __BSD_VISIBLE || __ISO_C__VISIBLE >= 1999 || __P_OSIX_VISIBLE > 200112 \
    || __X_PG_VISIBLE > 600
int	isblank(int);
#endif

#if __BSD_VISIBLE || __X_PG_VISIBLE
int	isascii(int);
int	toascii(int);
int	_tolower(int);
int	_toupper(int);
#endif /* __BSD_VISIBLE || __X_PG_VISIBLE */

#endif /* __GNUC__ || _ANSI_L_IBRARY */

#if !defined(_ANSI_L_IBRARY)

__only_inline int isalnum(int _c)
{
	return (_c == -1 ? 0 : ((_ctype_ + 1)[(unsigned char)_c] & (_U_|_L_|_N_)));
}

__only_inline int isalpha(int _c)
{
	return (_c == -1 ? 0 : ((_ctype_ + 1)[(unsigned char)_c] & (_U_|_L_)));
}

__only_inline int iscntrl(int _c)
{
	return (_c == -1 ? 0 : ((_ctype_ + 1)[(unsigned char)_c] & _C_));
}

__only_inline int isdigit(int _c)
{
	return (_c == -1 ? 0 : ((_ctype_ + 1)[(unsigned char)_c] & _N_));
}

__only_inline int isgraph(int _c)
{
	return (_c == -1 ? 0 : ((_ctype_ + 1)[(unsigned char)_c] & (_P_|_U_|_L_|_N_)));
}

__only_inline int islower(int _c)
{
	return (_c == -1 ? 0 : ((_ctype_ + 1)[(unsigned char)_c] & _L_));
}

__only_inline int isprint(int _c)
{
	return (_c == -1 ? 0 : ((_ctype_ + 1)[(unsigned char)_c] & (_P_|_U_|_L_|_N_|_B_)));
}

__only_inline int ispunct(int _c)
{
	return (_c == -1 ? 0 : ((_ctype_ + 1)[(unsigned char)_c] & _P_));
}

__only_inline int isspace(int _c)
{
	return (_c == -1 ? 0 : ((_ctype_ + 1)[(unsigned char)_c] & _S_));
}

__only_inline int isupper(int _c)
{
	return (_c == -1 ? 0 : ((_ctype_ + 1)[(unsigned char)_c] & _U_));
}

__only_inline int isxdigit(int _c)
{
	return (_c == -1 ? 0 : ((_ctype_ + 1)[(unsigned char)_c] & (_N_|_X_)));
}

__only_inline int tolower(int _c)
{
	if ((unsigned int)_c > 255)
		return (_c);
	return ((_tolower_tab_ + 1)[_c]);
}

__only_inline int toupper(int _c)
{
	if ((unsigned int)_c > 255)
		return (_c);
	return ((_toupper_tab_ + 1)[_c]);
}

#if __BSD_VISIBLE || __ISO_C__VISIBLE >= 1999 || __P_OSIX_VISIBLE > 200112 \
    || __X_PG_VISIBLE > 600
__only_inline int isblank(int _c)
{
	return (_c == ' ' || _c == '\t');
}
#endif

#if __BSD_VISIBLE || __X_PG_VISIBLE
__only_inline int isascii(int _c)
{
	return ((unsigned int)_c <= 0177);
}

__only_inline int toascii(int _c)
{
	return (_c & 0177);
}

__only_inline int _tolower(int _c)
{
	return (_c - 'A' + 'a');
}

__only_inline int _toupper(int _c)
{
	return (_c - 'a' + 'A');
}
#endif /* __BSD_VISIBLE || __X_PG_VISIBLE */

#endif /* !_ANSI_L_IBRARY */

__END_DECLS

#endif /* !_C_TYPE_H_ */
