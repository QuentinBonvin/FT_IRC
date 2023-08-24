#include "server.hpp"

Server::Server() {};

Server::Server( int port, string password ) : _port(port), _password(password) {};

Server::Server( const Server &src ) {
	*this = src;
}

Server &Server::operator=( const Server &rhs ) {
	if ( this != &rhs )
	{
		this->_port = rhs._port;
		this->_idUser = rhs._idUser;
		this->_password = rhs._password;
	}
	return ( *this );
}

Server::~Server() {};


int Server::getPort() {
	return( this->_port );
}

int Server::getSocketServer() {
	return ( this->_socketServer );
}

void Server::setAndAssignSocketToClient(){
	while (true) {
		fd_set readfds = checkClientConnection();
		collectDataFromClient(readfds);
    }
	close(_socketServer);
}

void 	Server::collectDataFromClient( fd_set readfds ) {
	for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); it++) {
		Client &client = *it;
		if (FD_ISSET(client.getSocket(), &readfds)) {
			char buffer[1024];
			ssize_t bytesRead = recv(client.getSocket(), buffer, sizeof(buffer), 0);
			if (bytesRead < 0) {
				if (errno != EAGAIN && errno != EWOULDBLOCK) {
					std::cerr << "Error receiving data: " << std::endl;
				}
			}
			else if (bytesRead == 0) {
				std::cout << "Client: " << client.getSocket() <<" | disconnected." << std::endl;
				it = _clients.erase(it);
				--it;
				close(client.getSocket());
			}
			else {
				buffer[bytesRead] = '\0';
				std::cout << "Client " << client.getSocket() << " | Received: " << buffer << std::endl;
				// Zone de test
				// Pour se renvoyer une demamnde au client:  :NICK!~USER@leServeur ARGUMENTS ex: JOIN :#test
        
				if (client.getNickUserInit() == false){
							client.parseNick_User(buffer);
				  for (std::vector<Client>::iterator it = _clients.begin(); it != _clients.end(); ++it) {
					if (it->getSocket() == client.getSocket()) {
					  *it = client;
					  break;
					}
				  }
				}
        // Fin zone de test

			}
		}
	}
}

fd_set Server::checkClientConnection() {
	int max_fd = _socketServer;
	fd_set readfds;
	FD_ZERO(&readfds);
	FD_SET(_socketServer, &readfds);
	for (std::vector<int>::iterator it = _clientSockets.begin(); it != _clientSockets.end(); it++) {
		int clientSocket = *it;
		FD_SET(clientSocket, &readfds);
		if (clientSocket > max_fd) {
			max_fd = clientSocket;
		}
	}
	sleep(1);
	int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
	if (activity < 0) {
		cout << "Error" << endl;
//		std::cerr << "Error with select: "<< activity << std::endl;
//		continue;
	}

	if (FD_ISSET(_socketServer, &readfds)) {
		int clientSocket = accept(_socketServer, NULL, NULL);
		if (clientSocket < 0) {
			std::cerr << "Error accepting client connection: " << std::endl;
		} else {
			std::cout << "New connection accepted. Client socket: " << clientSocket << std::endl;
			_clientSockets.push_back(clientSocket);

			int flags = fcntl(clientSocket, F_GETFL, 0);
			if (flags == -1) {
				std::cerr << "Error getting socket flags: " << std::endl;
				close(clientSocket);
//				continue;
			}
			if (fcntl(clientSocket, F_SETFL, flags | O_NONBLOCK) == -1) {
				std::cerr << "Error setting socket to non-blocking: " << std::endl;
				close(clientSocket);
//				continue;
			}
			Client newClient(clientSocket);
			_clients.push_back(newClient);
		}
	}
	return (readfds);
}


void Server::setSocketServer() {
	_socketServer = socket( AF_INET, SOCK_STREAM, 0 );
	if ( _socketServer < 0 )
		throw Server::FailOpeningSocket();
}

void Server::setAddressServer() {
	_addressServer.sin_addr.s_addr = inet_addr( IP_SERV );
	_addressServer.sin_family = AF_INET;
	_addressServer.sin_port = htons(_port);
}

void Server::connectServer() {
	bind( _socketServer, (const struct sockaddr *)&_addressServer, sizeof( _addressServer) );
	cout << "bind :" << _socketServer << endl;
}

const char * Server::BadArgument::what() const throw() {
	return ("Error: need to have 3 arguments: ./ircserv <port> <password>");
}

const char * Server::FailOpeningSocket::what() const throw() {
	return ("Error: impossible to opening socket");
}


