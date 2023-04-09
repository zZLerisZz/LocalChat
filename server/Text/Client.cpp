#include "Client.h"

Client::Client(SOCKET _connection, char* _nickname, bool _isInGame) {
	connection = _connection;
	nickname = _nickname;
	isInGame = _isInGame;
}

SOCKET Client::GetConnection() {
	return connection;
}

std::string Client::GetNickname() {
	return nickname;
}

bool Client::GetIsInGame() {
	return isInGame;
}

void Client::SetTurn(Commands _turn) {
	isInGame = true;
	turn = _turn;
}

Commands Client::GetTurn() {
	isInGame = false;
	return turn;
}