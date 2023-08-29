#include "client.hpp"
#include "server.hpp"

Client::Client() {};

Client::Client( const Client &src ) {
	*this = src;
}

Client &Client::operator=( const Client &rhs ) {
	if ( this != &rhs )
	{
		this->_command = rhs._command;
		this->_options = rhs._options;
		this->_server = rhs._server;
		this->_user = rhs._user;
	}
	return ( *this );
}

Client::~Client() {};

vector<User>    Client::getUserVector() const{
    return (this->_user);
}

string	Client::getCommand() const {
	return (this->_command);
}

string	Client::getOptions() const {
	return (this->_options);
}

void    Client::addUser(string buffer, int socketUser){
    User *newUser = new User;
    newUser->initUserAndNick(buffer);
    newUser->setSocketUser(socketUser);
    this->_user.push_back(*newUser);

    std::cout << "addUser: \n" << "Nick = " << _user.back().getNickName() << "\nUser = " << _user.back().getUserName() << "\nSocket = " << _user.back().getSocketUser() << std::endl;
}
