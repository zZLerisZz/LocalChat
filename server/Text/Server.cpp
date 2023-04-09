#include "Server.h"

std::list<Client>* Server::connectionsList = new std::list<Client>;

Client* Server::tempP = nullptr;
std::string Server::logFile = "";
bool Server::isLogWriting = true;

Server::Server() {
	DLLVersion = MAKEWORD(2, 1);
	if(WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error with library!" << std::endl;
	}
	isWorking = false;	
	isSearching = false;
	players[0] = players[1] = nullptr;
}

void Server::StartServer(std::string _ip) {
	if(inet_addr(_ip.c_str()) != INADDR_NONE) {
		isWorking = true;
		ip = _ip;
		addr.sin_addr.s_addr = inet_addr(ip.c_str());
		addr.sin_port = 0;
		addr.sin_family = AF_INET;
		listener = socket(AF_INET, SOCK_STREAM, NULL);
		bind(listener, (SOCKADDR*)&addr, sizeof(addr));
		addrSize = sizeof(addr);
		getsockname(listener, (SOCKADDR*)&addr, &addrSize);
		port = ntohs(addr.sin_port);
		listen(listener, SOMAXCONN);
		char timeString[20];
		time_t now = time(0);
		struct tm tstruct = *localtime(&now);
		strftime(timeString, sizeof(timeString), "%d-%m-%Y", &tstruct);
		logFile += "log[";
		logFile += timeString;
		logFile += "].txt";
		if(isLogWriting)
			WriteInLog("Сервер создан");
	}
}

bool Server::GetIsWorking() {
	return isWorking;
}

void Server::AddClient(SOCKET client) {
	int nick_size;
	std::string con;
	int sz_con;
	char *nick = nullptr;
	do {
		if(nick != nullptr)
			delete[] nick;
		recv(client, (char*)&nick_size, sizeof(int), NULL);
		nick = new char[nick_size + 1];
		recv(client, nick, nick_size, NULL);
		nick[nick_size] = '\0';
		if(!CheckNickname(nick)) {
			con = CommandsHandler::GetCommand(DENY);
			sz_con = con.size();
			send(client, (char*)&sz_con, sizeof(int), NULL);
			send(client, con.c_str(), sz_con, NULL);
		}
	}while(!CheckNickname(nick));
	con = CommandsHandler::GetCommand(ACCEPT);
	sz_con = con.size();
	send(client, (char*)&sz_con, sizeof(int), NULL);
	send(client, con.c_str(), sz_con, NULL);
	connectionsList->push_back(Client(client, nick, false));
	delete[] nick;
	auto it = connectionsList->end();
	it--;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&ClientHandler, (LPVOID)&(*it), NULL, NULL);
	con = "На сервер зашел " + it->GetNickname();
	if(isLogWriting)
		WriteInLog(con);
	int con_sz = con.size();
	for(auto it = connectionsList->begin(); it != connectionsList->end(); it++) {
		send(it->GetConnection(), (char*)&con_sz, sizeof(int), NULL);
		send(it->GetConnection(), con.c_str(), con_sz, NULL);
	}
}

void Server::CheckConnections() {
	isSearching = true;
	SOCKET temp = accept(listener, (SOCKADDR*)&addr, &addrSize);
	if(temp == 0) {
		std::cout << "Error with connect to server!" << std::endl;
	}
	else {
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&AddClient, (LPVOID)temp, NULL, NULL);
	}
	isSearching = false;
}

bool Server::GetSearching() {
	return isSearching;
}

void Server::Update() {
	if(tempP != nullptr) {
		MakeGame(tempP);
		tempP = nullptr;
	}
	int sz = sizeof(CommandsHandler::GetCommand(CHECK));
	for(auto it = connectionsList->begin(); it != connectionsList->end();) {
		if(send(it->GetConnection(), (char*)&sz, sizeof(int), NULL) == -1 || send(it->GetConnection(), CommandsHandler::GetCommand(CHECK), sz, NULL) == -1) {
			close(it->GetConnection());
			std::string log = it->GetNickname() + " отключился.";
			if(isLogWriting)
				WriteInLog(log);
			it = connectionsList->erase(it);
		}
		else {
			it++;
		}
	}
}

void Server::ClientHandler(Client *client) {
	char *msg;
	int sz = sizeof(CommandsHandler::GetCommand(CHECK)), msg_size;
	while(send(client->GetConnection(), (char*)&sz, sizeof(int), NULL) != -1 && send(client->GetConnection(), CommandsHandler::GetCommand(CHECK), sz, NULL) != -1) {
		recv(client->GetConnection(), (char*)&msg_size, sizeof(int), NULL);
		msg = new char[msg_size + 1];
		recv(client->GetConnection(), msg, msg_size, NULL);
		msg[msg_size] = '\0';
		if(CommandsHandler::SetCommand(msg) == NOT_COMMAND) {
			char timeString[20];
			time_t now = time(0);
			struct tm tstruct = *localtime(&now);
			strftime(timeString, sizeof(timeString), "%d.%m.%Y %X", &tstruct);
			std::string res = client->GetNickname() + "[" + timeString + "]: " + msg;
			msg_size = res.size();
			for(auto it = Server::connectionsList->begin(); it != Server::connectionsList->end(); it++) {
				if(it->GetNickname() == client->GetNickname())
					continue;
				send(it->GetConnection(), (char*)&msg_size, sizeof(int), NULL);
				send(it->GetConnection(), res.c_str(), msg_size, NULL);
			}
		}
		else if(CommandsHandler::SetCommand(msg) != CHECK && CommandsHandler::SetCommand(msg) != QUIT) {
			if(!client->GetIsInGame()) {
				client->SetTurn(CommandsHandler::SetCommand(msg));
				Server::tempP = client;
			}
		}
		else if(CommandsHandler::SetCommand(msg) == QUIT) {
			delete[] msg;
			std::string info = client->GetNickname() + " вышел из чата";
			if(isLogWriting)
				WriteInLog(info);
			int info_sz = info.size();
			for(auto it = connectionsList->begin(); it != connectionsList->end(); ) {
				if(it->GetNickname() == client->GetNickname()) {
					close(it->GetConnection());
					it = connectionsList->erase(it);
				}
				else {
					send(it->GetConnection(), (char*)&info_sz, sizeof(int), NULL);
					send(it->GetConnection(), info.c_str(), info_sz, NULL);
					it++;
				}
			}
			return;
		}
		delete[] msg;
	}
}

bool Server::CheckNickname(char *nick) {
	for(auto it = connectionsList->begin(); it != connectionsList->end(); it++)
		if(strcmp(it->GetNickname().c_str(), nick) == 0)
			return false;
	return true;
}

void Server::MakeGame(Client *player) {
	if(players[0] == nullptr) {
		std::string msg = player->GetNickname() + " запустил игру.";
		int msg_size = msg.size();
		players[0] = player;
		for(auto it = connectionsList->begin(); it != connectionsList->end(); it++) {
			send(it->GetConnection(), (char*)&msg_size, sizeof(int), NULL);
			send(it->GetConnection(), msg.c_str(), msg_size, NULL);
		}
		if(isLogWriting)
			WriteInLog(msg);
	}
	else {
		players[1] = player;
		GiveResult();
	}
}

void Server::GiveResult() {
	std::string msg = "Два игрока сошлись в нереальной битве: " + players[0]->GetNickname() + " и " + players[1]->GetNickname() + ". Победитель - ";
	int msg_size;
	if(players[0]->GetTurn() == players[1]->GetTurn())
		msg += "дружба!";
	else 
		switch(players[0]->GetTurn()) {
		case ROCK:
			switch(players[1]->GetTurn()) {
			case SCISSORS:
				msg += players[0]->GetNickname() + "!";
				break;
			case PAPER:
				msg += players[1]->GetNickname() + "!";
				break;
			}
			break;
		case PAPER:
			switch(players[1]->GetTurn()) {
			case SCISSORS:
				msg += players[1]->GetNickname() + "!";
				break;
			case ROCK:
				msg += players[0]->GetNickname() + "!";
				break;
			}
			break;
		case SCISSORS:
			switch(players[1]->GetTurn()) {
			case PAPER:
				msg += players[0]->GetNickname() + "!";
				break;
			case ROCK:
				msg += players[1]->GetNickname() + "!";
				break;
			}
			break;
		}
	players[0] = players[1] = nullptr;
	msg_size = msg.size();
	for(auto it = connectionsList->begin(); it != connectionsList->end(); it++) {
		send(it->GetConnection(), (char*)&msg_size, sizeof(int), NULL);
		send(it->GetConnection(), msg.c_str(), msg_size, NULL);
	}
	if(isLogWriting)
		WriteInLog(msg);
}

Server::~Server() {
	close(listener);
}

void Server::SendQuit() {
	std::string quit = "/quit";
	int quit_sz = quit.size();
	for(auto it = connectionsList->begin(); it != connectionsList->end(); it++) {
		send(it->GetConnection(), (char*)&quit_sz, sizeof(int), NULL);
		send(it->GetConnection(), quit.c_str(), quit_sz, NULL);
	}
}

void Server::WriteInLog(const char* msg) {
	char timeString[20];
	time_t now = time(0);
	struct tm tstruct = *localtime(&now);
	strftime(timeString, sizeof(timeString), "%d.%m.%Y %X", &tstruct);
	std::string res = "[";
	res += timeString;
	res += "] ";
	res += msg;
	std::ofstream f(logFile, std::ios::app);
	f << res << std::endl;
	f.close();
}

void Server::WriteInLog(char* msg) {
	char timeString[20];
	time_t now = time(0);
	struct tm tstruct = *localtime(&now);
	strftime(timeString, sizeof(timeString), "%d.%m.%Y %X", &tstruct);
	std::string res = "[";
	res += timeString;
	res += "] ";
	res += msg;
	std::ofstream f(logFile, std::ios::app);
	f << res << std::endl;
	f.close();
}

void Server::WriteInLog(std::string msg) {
	char timeString[20];
	time_t now = time(0);
	struct tm tstruct = *localtime(&now);
	strftime(timeString, sizeof(timeString), "%d.%m.%Y %X", &tstruct);
	std::string res = "[";
	res += timeString;
	res += "] ";
	res += msg;
	std::ofstream f(logFile, std::ios::app);
	f << res << std::endl;
	f.close();
}

unsigned short Server::GetPort() {
	return port;
}

unsigned int Server::GetUserCount() {
	return (unsigned int)connectionsList->size();
}

std::string Server::GetIP() {
	return ip;
}

std::string Server::GetLogFile() {
	return logFile;
}

const char* Server::GetIsWriting() {
	if(isLogWriting)
		return "логи записываются";
	return "логи не записываются";
}

void Server::SwitchLog() {
	if(isLogWriting)
		WriteInLog("Запись логов была отключена");
	else
		WriteInLog("Запись логов была включена");
	isLogWriting = !isLogWriting;
}

bool Server::GetIsLogWriting() {
	return isLogWriting;
}