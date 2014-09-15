#ifndef JOB_HH
#define JOB_HH

namespace Kernel
{
   class Job
   {
   public:
      using Function = void (*)(void*);

      Job();
      Job(Function f, void* arg);
      Job(const Job& other);
      ~Job();

      void execute();

   private:
      Function functionM;
      void* argM;
   };
};

#endif
