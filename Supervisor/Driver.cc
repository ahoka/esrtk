#include <Driver.hh>
#include <cstdio>

Driver::Driver() : 
   state(State::Uninitalized),
   next(0)
{
   DriverManager::registerDriver(this);
}

Driver::~Driver()
{
   // dummy
   state = State::Uninitalized;
}

Driver* DriverManager::driverList = 0;

bool
DriverManager::registerDriver(Driver* driver)
{
   printf("DriverManager: registering driver %p\n", driver);

   driver->next = driverList;
   driverList = driver;

   return true;
}

bool
DriverManager::deregisterDriver(Driver* /*driver*/)
{
   return true;
}

void
DriverManager::probeAndInit()
{
   Driver* driver = driverList;
   while (driver != 0)
   {
      driver->init();
      driver = driver->next;
   }
}
