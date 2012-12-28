#ifdef __GNUC__

// a pure virtual function is called
extern "C" void __cxa_pure_virtual()
{
    // do nothing
}

// atexit == __cxa_atexit(f, 0, 0);
extern "C" int __cxa_atexit(void (*f)(void *), void* p, void* d);
extern "C" void __cxa_finalize(void* d);

#else
#  error Compiler not supported.
#endif
