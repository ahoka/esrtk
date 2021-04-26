#ifndef POWER_H
#define	POWER_H

namespace Power
{
   void pause();
   void halt();

   void shutdown() __attribute__((noreturn));

   void reboot() __attribute__((noreturn));

   void onIdle(void *);
};

#endif
