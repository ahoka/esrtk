#include <Driver/Driver.hh>
#include <Driver/DriverManager.hh>
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
