#include "CommandsHandler.h"

const char* CommandsHandler::GetCommand(Commands com) {
	switch(com) {
	case CHECK:
		return "/check";
	case ACCEPT:
		return "/accept";
	case DENY:
		return "/deny";
	case ROCK:
		return "/rock";
	case SCISSORS:
		return "/scissors";
	case PAPER:
		return "/paper";
	case QUIT:
		return "/quit";
	}
}

Commands CommandsHandler::SetCommand(char *msg) {
	if(strcmp(msg, "/check") == 0)
		return CHECK;
	if(strcmp(msg, "/accept") == 0)
		return ACCEPT;
	if(strcmp(msg, "/deny") == 0)
		return DENY;
	if(strcmp(msg, "/rock") == 0)
		return ROCK;
	if(strcmp(msg, "/scissors") == 0)
		return SCISSORS;
	if(strcmp(msg, "/paper") == 0)
		return PAPER;
	if(strcmp(msg, "/quit") == 0)
		return QUIT;
	return NOT_COMMAND;
}

Commands CommandsHandler::SetCommand(const char *msg) {
	if(strcmp(msg, "/check") == 0)
		return CHECK;
	if(strcmp(msg, "/accept") == 0)
		return ACCEPT;
	if(strcmp(msg, "/deny") == 0)
		return DENY;
	if(strcmp(msg, "/rock") == 0)
		return ROCK;
	if(strcmp(msg, "/scissors") == 0)
		return SCISSORS;
	if(strcmp(msg, "/paper") == 0)
		return PAPER;
	if(strcmp(msg, "/quit") == 0)
		return QUIT;
	return NOT_COMMAND;
}