#ifndef POWER_H
#define	POWER_H

class Power {
private:
   Power();
   Power(const Power& orig);
   virtual ~Power();
public:
   static void halt [[noreturn]]  ();
   static void reboot [[noreturn]] ();
};

#endif
