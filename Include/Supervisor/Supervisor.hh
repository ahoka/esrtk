#ifndef SUPERVISOR_H
#define	SUPERVISOR_H

#include <Kernel/Thread.hh>

namespace Supervisor
{

class Supervisor
{
public:
   static void init();
   void run();

   Supervisor() = default;
   ~Supervisor() = default;

private:
   Supervisor(const Supervisor& orig) = delete;
};

}

#endif	/* SUPERVISOR_H */
