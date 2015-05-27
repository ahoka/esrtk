extern "C" void
start()
{
   asm volatile("cli");

   for (;;)
   {
      asm volatile("pause");
   }
}
