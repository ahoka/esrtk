/* 
 * File:   Supervisor.cpp
 * Author: edmmhka
 * 
 * Main class for the supervisor
 * 
 * Created on August 29, 2012, 2:57 PM
 */

#include "Supervisor.h"
#include "Debug.h"
#include <VgaConsole.h>

Supervisor::Supervisor()
{
}

Supervisor::Supervisor(const Supervisor& /*orig*/)
{
}

Supervisor::~Supervisor()
{
}

void
Supervisor::run()
{
   VgaConsole console;

   // console.putChar('0', 0, 0);
   // console.putChar('1', 0, 1);
   // console.putChar('2', 0, 2);
   // console.putChar('3', 0, 3);
   // console.putChar('1', 1, 0);
   // console.putChar('2', 2, 0);
   // console.putChar('3', 3, 0);

   console.Console::putChar('H');
   console.Console::putChar('e');
   console.Console::putChar('l');
   console.putChar('l');
   console.putChar('o');

//   *(unsigned short* )(0xb8000) = ('A' & 0xff) | (0x07 << 8);;
   for (;;)
   {
      asm volatile("pause");
   }

   //   KASSERT(1 == 1);
}
