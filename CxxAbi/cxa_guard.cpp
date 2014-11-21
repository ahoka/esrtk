//===---------------------------- cxa_guard.cpp ---------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

// Heavily modified for ESRTK
//

#include <Debug.hh>
#include <spinlock.h>
#include <stdint.h>

namespace __cxxabiv1
{

namespace
{

typedef uint64_t guard_type;

bool is_initialized(guard_type* guard_object) {
    char* initialized = (char*)guard_object;
    return *initialized;
}

void set_initialized(guard_type* guard_object) {
    char* initialized = (char*)guard_object;
    *initialized = 1;
}

spinlock_t guard_mut = SPINLOCK_STATIC_INITIALIZER;

typedef bool lock_type;

inline
lock_type
get_lock(uint64_t x)
{
    union
    {
        uint64_t guard;
        uint8_t lock[2];
    } f = {x};
    return f.lock[1] != 0;
}

inline
void
set_lock(uint64_t& x, lock_type y)
{
    union
    {
        uint64_t guard;
        uint8_t lock[2];
    } f = {0};
    f.lock[1] = y;
    x = f.guard;
}

}  // unnamed namespace

extern "C"
{

int __cxa_guard_acquire(guard_type* guard_object)
{
    char* initialized = (char*)guard_object;
    if (spinlock_enter(&guard_mut) != 0)
       Debug::panic("__cxa_guard_acquire failed to acquire mutex");
    int result = *initialized == 0 ? 1 : 0;
    if (result)
    {
        while (get_lock(*guard_object))
        {
           asm volatile("pause");
        }
        result = *initialized == 0;
        if (result)
            set_lock(*guard_object, true);
    }
    if (spinlock_exit(&guard_mut) != 1)
        Debug::panic("__cxa_guard_acquire failed to release mutex");
    return result;
}

void __cxa_guard_release(guard_type* guard_object)
{
    if (spinlock_enter(&guard_mut) != 0)
        Debug::panic("__cxa_guard_release failed to acquire mutex");
    *guard_object = 0;
    set_initialized(guard_object);
    if (spinlock_exit(&guard_mut) != 1)
        Debug::panic("__cxa_guard_release failed to release mutex");
}

void __cxa_guard_abort(guard_type* guard_object)
{
    if (spinlock_enter(&guard_mut) != 0)
        Debug::panic("__cxa_guard_abort failed to acquire mutex");
    *guard_object = 0;
    if (spinlock_exit(&guard_mut) != 1)
        Debug::panic("__cxa_guard_abort failed to release mutex");
}

}  // extern "C"

}  // __cxxabiv1
