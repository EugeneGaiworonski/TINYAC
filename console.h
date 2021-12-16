#ifndef CONSOLEH
#define CONSOLEH
#include "processor.h"
class Console 
{
	public:
	bool Hmode;
	Console();
	void doConsole(Processor);
	void setConsoleTitle(int8_t);
};

#endif