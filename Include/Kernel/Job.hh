#ifndef JOB_HH
#define JOB_HH

#include <functional>

namespace Kernel
{
   class Job
   {
   public:
      Job(const std::function<void()>&);
      Job(const Job&);
      ~Job();

      void execute();

   private:
      std::function<void()> functionM;
   };
};

#endif
