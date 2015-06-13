#include <Kernel/Job.hh>

using namespace Kernel;

Job::Job(const std::function<void()>& f)
   : functionM(f)
{
}

Job::Job(const Job& other)
{
   if (&other == this)
   {
      return;
   }

   functionM = other.functionM;
}

Job::~Job()
{
}

void
Job::execute()
{
   functionM();
}
