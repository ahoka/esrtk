#ifdef __GNUC__

#include <CompilerSupport.hh>
#include <cstdio>
#include <cstdint>

#define DEBUG

// a pure virtual function is called
void
__cxa_pure_virtual()
{
   // do nothing
   // panic("pure virtual method was called\n");
}

void* __dso_handle = 0;

const int __cxaimpl_atexit_size = 128;
static __cxaimpl_atexit_container __cxaimpl_atexit_list[__cxaimpl_atexit_size];
int __cxaimpl_atexit_nextfree = 0;
static bool __cxaimpl_atexit_firstrun = true;

// atexit() == __cxa_atexit(f, 0, 0);
int
__cxa_atexit(void (*destructor)(void*), void* argument, void* dso)
{
   if (destructor == 0)
   {
      return 1;
   }

   // init to zero on first usage
   if (__cxaimpl_atexit_firstrun)
   {
      for (int i = 0; i < __cxaimpl_atexit_size; i++)
      {
	 __cxaimpl_atexit_list[i].destructor = 0;
	 __cxaimpl_atexit_list[i].argument = 0;
	 __cxaimpl_atexit_list[i].dso = 0;
      }
      __cxaimpl_atexit_firstrun = false;
   }

   bool had_free = false;
   for (int i = 0; i < __cxaimpl_atexit_size; i++)
   {
      if (__cxaimpl_atexit_list[i].destructor == 0)
      {
	 had_free = true;
	 __cxaimpl_atexit_list[i].destructor = destructor;
	 __cxaimpl_atexit_list[i].argument = argument;
	 __cxaimpl_atexit_list[i].dso = dso;
      }
   }

   if (!had_free)
   {
      // panic("Maximum number of atexit functions or destructors reached!\n");
      return 1;
   }
   
   return 0;
}

void
__cxa_finalize(void* dso)
{
   if (__cxaimpl_atexit_firstrun)
   {
      // nothing to do here
      return;
   }

   // call all destructors
   if (dso == 0)
   {
      for (int i = __cxaimpl_atexit_size - 1; i >= 0; i--)
      {
	 if (__cxaimpl_atexit_list[i].destructor != 0)
	 {
	    void* arg = __cxaimpl_atexit_list[i].argument;
	    __cxaimpl_atexit_function dtor = __cxaimpl_atexit_list[i].destructor;
	    
	    dtor(arg);
	    
	    __cxaimpl_atexit_list[i].destructor = 0;
	    __cxaimpl_atexit_list[i].argument = 0;
	    __cxaimpl_atexit_list[i].dso = 0;
	 }
      }
   }

   for (int i = 0; i < __cxaimpl_atexit_size; i++)
   {
      if (__cxaimpl_atexit_list[i].dso == dso)
      {
	 void* arg = __cxaimpl_atexit_list[i].argument;
	 __cxaimpl_atexit_function dtor = __cxaimpl_atexit_list[i].destructor;

	 dtor(arg);

	 __cxaimpl_atexit_list[i].destructor = 0;
	 __cxaimpl_atexit_list[i].argument = 0;
	 __cxaimpl_atexit_list[i].dso = 0;
      }
   }
}

#else
#  error Compiler not supported.
#endif

void
__cxaimpl_call_constructors()
{
#ifdef DEBUG
   printf("%p -> %p\n", &__start_ctors, &__end_ctors);
#endif
   for (__cxaimpl_constructor* c = &__start_ctors; c != &__end_ctors; c++)
   {
      printf("Calling constructor at %p\n", *c);
      (*c)();
   }
}

extern uintptr_t __start_bss;
extern uintptr_t __end_bss;

void
__cxaimpl_zero_bss()
{
   for (uint8_t* p = (uint8_t* )__start_bss; p != (uint8_t* )__end_bss; p++)
   {
      *p = 0;
   }
}
