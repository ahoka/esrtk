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

   setCursor(currentRow, currentColumn);

   return retval;
}

int
Console::write(const char* str, int size)
{
   for (int i = 0; i < size; i++)
   {
      putChar(*str++);
   }

   setCursor(currentRow, currentColumn);

   return size;
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
   PRINTF_MODIFIER_LONG = (1 << 0),
   PRINTF_MODIFIER_LONGLONG = (1 << 1),
   PRINTF_MODIFIER_SHORT = (1 << 2),
   PRINTF_MODIFIER_SHORTSHORT = (1 << 3),
};

enum {
   PRINTF_FLAGS_THOUSAND = (1 << 0),
   PRINTF_FLAGS_LEFTJUSTIFIED = (1 << 1),
   PRINTF_FLAGS_SIGN = (1 << 2),
   PRINTF_FLAGS_SPACE = (1 << 3),
   PRINTF_FLAGS_ALTERNATIVE = (1 << 4),
   PRINTF_FLAGS_ZEROPADDING = (1 << 5)
};

enum {
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

#define PRINTF_PUTCHAR(x) (putChar(x), retval++)

unsigned long
getUnsignedFromVa(va_list* ap, int modifiers)
{
      // no long long for now
      unsigned long n = 0;
      if (modifiers & PRINTF_MODIFIER_LONG)
      {
	 n = va_arg(*ap, unsigned long);
      }
      else if (modifiers & PRINTF_MODIFIER_LONGLONG)
      {
	 //n = va_arg(*ap, unsigned long long);
	 unsigned long tmp = va_arg(*ap, unsigned long);
	 n = va_arg(*ap, unsigned long);
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

long
getSignedFromVa(va_list* ap, int modifiers)
{
      // no long long for now
      long n = 0;

      if (modifiers & PRINTF_MODIFIER_LONG)
      {
	 n = va_arg(*ap, long);
      }
      else if (modifiers & PRINTF_MODIFIER_LONGLONG)
      {
	 //n = va_arg(*ap,  long long);
	 long tmp = va_arg(*ap, long);
	 n = va_arg(*ap, long);
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

int
Console::doVaPrint(va_list* ap, int type, int modifiers, int flags)
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

      putChar('0');
      putChar('x');
      retval += 2;
      
      for (unsigned int i = 0; i < sizeof(p) * 2; i++)
      {
	 *bufp++ = hexl[p & 0x0f];
	 p >>= 4;
      }
   }

   if (type == PRINTF_TYPE_UNSIGNED || type == PRINTF_TYPE_DECIMAL)
   {
      unsigned long n;

      if (type == PRINTF_TYPE_DECIMAL)
      {
	 long sn = getSignedFromVa(ap, modifiers);
	 if (sn < 0)
	 {
	    PRINTF_PUTCHAR('-');
	    n = sn * -1;
	 } else {
	    if (flags & PRINTF_FLAGS_SIGN)
	    {
	       PRINTF_PUTCHAR('+');
	    }
	    else if (flags & PRINTF_FLAGS_SPACE)
	    {
	       PRINTF_PUTCHAR(' ');
	    }
	    n = sn;
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

      unsigned long n = getUnsignedFromVa(ap, modifiers);

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

      do
      {
	 *bufp++ = hex[n & 0x0f];
	 n >>= 4;
      }
      while (n != 0);
   }
   else if (type == PRINTF_TYPE_OCTAL)
   {
      unsigned long n = getUnsignedFromVa(ap, modifiers);
      
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
	       case 'l':
		  if (modifiers & PRINTF_MODIFIER_LONGLONG ||
		      modifiers & PRINTF_MODIFIER_SHORT)
		  {
		     finished = true;
		     // TODO print all stuff interpreted so far
		     PRINTF_PUTCHAR(*format++);
		     // putChar(*format++);
		     // retval++;
		  }
		  else if (modifiers & PRINTF_MODIFIER_LONG)
		  {
		     //modifiers |= PRINTF_MODIFIER_LONGLONG;
		     modifiers = 0;
		     finished = true;
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
		     // putChar(*format++);
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

   va_end(ap);

   setCursor(currentRow, currentColumn);
   return retval;
}
