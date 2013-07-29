#ifndef COMPILER_SUPPORT_HH
#define COMPILER_SUPPORT_HH

extern "C" void __cxa_pure_virtual();
extern "C" int __cxa_atexit(void (*)(void*), void*, void*);
extern "C" void __cxa_finalize(void*);

extern "C" void __cxaimpl_call_constructors();

extern int __cxaimpl_atexit_nextfree;
extern void* __dso_handle;

typedef void (*__cxaimpl_atexit_function)(void*);

struct __cxaimpl_atexit_container
{
   __cxaimpl_atexit_function destructor;
   void* argument;
   void* dso;
};

typedef void (*__cxaimpl_constructor)();

extern __cxaimpl_constructor __start_ctors;
extern __cxaimpl_constructor __end_ctors;

#endif

