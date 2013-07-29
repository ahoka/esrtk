#ifndef SUPERVISOR_H
#define	SUPERVISOR_H

class Supervisor
{
public:
   Supervisor();
   virtual ~Supervisor();

   void run() __attribute__((noreturn));
private:
   Supervisor(const Supervisor& orig);
};

extern "C" void initSupervisor();

#endif	/* SUPERVISOR_H */

