#ifndef _COLOR_H_
#define _COLOR_H_

class Console {
public:
   Console();
   virtual ~Console();

   void putChar(char ch);
   void write(const char* string);
   virtual void putChar(int ch, int row, int column) = 0;

protected:
   virtual int getRows() = 0;
   virtual int getColumns() = 0;
   virtual int getChar() = 0;

private:
   int currentRow;
   int currentColumn;
};

#endif
