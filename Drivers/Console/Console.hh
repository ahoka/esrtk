#ifndef _COLOR_H_
#define _COLOR_H_

#include "ConsoleInterface.hh"

class Console {
public:
	Console(ConsoleInterface* interface);
	~Console();

	void putChar(int ch);
	void write(const char* string);
private:
	void setInterface(ConsoleInterface* interface);

	ConsoleInterface* interface;

	int currentColumn;
	int currentRow;
};

#endif
