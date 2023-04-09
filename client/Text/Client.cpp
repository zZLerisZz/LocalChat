#include "Client.h"

bool Client::isConnected = false;
SOCKET Client::connection;
std::list<std::pair<std::string, int>>* Client::messagesToCut = nullptr;
bool* Client::quit = nullptr;

Client::Client() {
	DLLVersion = MAKEWORD(2, 1);
	if(WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error with library!" << std::endl;
	}
	addr.sin_family = AF_INET;
}

Client::~Client() {
	close(connection);
}

int Client::TryToConnect(std::string _ip, unsigned short _port, std::string _name) {
	addr.sin_addr.s_addr = inet_addr(_ip.c_str());
	addr.sin_port = htons(_port);
	connection = socket(AF_INET, SOCK_STREAM, NULL);
	if(connect(connection, (SOCKADDR*)&addr, sizeof(addr)) == 0) {
		ip = _ip;
		port = _port;
		int nick_size = _name.size();
		send(connection, (char*)&nick_size, sizeof(int), NULL);
		send(connection, _name.c_str(), nick_size, NULL);
		char *ans;
		recv(connection, (char*)&nick_size, sizeof(int), NULL);
		ans = new char[nick_size + 1];
		recv(connection, ans, nick_size, NULL);
		ans[nick_size] = '\0';
		if(CommandsHandler::SetCommand(ans) == DENY) {
			delete[] ans;
			return 2;
		}
		isConnected = true;
		nickname = _name;
		delete[] ans;
		return 0;
	}
	else {
		close(connection);
	}
	return 1;
}

void Client::StartClient(std::list<std::pair<std::string, int>> *_messagesToCut) {
	messagesToCut = _messagesToCut;
	CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)&ServerHandler, NULL, NULL, NULL);
}

void Client::ServerHandler() {
	char *msg;
	int msg_size;
	int ch_size = sizeof(CommandsHandler::GetCommand(CHECK));
	while(isConnected){
		if(recv(connection, (char*)&msg_size, sizeof(int), NULL) <= 0) {
			isConnected = false;
			break;
		}
		msg = new char[msg_size + 1];
		if(recv(connection, msg, msg_size, NULL) <= 0) {
			isConnected = false;
			delete[] msg;
			break;
		}
		msg[msg_size] = '\0';

		if(CommandsHandler::SetCommand(msg) != CHECK && CommandsHandler::SetCommand(msg) != QUIT) {
			std::string res = msg;
			std::size_t found = res.find('[');
			if(found != std::string::npos)
				messagesToCut->push_back(std::make_pair(res, 1));
			else
				messagesToCut->push_back(std::make_pair(res, 2));
		}
		else if(CommandsHandler::SetCommand(msg) == QUIT){
			delete[] msg;
			*quit = true;
			return;
		}
		delete[] msg;
	}
}

void Client::SendMessage(std::string msg) {
	int msg_size = msg.size();
	send(connection, (char*)&msg_size, sizeof(int), NULL);
	send(connection, msg.c_str(), msg_size, NULL);
}

bool Client::CheckConnection() {
	int msg_size = sizeof(CommandsHandler::GetCommand(CHECK));
	if(send(connection, (char*)&msg_size, sizeof(int), NULL) != -1 && send(connection, CommandsHandler::GetCommand(CHECK), msg_size, NULL) != -1)
		return true;
	isConnected = false;
	return false;
}

bool Client::GetIsConnected() {
	return isConnected;
}

std::string Client::GetIP() {
	return ip;
}

std::string Client::GetNickname() {
	return nickname;
}

unsigned short Client::GetPort() {
	return port;
}

void Client::QuitServer() {
	std::string quit = "/quit";
	int quit_sz = quit.size();
	send(connection, (char*)&quit_sz, sizeof(int), NULL);
	send(connection, quit.c_str(), quit_sz, NULL);
	close(connection);
	isConnected = false;
}

void Client::SetQuit(bool *_quit) {
	quit = _quit;
}