#include <StdioSupport.hh>

#include <stdarg.h>
#include <stdio.h>
#include <stddef.h>
#include <stdbool.h>

#define HAS_LONGLONG

#ifdef HAS_LONGLONG
typedef unsigned long long printf_uint_t;
typedef long long printf_int_t;
#else
typedef unsigned long printf_uint_t;
typedef long printf_int_t;
#endif

enum
{
   PRINTF_MODIFIER_LONG = (1 << 0),
   PRINTF_MODIFIER_LONGLONG = (1 << 1),
   PRINTF_MODIFIER_SHORT = (1 << 2),
   PRINTF_MODIFIER_SHORTSHORT = (1 << 3),
};

enum
{
   PRINTF_FLAGS_THOUSAND = (1 << 0),
   PRINTF_FLAGS_LEFTJUSTIFIED = (1 << 1),
   PRINTF_FLAGS_SIGN = (1 << 2),
   PRINTF_FLAGS_SPACE = (1 << 3),
   PRINTF_FLAGS_ALTERNATIVE = (1 << 4),
   PRINTF_FLAGS_ZEROPADDING = (1 << 5)
};

enum
{
   PRINTF_TYPE_INVALID,
   PRINTF_TYPE_DECIMAL,
   PRINTF_TYPE_UNSIGNED,
   PRINTF_TYPE_LOWERHEX,
   PRINTF_TYPE_UPPERHEX,
   PRINTF_TYPE_OCTAL,
   PRINTF_TYPE_STRING,
   PRINTF_TYPE_CHARACTER,
   PRINTF_TYPE_POINTER
};

int
putchar(int c)
{

/*    if (serialConsole) */
/*    { */
/*       SerialConsole::putChar(c); */
/*    } */

/*    if (!noVga) */
/*    { */
/* //      return System::console.putChar(c); */
/*    } */

   return system_putchar(c);
}

int
puts(const char* string)
{
/*    int ret = 0; */

/*    if (!noVga) */
/*    { */
/* //      ret = System::console.putString(string); */
/*    } */

/*    if (serialConsole) */
/*    { */
/*       while (*string) */
/*       { */
/*          SerialConsole::putChar(*string++); */
/*       } */
/*    } */

   return system_puts(string);
}

#define PRINTF_PUTCHAR(x) (putchar(x), retval++)

static printf_uint_t
getUnsignedFromVa(va_list* ap, int modifiers)
{
      printf_uint_t n = 0;
      if (modifiers & PRINTF_MODIFIER_LONG)
      {
	 n = va_arg(*ap, unsigned long);
      }
      else if (modifiers & PRINTF_MODIFIER_LONGLONG)
      {
	 n = va_arg(*ap, printf_uint_t);
//         n |= va_arg(*ap, unsigned long) << 32;
//	 unsigned long tmp = va_arg(*ap, unsigned long);
// 	 n = va_arg(*ap, unsigned long);
      }
      else if (modifiers & PRINTF_MODIFIER_SHORT)
      {
	 n = va_arg(*ap, unsigned int);
      }
      else if (modifiers & PRINTF_MODIFIER_SHORTSHORT)
      {
	 n = va_arg(*ap, unsigned int);
      }
      else
      {
	 n = va_arg(*ap, unsigned int);
      }
      
      return n;
}

static printf_int_t
getSignedFromVa(va_list* ap, int modifiers)
{
      printf_int_t n = 0;

      if (modifiers & PRINTF_MODIFIER_LONG)
      {
	 n = va_arg(*ap, long);
      }
      else if (modifiers & PRINTF_MODIFIER_LONGLONG)
      {
	 n = va_arg(*ap,  printf_int_t);
	 // long tmp = va_arg(*ap, long);
	 // n = va_arg(*ap, long);
      }
      else if (modifiers & PRINTF_MODIFIER_SHORT)
      {
	 n = va_arg(*ap, int);
      }
      else if (modifiers & PRINTF_MODIFIER_SHORTSHORT)
      {
	 n = va_arg(*ap, int);
      }
      else
      {
	 n = va_arg(*ap, int);
      }
      
      return n;
}

static int
doVaPrint(va_list* ap, int type, int modifiers, int flags)
{
   const char hexl[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		       'a', 'b', 'c', 'd', 'e', 'f' };

   const char hexu[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		       'A', 'B', 'C', 'D', 'E', 'F' };

   char printfBuffer[128];
   char* bufp = printfBuffer;
   int retval = 0;

   if (type == PRINTF_TYPE_POINTER)
   {
      // XXX uintptr_t ?
      unsigned long p = (unsigned long)va_arg(*ap, void*);

      putchar('0');
      putchar('x');
      retval += 2;
      
      for (unsigned int i = 0; i < sizeof(p) * 2; i++)
      {
	 *bufp++ = hexl[p & 0x0f];
	 p >>= 4;
      }
   }

   if (type == PRINTF_TYPE_UNSIGNED || type == PRINTF_TYPE_DECIMAL)
   {
      printf_uint_t n;

      if (type == PRINTF_TYPE_DECIMAL)
      {
      	 printf_int_t sn = getSignedFromVa(ap, modifiers);
	 if (sn < 0)
	 {
	    PRINTF_PUTCHAR('-');
	    n = (printf_uint_t )(sn * -1);
	 } else {
	    if (flags & PRINTF_FLAGS_SIGN)
	    {
	       PRINTF_PUTCHAR('+');
	    }
	    else if (flags & PRINTF_FLAGS_SPACE)
	    {
	       PRINTF_PUTCHAR(' ');
	    }
	    n = (printf_uint_t )sn;
	 }
      }
      else
      {
	 n = getUnsignedFromVa(ap, modifiers);
      }

      do
      {
	 *bufp++ = n % 10 + '0';
	 n /= 10;
      }
      while(n);

   }
   else if (type == PRINTF_TYPE_LOWERHEX || type == PRINTF_TYPE_UPPERHEX)
   {
      const char* hex;

      printf_uint_t n = getUnsignedFromVa(ap, modifiers);

      if (type == PRINTF_TYPE_LOWERHEX)
      {
	 if (flags & PRINTF_FLAGS_ALTERNATIVE && n != 0)
	 {
	    PRINTF_PUTCHAR('0');
	    PRINTF_PUTCHAR('x');
	 }
	 hex = hexl;
      }
      else
      {
	 if (flags & PRINTF_FLAGS_ALTERNATIVE && n != 0)
	 {
	    PRINTF_PUTCHAR('0');
	    PRINTF_PUTCHAR('X');
	 }
	 hex = hexu;
      }

      int count = 0;
      do
      {
	 *bufp++ = hex[n & 0x0f];
	 n >>= 4;
	 count++;
      }
      while (n != 0);

      if (flags & PRINTF_FLAGS_ZEROPADDING)
      {
	 int pad;
	 if (modifiers & PRINTF_MODIFIER_SHORTSHORT)
	 {
	    pad = 2;
	 }
	 else if (modifiers & PRINTF_MODIFIER_SHORT)
	 {
	    pad = 4;
	 }
	 else if ((modifiers & PRINTF_MODIFIER_LONG && (sizeof (long) == 16)) ||
                  modifiers & PRINTF_MODIFIER_LONGLONG)
         {
	    pad = 16;
	 }
	 else
	 {
	    pad = 8;
	 }

	 if (pad - count > 0)
	 {
	    for (int i = 0; i < pad - count; i++)
	    {
	       PRINTF_PUTCHAR('0');
	    }
	 }
      }
   }
   else if (type == PRINTF_TYPE_OCTAL)
   {
      printf_uint_t n = getUnsignedFromVa(ap, modifiers);
      
      // XXX this will be incorrect when we have field width!
      if (flags & PRINTF_FLAGS_ALTERNATIVE && n != 0)
      {
	 PRINTF_PUTCHAR('0');
      }

      do
      {
	 *bufp++ = (n & 07) + '0';
	 n >>= 3;
      }
      while (n != 0);
   }
   else if (type == PRINTF_TYPE_STRING)
   {
      const char* str = va_arg(*ap, const char*);
      retval += puts(str);
   }
   else if (type == PRINTF_TYPE_CHARACTER)
   {
      unsigned char ch = (unsigned char )va_arg(*ap, int);
      putchar(ch);
      retval += 1;
   }

   if (bufp != printfBuffer)
   {
      bufp--;
      do
      {
	 PRINTF_PUTCHAR(*bufp);
      }
      while (bufp-- != printfBuffer);
   }

   return retval;
}

int
printf(const char* format, ...)
{
   int retval;
   va_list ap;

   va_start(ap, format);

   retval = vprintf(format, ap);

   va_end(ap);

   return retval;
}

int
vprintf(const char* format, va_list ap)
{
   int retval = 0;

   while (*format != 0)
   {
      if (*format != '%')
      {
	 putchar(*format++);
	 retval++;
      }
      else
      {
	 int modifiers = 0;
	 int flags = 0;
	 int type = PRINTF_TYPE_INVALID;
	 bool finished = false;
	 while (*++format != 0 && !finished)
	 {
	    switch (*format)
	    {
	       case '\'':
		  flags |= PRINTF_FLAGS_THOUSAND;
		  break;
	       case '-':
		  flags |= PRINTF_FLAGS_LEFTJUSTIFIED;
		  break;
	       case '+':
		  flags |= PRINTF_FLAGS_SIGN;
		  break;
	       case ' ':
		  flags |= PRINTF_FLAGS_SPACE;
		  break;
	       case '#':
		  flags |= PRINTF_FLAGS_ALTERNATIVE;
		  break;
	       case '0':
		  flags |= PRINTF_FLAGS_ZEROPADDING;
		  break;
	       case '1':
	       case '2':
	       case '3':
	       case '4':
	       case '5':
	       case '6':
	       case '7':
	       case '8':
	       case '9':
		  // ignored for now...
		  // did i mention i love fallthrough?
		  break;
	       case 'l':
		  if (modifiers & PRINTF_MODIFIER_LONGLONG ||
		      modifiers & PRINTF_MODIFIER_SHORT)
		  {
		     finished = true;
		     // TODO print all stuff interpreted so far
		     PRINTF_PUTCHAR(*format++);
		     // putchar(*format++);
		     // retval++;
		  }
		  else if (modifiers & PRINTF_MODIFIER_LONG)
		  {
		     modifiers |= PRINTF_MODIFIER_LONGLONG;
//		     modifiers = 0;
//		     finished = true;
		  }
		  else
		  {
		     modifiers |= PRINTF_MODIFIER_LONG;
		  }
		  break;
	       case 'h':
		  if (modifiers & PRINTF_MODIFIER_LONG ||
		      modifiers & PRINTF_MODIFIER_SHORTSHORT)
		  {
		     finished = true;
		     // TODO print all stuff interpreted so far
		     PRINTF_PUTCHAR(*format++);
		     // putchar(*format++);
		     // retval++;
		  }
		  else if (modifiers & PRINTF_MODIFIER_SHORT)
		  {
		     modifiers |= PRINTF_MODIFIER_SHORTSHORT;
		  }
		  else
		  {
		     modifiers |= PRINTF_MODIFIER_SHORT;
		  }
		  break;
	       case 'd':
	       case 'i':
		  type = PRINTF_TYPE_DECIMAL;
		  finished = true;
		  break;
	       case 'u':
		  type = PRINTF_TYPE_UNSIGNED;
		  finished = true;
		  break;
	       case 'x':
		  type = PRINTF_TYPE_LOWERHEX;
		  finished = true;
		  break;
	       case 'X':
		  type = PRINTF_TYPE_UPPERHEX;
		  finished = true;
		  break;
	       case 'o':
		  type = PRINTF_TYPE_OCTAL;
		  finished = true;
		  break;
	       case 'p':
		  type = PRINTF_TYPE_POINTER;
		  finished = true;
		  break;
	       case 's':
		  // l modifier is valid, but unimplemented
		  if (modifiers == PRINTF_MODIFIER_LONG || modifiers == 0)
		  {
		     type = PRINTF_TYPE_STRING;
		  }
		  else
		  {
		     type = PRINTF_TYPE_INVALID;
		  }
		  finished = true;
		  break;
	       case 'c':
		  // l modifier is valid, but unimplemented
		  if (modifiers == PRINTF_MODIFIER_LONG || modifiers == 0)
		  {
		     type = PRINTF_TYPE_CHARACTER;
		  }
		  else
		  {
		     type = PRINTF_TYPE_INVALID;
		  }
		  finished = true;
		  break;
	       default:
		  // something unexpected
		  // just print whatever it was...
		  finished = true;
		  PRINTF_PUTCHAR(*format++);
	    }

	    // do the actual conversion
	    if (finished)
	    {
	       retval += doVaPrint(&ap, type, modifiers, flags);
	    }
	 }
      }
   }

   // // XXX does not belong here
   // if (retval != 0)
   // {
   //    Console::setCursor(Console::currentRow, currentColumn);
   // }
   return retval;
}


// int printf(const char* format, ...)
// {
//    va_list ap;
//    int retval;
   
//    va_start(ap, format);
   
//    retval = System::console.vprintf(format, ap);
   
//    va_end(ap);
   
//    return retval;
// }

// int vprintf(const char* format, va_list ap)
// {
//    int retval;

//    retval = System::console.vprintf(format, ap);

//    return retval;
// }


