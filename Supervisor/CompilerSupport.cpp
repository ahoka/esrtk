#ifdef __GNUC__

// a pure virtual function is called
extern "C" void __cxa_pure_virtual()
{
    // do nothing
}

#else
#  error Compiler not supported.
#endif
