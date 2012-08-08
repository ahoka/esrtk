#include "Console.hh"

Console::Console(ConsoleInterface* interface)
{
	setInterface(interface);
}

Console::~Console()
{
}

void
Console::write(const char* /*string*/)
{
	//
}

void
Console::putChar(int ch)
{
	interface->putChar(ch, currentColumn, currentRow);

	// incrementColumn() ->
	if (currentColumn > interface->getColumns() - 1) {
		currentColumn %= interface->getColumns();

		// incrementRow() ->
		currentRow++;

		if (currentRow > interface->getRows() - 1) {
			// generic scroll or smart scroll by driver?
			//scrollScreen();
			currentRow = 0; // just flow over to the top now
		}
	}
}

void
Console::setInterface(ConsoleInterface* interface)
{
	this->interface = interface;
}
