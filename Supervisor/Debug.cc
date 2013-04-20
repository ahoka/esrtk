/* 
 * File:   Debug.cpp
 * Author: edmmhka
 * 
 * Created on August 17, 2012, 1:39 PM
 */

#include <Debug.hh>
#include <StackTrace.hh>
#include <Power.hh>
#include <stdio.h>

void
Debug::panic(const char* message)
{
   printf("Supervisor check: %s\n", message);

   StackTrace::printStackTrace();

   Power::halt();
}

