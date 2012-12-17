#include "Console.h"

#include <stdarg.h>

Console::Console()
  : currentRow(0),
    currentColumn(0)
{
}

Console::~Console()
{
}

int
Console::putString(const char* str)
{
   int retval = 0;

   while (*str)
   {
      putChar(*str++);
      retval++;
   }

   return retval;
}

void
Console::stepRow()
{
   currentRow++;
  
   if (currentRow > getRows() - 1)
   {
      // generic scroll or smart scroll by driver?
      scrollScreen();
      currentColumn = 0;
      currentRow = getRows() - 1; // just flow over to the top now
      setCursor(getRows() - 1, 0);
   }
}

void
Console::stepColumn()
{
      currentColumn++;
      if (currentColumn > getColumns() - 1) {
	 currentColumn = 0;
	 stepRow();
      }
}

#define TABSIZE 8

void
Console::putChar(char ch)
{
   switch (ch)
   {
      case '\n':
	 stepRow();
	 currentColumn = 0;
	 break;
      case '\r':
	 break;
      case '\t':
      {
	 // TODO Refactor!
	 int newcol = ((currentColumn + TABSIZE) / TABSIZE) * TABSIZE;
	 int steps = newcol - currentColumn;
	 for (int i = 0; i < steps; i++)
	 {
	    stepColumn();
	 }
	 break;
      }
      default:
	 putChar(ch, currentRow, currentColumn);
	 stepColumn();
	 break;
   }
}

enum {
   PRINTF_FLAG_LONG = 1 << 5,
   PRINTF_FLAG_LONGLONG = 1 << 6,
   PRINTF_FLAG_SHORT = 1 << 7,
   PRINTF_FLAG_SHORTSHORT = 1 << 8,
};

enum {
   PRINTF_TYPE_INVALID,
   PRINTF_TYPE_DECIMAL,
   PRINTF_TYPE_UNSIGNED,
   PRINTF_TYPE_LOWERHEX,
   PRINTF_TYPE_UPPERHEX,
   PRINTF_TYPE_STRING,
   PRINTF_TYPE_CHARACTER
};

#define PRINTF_PUTCHAR(x) (putChar(x), retval++)

unsigned long
getUnsignedFromVa(va_list* ap, int flags, unsigned long* high)
{
      // no long long for now
      unsigned long n = 0;
      if (flags & PRINTF_FLAG_LONG)
      {
	 n = va_arg(*ap, unsigned long);
      }
      else if (flags & PRINTF_FLAG_LONGLONG)
      {
	 //n = va_arg(*ap, unsigned long long);
	 unsigned long tmp = va_arg(*ap, unsigned long);
	 n = va_arg(*ap, unsigned long);

	 if (high != 0)
	 {
	    *high = tmp;
	 }
      }
      else if (flags & PRINTF_FLAG_SHORT)
      {
	 n = va_arg(*ap, unsigned int);
      }
      else if (flags & PRINTF_FLAG_SHORTSHORT)
      {
	 n = va_arg(*ap, unsigned int);
      }
      else
      {
	 n = va_arg(*ap, unsigned int);
      }
      
      return n;
}

long
getSignedFromVa(va_list* ap, int flags, long* high)
{
      // no long long for now
      long n = 0;

      if (flags & PRINTF_FLAG_LONG)
      {
	 n = va_arg(*ap, long);
      }
      else if (flags & PRINTF_FLAG_LONGLONG)
      {
	 //n = va_arg(*ap,  long long);
	 long tmp = va_arg(*ap, long);
	 n = va_arg(*ap, long);

	 if (high != 0)
	 {
	    *high = tmp;
	 }
      }
      else if (flags & PRINTF_FLAG_SHORT)
      {
	 n = va_arg(*ap, int);
      }
      else if (flags & PRINTF_FLAG_SHORTSHORT)
      {
	 n = va_arg(*ap, int);
      }
      else
      {
	 n = va_arg(*ap, int);
      }
      
      return n;
}

int
Console::doVaPrint(va_list* ap, int type, int flags)
{
   const char hexl[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		       'a', 'b', 'c', 'd', 'e', 'f' };

   const char hexu[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
		       'A', 'B', 'C', 'D', 'E', 'F' };

   char* bufp = printfBuffer;
   int retval = 0;

   if (type == PRINTF_TYPE_UNSIGNED || type == PRINTF_TYPE_DECIMAL)
   {
      unsigned long n;

      if (type == PRINTF_TYPE_DECIMAL)
      {
	 long sn = getSignedFromVa(ap, flags, 0);
	 if (sn < 0)
	 {
	    PRINTF_PUTCHAR('-');
	    n = sn * -1;
	 } else {
	    n = sn;
	 }
      }
      else
      {
	 n = getUnsignedFromVa(ap, flags, 0);
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

      unsigned long high = 0;
      unsigned long n = getUnsignedFromVa(ap, flags, &high);

      if (type == PRINTF_TYPE_LOWERHEX)
      {
	 hex = hexl;
      }
      else
      {
	 hex = hexu;
      }

      do
      {
	 *bufp++ = hex[n & 0x0f];
	 n >>= 4;
      }
      while (n != 0);

      if (high != 0)
      {
	 do
	 {
	    *bufp++ = hex[high & 0x0f];
	    high >>= 4;
	 }
	 while (high != 0);
      }
   }
   else if (type == PRINTF_TYPE_STRING)
   {
      const char* str = va_arg(*ap, const char*);
      retval += putString(str);
   }
   else if (type == PRINTF_TYPE_CHARACTER)
   {
      unsigned char ch = va_arg(*ap, int);
      putChar(ch);
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
Console::printf(const char* format, ...)
{
   va_list ap;
   int retval = 0;

   va_start(ap, format);

   while (*format != 0)
   {
      if (*format != '%')
      {
	 putChar(*format++);
	 retval++;
      }
      else
      {
	 int flags = 0;
	 int type = PRINTF_TYPE_INVALID;
	 bool finished = false;
	 while (*++format != 0 && !finished)
	 {
	    switch (*format)
	    {
	       case 'l':
		  if (flags & PRINTF_FLAG_LONGLONG ||
		      flags & PRINTF_FLAG_SHORT)
		  {
		     finished = true;
		     // TODO print all stuff interpreted so far
		     PRINTF_PUTCHAR(*format++);
		     // putChar(*format++);
		     // retval++;
		  }
		  else if (flags & PRINTF_FLAG_LONG)
		  {
		     //flags |= PRINTF_FLAG_LONGLONG;
		     flags = 0;
		     finished = true;
		  }
		  else
		  {
		     flags |= PRINTF_FLAG_LONG;
		  }
		  break;
	       case 'h':
		  if (flags & PRINTF_FLAG_LONG ||
		      flags & PRINTF_FLAG_SHORTSHORT)
		  {
		     finished = true;
		     // TODO print all stuff interpreted so far
		     PRINTF_PUTCHAR(*format++);
		     // putChar(*format++);
		     // retval++;
		  }
		  else if (flags & PRINTF_FLAG_SHORT)
		  {
		     flags |= PRINTF_FLAG_SHORTSHORT;
		  }
		  else
		  {
		     flags |= PRINTF_FLAG_SHORT;
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
	       case 's':
		  // l modifier is valid, but unimplemented
		  if (flags == PRINTF_FLAG_LONG || flags == 0)
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
		  if (flags == PRINTF_FLAG_LONG || flags == 0)
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
	       retval += doVaPrint(&ap, type, flags);
	    }
	 }
      }
   }

   va_end(ap);

   setCursor(currentRow, currentColumn);
   return retval;
}
