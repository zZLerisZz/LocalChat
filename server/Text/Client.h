#include <winsock2.h>
#include <cstring>
#include <iostream>
#include "Commands.h"

#ifndef CLIENT_H
#define CLIENT_H

class Client {
private:
	SOCKET connection;
	std::string nickname;
	bool isInGame;
	Commands turn;
public:
	Client(SOCKET _connection, char* _nickname, bool _isInGame);
	SOCKET GetConnection();
	std::string GetNickname();
	bool GetIsInGame();
	void SetTurn(Commands _turn);
	Commands GetTurn();
};

#endif