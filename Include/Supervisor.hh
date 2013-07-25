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

#endif	/* SUPERVISOR_H */

