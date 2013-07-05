#ifndef SYSTEMCONTROL_HH
#define SYSTEMCONTROL_HH

#include <Register.hh>

namespace CR0
{
   enum
   {
      PE = BIT(0),
      MP = BIT(1),
      EM = BIT(2),
      TS = BIT(3),
      ET = BIT(4),
      NE = BIT(5),
      WP = BIT(16),
      AM = BIT(18),
      NW = BIT(29),
      CD = BIT(30),
      PG = BIT(31)
   };
}

#endif
