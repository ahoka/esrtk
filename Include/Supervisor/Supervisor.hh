#ifndef SUPERVISOR_H
#define	SUPERVISOR_H

#include <Kernel/Thread.hh>

namespace Supervisor
{

class Supervisor
{
public:
   static void init();

   Supervisor() = default;
   ~Supervisor() = default;

private:
   void run();
   Supervisor(const Supervisor& orig) = delete;
};

}

#endif	/* SUPERVISOR_H */
