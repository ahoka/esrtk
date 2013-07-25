#ifndef POWER_H
#define	POWER_H

class Power {
private:
   Power();
   Power(const Power& orig);
   virtual ~Power();
public:
   static void halt() __attribute__((noreturn));
   static void reboot() __attribute__((noreturn));
};

#endif
