#ifndef FRAMEBUFFER_HH
#define FRAMEBUFFER_HH

#include <Driver/Console.hh>

class FrameBuffer : public Console
{
public:
   FrameBuffer();
   virtual ~FrameBuffer();

   virtual void putChar(char ch);

protected:
   virtual int putChar(int ch, int row, int column) = 0;
   virtual void clearScreen() = 0;
   virtual void setCursor(int row, int column) = 0;
   virtual void scrollScreen() = 0;

   virtual int getRows() = 0;
   virtual int getColumns() = 0;

private:
   FrameBuffer(const FrameBuffer&) = delete;
   FrameBuffer& operator=(const FrameBuffer&) = delete;

   void stepRow();
   void stepColumn();
   int currentRow;
   int currentColumn;
   bool isEnabledM;
};

#endif
