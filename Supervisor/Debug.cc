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
Debug::panic(const char* message, ...)
{
   va_list va;
   va_start(va, message);

   printf("Supervisor Error: ");
   vprintf(message, va);
   printf("\n");

   va_end(va);

   StackTrace::printStackTrace();

   Power::halt();
}

