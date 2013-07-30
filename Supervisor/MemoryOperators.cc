#include <cstddef>
#include <new>

void *operator new(std::size_t /*size*/)
{
    return 0;
}
 
void *operator new[](std::size_t /*size*/)
{
    return 0;
}
 
void operator delete(void */*p*/)
{
}
 
void operator delete[](void */*p*/)
{
}
