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

extern bool noVga;

void
Debug::panic(const char* message, ...)
{
   va_list va;
   va_start(va, message);

   noVga = true;

   printf("Supervisor Error: ");
   vprintf(message, va);
   printf("\n");

   va_end(va);

   StackTrace::printStackTrace();

   printf("\nYour adventure ends here.\n");

   Power::halt();
}

