#ifndef POWER_H
#define	POWER_H

class Power
{
public:
   static void halt() __attribute__((noreturn));
   static void reboot() __attribute__((noreturn));

private:
   Power() = delete;
   Power(const Power&) = delete;
   ~Power() = delete;
};

#endif
