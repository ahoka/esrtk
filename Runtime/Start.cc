extern "C" void
start()
{
   for (;;)
   {
      asm volatile("pause");
   }
}
