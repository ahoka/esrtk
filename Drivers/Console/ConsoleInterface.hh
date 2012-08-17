#ifndef _CONSOLEINTERFACE_H_
#define _CONSOLEINTERFACE_H_

class ConsoleInterface {
public:
	ConsoleInterface();
	virtual ~ConsoleInterface();

	virtual void putChar(int ch, int x, int y/*, Color fg, Color bg*/) = 0;
	virtual int getChar(int x, int y/*, Color* fg, Color* bg*/) = 0;
	virtual int getColumns() = 0;
	virtual int getRows() = 0;
	virtual void scroll(int lines) = 0;
//	virtual void setPosition(int x, int y) = 0;
//	virtual void getPosition(int* x, int* y) = 0;
};

#endif
