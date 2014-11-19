#include <Driver/Driver.hh>
#include <X86/Processor.hh>

class NmiWatchdog : public Driver
{
public:
   NmiWatchdog();
   virtual ~NmiWatchdog();

   virtual int probe();
   virtual bool init();
   virtual bool finalize();
   virtual const char* name() const;
};

NmiWatchdog nmiWatchdog;

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

   x86_wrmsr(0x186, 0x53003CUL);
   x86_wrmsr(0x0c1, 0xffffffff80000000UL);

   for (int i = 0; i < 3; i++)
   {
//      uint64_t pmc = x86_rdpmc(0);
      uint64_t pmc = x86_rdmsr(0x0c1);
      driverInfo("PMC: %llu\n", pmc);
   }

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
