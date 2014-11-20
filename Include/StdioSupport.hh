#ifndef STDIOSUPPORT_HH
#define STDIOSUPPORT_HH

#ifdef __cplusplus
class Console;

namespace KernelStdio
{
   void registerConsole(Console *);
   void unregisterConsole(Console *);
};
#endif

#ifdef __cplusplus
extern "C"
{
#endif

int system_putchar(int);
int system_puts(const char *);

#ifdef __cplusplus
}
#endif

#endif
