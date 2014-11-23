#include <Kernel/Job.hh>

using namespace Kernel;

Job::Job()
   : functionM(0),
     argM(0)
{
}

Job::Job(Function f, void* arg)
   : functionM(f),
     argM(arg)
{
}

Job::Job(const Job& other)
{
   if (&other == this)
   {
      return;
   }

   functionM = other.functionM;
   argM = other.argM;
}

Job::~Job()
{
}

void
Job::execute()
{
   functionM(argM);
}
