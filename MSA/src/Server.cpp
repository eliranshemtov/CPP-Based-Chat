
#include "Server.h"
#include "MessengerProtocol.h"

void print_instructions();

int main() {

	Server server;
	string command;

	cout << "Messenger App - Server Side" << endl;

	print_instructions();

	do
	{
		cin >> command;

		if (command == "lcu") //list all connected users
		{
		server.printUsers();
		}
		else if (command == "lu") // list all users
		{
			server.printAllUsers();
		}
		else if (command == "ls") //list all sessions
		{
			server.ptintSessions();
		}
		else if (command == "p")	// print the instructions again
		{
			print_instructions();
		}
		else if (command == "x") //close the app
		{
			server.close();
		}
		else
		{
			cout << "** BAD COMMAND **" << endl;
		}
	}
	while (command != "x");


	return 0;
}

void print_instructions()
{

	cout << endl;
	cout << "Supported Actions:" << endl;
	cout << "lu - list all users" << endl;
	cout << "lcu - list all connected users" <<  endl;
	cout << "ls - list all sessions" << endl;
	cout << "p - print commands" << endl;
	cout << "x - close the app" << endl;
	cout << endl;
}

Server::Server() :loginAndSign(new LoginAndSignUp){
	this->sock= NULL;
	this->flag = false;
	this->start();
}

void Server::printAllUsers()
{
	vector<string> names_vector = loginAndSign->getAllUsers();

	cout << "=====Users List:=====" << endl;
	for (unsigned int i=0 ; i< names_vector .size() ; i++)
	{
		cout << "user #" << i+1 << ": " <<  names_vector [i] << endl;
	}
}

void Server::printUsers()
{
	loginAndSign->printUsers();
}
void Server::ptintSessions()
{
	loginAndSign->ptintSessions();
}



void Server::run()
{

	this->flag = true;
	this->sock= new TCPSocket(SERVER_PORT);
	sleep(2);
	cout << "listening on port " << SERVER_PORT << endl;

	while (this->flag)
	{

		TCPSocket* sockTCP = this->sock->listenAndAccept();

		if (!sockTCP)
			continue;
		else
			loginAndSign->addPeer(sockTCP);
	}
	cout << "Server is done..." << endl;
}

void Server::close(){
	cout << "Shutting down the server..." << endl;
	this->flag = false;
	this->loginAndSign->close();
	this->loginAndSign->waitForThread();
	this->sock->cclose();

	delete(loginAndSign);
	this->flag = false;
}

Server::~Server() {
}
