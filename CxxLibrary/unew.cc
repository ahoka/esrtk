// This file is part of the uSTL library, an STL implementation.
//
// Copyright (c) 2005 by Mike Sharov <msharov@users.sourceforge.net>
// This file is free software, distributed under the MIT License.

#include "unew.h"

void* tmalloc (size_t n) throw (ustl::bad_alloc)
{
    void* p = malloc (n);
#ifdef HAVE_EXCEPTIONS
    if (!p)
	throw ustl::bad_alloc (n);
#endif
    return (p);
}
