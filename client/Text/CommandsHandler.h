#include "Commands.h"

#ifndef COMMANDSHANDLER_H
#define COMMANDSHANDLER_H

class CommandsHandler {
public:
	static const char* GetCommand(Commands com);
	static Commands SetCommand(char *msg);
	static Commands SetCommand(const char *msg);
};

#endif