#include <Kernel/Heap.hh>

#include <cstddef>
#include <cstring>

extern "C" void *
malloc(size_t size)
{
   return Kernel::Heap::get().allocate(size);
}

extern "C" void *
calloc(size_t nelem, size_t elemsz)
{
   size_t size = nelem * elemsz;
   char* mem = (char*)Kernel::Heap::get().allocate(size);
   memset(mem, 0, size);

   return (void*)mem;
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
