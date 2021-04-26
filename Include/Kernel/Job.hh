#ifndef JOB_HH
#define JOB_HH

#include <functional>

namespace Kernel
{
   class Job
   {
   public:
      Job(void (* main)(void *), void * arg);
      Job(const Job&);
      ~Job();

      void execute();

   private:
      void (* mainM)(void * arg);
      void * argM;
   };
};

#endif
