#include "SystemCall.h"

class Foo
{
public:
   Foo(const char* text)
      : t(text)
   {
   }

   ~Foo()
   {
   }
private:
   const char* t;
};

extern "C" void
start()
{
   syscall(0, (void*)0x123456);

   Foo foo("Hello");

   for (;;)
   {
      asm volatile("pause");
   }
}
