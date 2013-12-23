#ifndef STACKTRACE_HH
#define STACKTRACE_HH

class StackTrace
{
public:
   static void printStackTrace();
   static void printStackTrace(void* data);
};

#endif
