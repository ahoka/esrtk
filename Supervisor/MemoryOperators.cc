#include <stddef.h>

void *operator new(size_t /*size*/)
{
    return 0;
}
 
void *operator new[](size_t /*size*/)
{
    return 0;
}
 
void operator delete(void */*p*/)
{
}
 
void operator delete[](void */*p*/)
{
}
