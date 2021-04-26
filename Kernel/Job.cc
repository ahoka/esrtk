#include <Kernel/Job.hh>

using namespace Kernel;

Job::Job(void (* main)(void *), void * arg) :
   mainM(main),
   argM(arg)
{
}

Job::Job(const Job& other)
{
   if (&other == this)
   {
      return;
   }

   mainM = other.mainM;
   argM = other.argM;
}

Job::~Job()
{
}

void
Job::execute()
{
   mainM(argM);
}
