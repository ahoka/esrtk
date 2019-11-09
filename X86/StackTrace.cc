#include <StackTrace.hh>
#include <X86/PageDirectory.hh>

#include <cstdio>

typedef unsigned long reg_t;

void
StackTrace::printStackTrace(void* data, void* eip)
{
   reg_t* ebp = reinterpret_cast<reg_t*>(data);

   printf("Stack trace:");

   if (eip != 0)
   {
      printf(" 0x%lx", (long)eip);
   }

   if (ebp == 0 || !PageDirectory::isMapped((uint32_t)ebp))
   {
      printf(" Not available.\n");
   }
   else
   {
      while (ebp != 0 && PageDirectory::isMapped((uint32_t)ebp))
      {
         reg_t* ip = ebp + 1;

	 if (!PageDirectory::isMapped((uint32_t)ip))
	 {
	    break;
	 }

	 printf(" 0x%lx", *ip);

	 if (*ip == 0)
	 {
	    break;
	 }

	 ebp = reinterpret_cast<reg_t*>(*ebp);

      }
      printf("\n");
   }
}

void
StackTrace::printStackTrace()
{
   void* ebp;
   
   asm volatile("movl %%ebp, %0" : "=g"(ebp));

   printStackTrace(ebp, 0);
}
