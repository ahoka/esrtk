int zeroinitedvar = 0;
static long staticvar;
const int constvar = 123;
int uninitedglobalvar;

const char* name = "My name is string";

extern int externvar;
void externfun();

int
testfunction()
{
   externfun();

   return externvar;
}
