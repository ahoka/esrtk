#include <cstdint>
#include <cstdio>
#include <cstring>

extern uintptr_t ap_startup_routine;
extern size_t ap_startup_routine_size;

void
boot_aps()
{
   printf("Startup Code: %p (%zu)\n", (void*)ap_startup_routine, ap_startup_routine_size);
}
