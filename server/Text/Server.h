#include <winsock2.h>
#include <iostream>
#include <list>
#include <iterator>
#include <functional>
#include <utility>
#include <cstring>
#include <ctime>
#include "Client.h"
#include "Commands.h"
#include "CommandsHandler.h"
#include <unistd.h>
#include <fstream>

#ifndef SERVER_H
#define SERVER_H

class Server {
private:
	WSAData wsaData;
	WORD DLLVersion;
	SOCKADDR_IN addr;
	SOCKET listener;
	bool isSearching;
	int addrSize;
	Client *players[2];
	static std::string logFile;
	static bool isLogWriting;
	unsigned short port;
	std::string ip;
	bool isWorking;
	static bool CheckNickname(char *name);
	static Client *tempP;
	static std::list<Client> *connectionsList;
	void MakeGame(Client *player);
	static void ClientHandler(Client *client);
	static void AddClient(SOCKET client);
	static void WriteInLog(const char* msg);
	static void WriteInLog(char* msg);
	static void WriteInLog(std::string msg);
public:
	Server();
	void StartServer(std::string _ip);
	void CheckConnections();
	bool GetIsWorking();
	void Update();
	bool GetSearching();
	void GiveResult();
	unsigned short GetPort();
	unsigned int GetUserCount();
	std::string GetIP();
	std::string GetLogFile();
	const char* GetIsWriting();
	void SwitchLog();
	bool GetIsLogWriting();
	void SendQuit();
	~Server();
};

#endif