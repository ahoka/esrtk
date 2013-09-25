#ifndef PLATFORM_HH
#define PLATFORM_HH

class Platform
{
public:
   static void init();
private:
   Platform();
   Platform(const Platform& orig);
   ~Platform();
};

#endif
