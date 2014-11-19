#include <Driver/Driver.hh>
#include <X86/Processor.hh>

class NmiWatchdog : public Driver
{
   NmiWatchdog();
   virtual ~NmiWatchdog();

   virtual int probe();
   virtual bool init();
   virtual bool finalize();
   virtual const char* name() const;
};

NmiWatchdog::NmiWatchdog()
{
}

NmiWatchdog::~NmiWatchdog()
{
}

int
NmiWatchdog::probe()
{
   return 1;
}

bool
NmiWatchdog::init()
{
   driverInfo("NmiWatchdog::init\n");

//   x86_wrmsr(0x186, 0x53003CUL);
//   x86_wrmsr(0x0c1, 0xffffffff80000000UL);

   return true;
}

bool
NmiWatchdog::finalize()
{
   return true;
}

const char*
NmiWatchdog::name() const
{
   return "NmiWatchdog";
}
