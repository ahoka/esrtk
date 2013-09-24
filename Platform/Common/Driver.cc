#include <Driver.hh>
#include <DriverManager.hh>
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
