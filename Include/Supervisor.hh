#ifndef SUPERVISOR_H
#define	SUPERVISOR_H

class Supervisor
{
public:
   static void init();
   static void run [[noreturn]] ();
private:
   Supervisor();
   virtual ~Supervisor();
   Supervisor(const Supervisor& orig);
};

extern "C" void supervisor_init();

#endif	/* SUPERVISOR_H */

