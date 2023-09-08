#ifndef SERVER_HPP
#define SERVER_HPP

// Linux
#include <cstdlib>
#include <cerrno>
#include <cstring>


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <exception>
#include <fcntl.h>
#include <poll.h>
#include <sys/select.h>
#include <algorithm>
#include "user.hpp"
#include "client.hpp"

using namespace std;

//#define IP_SERV "127.0.0.1"


//#define IP_SERV "10.13.3.5"

#define IP_SERV "127.0.0.1"


//#define IP_SERV "10.11.12.1" // IP 42 Alex
//#define IP_SERV "10.11.12.3" // IP 42 Quent

class User;
class Client;

class Server {

private:
	int _socketServer;
	int _port;
	string _password;
	bool _validPassword;
	vector<int> _userSocket;
	struct sockaddr_in _serverAddress;
	Client				*_client;


public:

	Server();
	Server(Server const &src);
	Server(int port, string password);
	Server &operator=(Server const &rhs);
	~Server();
//SET
	void setPassword(std::string password);

// GET
	int getPort() const;
	int getSocketServer() const;
	string getPassword() const;
	bool getValidPassword() const;
	const vector<int>& getUserSockets() const;

//SERV
	void createSocketServer();
	void waitToNewConnection();

//PRINT OUTPUT
	void serverPrintOutput(int port, int socketserver);


	class BadArgument : public exception {
	public:
		virtual const char *what() const throw();
	};

	class FailOpeningSocket : public exception {
	public:
		virtual const char *what() const throw();
	};

};

std::ostream &operator<<(std::ostream &o, Server const &i);

#endif