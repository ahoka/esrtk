#ifndef _CONSOLEINTERFACE_H_
#define _CONSOLEINTERFACE_H_

// dumb console interface

class ConsoleInterface {
public:
	ConsoleInterface() {};
	virtual ~ConsoleInterface() {};

	virtual void putChar(int ch) = 0;
	virtual int getColumns() = 0;
	virtual int getRows() = 0;
	virtual void scroll(int lines) = 0;
};

#endif
