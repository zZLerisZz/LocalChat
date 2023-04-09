#include <winsock2.h>
#include <cstring>
#include <unistd.h>
#include <list>
#include "Commands.h"
#include "CommandsHandler.h";
#include "ChatLine.h"

#ifndef CLIENT_H
#define CLIENT_H

class Client {
private:
	WSAData wsaData;
	WORD DLLVersion;
	SOCKADDR_IN addr;
	unsigned short port;
	std::string ip;
	static SOCKET connection;
	std::string nickname;
	static bool isConnected;
	static std::list<std::pair<std::string, int>> *messagesToCut;
	static bool *quit;
	static void ServerHandler();
	bool CheckConnection();
public:
	Client();
	void SetQuit(bool *_quit);
	~Client();
	bool GetIsConnected();
	void SendMessage(std::string msg);
	std::string GetIP();
	std::string GetNickname();
	unsigned short GetPort();
	void StartClient(std::list<std::pair<std::string, int>> *_messagesToCut);
	void QuitServer();
	int TryToConnect(std::string _ip, unsigned short _port, std::string _nickname);
};

#endif