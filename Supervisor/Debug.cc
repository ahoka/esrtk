/* 
 * File:   Debug.cpp
 * Author: edmmhka
 * 
 * Created on August 17, 2012, 1:39 PM
 */

#include <Debug.hh>
#include <StackTrace.hh>
#include <Power.hh>

static void
panic(const char* message)
{
   printf("Supervisor check: %s\n", message);

   StackTrace::printStackTrace();

   Power::halt();
}

