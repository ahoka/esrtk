#include <Kernel/Heap.hh>

#include <cstddef>
#include <new>

void *operator new(std::size_t size)
{
   return Kernel::Heap::get().allocate(size);
}
 
void *operator new[](std::size_t size)
{
   return Kernel::Heap::get().allocate(size);
}
 
void operator delete(void* p)
{
   Kernel::Heap::get().deallocate(p);
}
 
void operator delete[](void* p)
{
   Kernel::Heap::get().deallocate(p);
}

void operator delete(void* p, std::size_t)
{
   Kernel::Heap::get().deallocate(p);
}
 
void operator delete[](void* p, std::size_t)
{
   Kernel::Heap::get().deallocate(p);
}
