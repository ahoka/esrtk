#include <Kernel/Heap.hh>

#include <cstddef>

extern "C" void *
malloc(size_t size)
{
   return Kernel::Heap::get().allocate(size);
}

extern "C" void
free(void* data)
{
   Kernel::Heap::get().deallocate(data);
}

extern "C" void *
realloc(void* data, size_t size)
{
   return Kernel::Heap::get().reallocate(data, size);
}
