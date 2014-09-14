#include <stdlib.h>

static unsigned int x;

void
srand(unsigned int seed)
{
   x = seed;
}

int
rand(void)
{
   x ^= x >> 13;
   x ^= x << 17;
   x ^= x >> 5;

   return abs(x);
}
