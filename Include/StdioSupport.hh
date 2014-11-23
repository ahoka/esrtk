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

int console_putchar(int);
int console_puts(const char *);

int console_feed(int c);

int console_getchar();

#ifdef __cplusplus
}
#endif

#endif
