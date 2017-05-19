
#include "ClientMessenger.h"
#include "MessengerProtocol.h"

const string MSG_FROM_SERVER =  "Message from Server: ";

void print_instructions();

int main() {

	string command,user,pass;
	string ip;
	Client client;

	cout << "Welcome To the messenger App!" << endl;
	
	print_instructions();

	do
	{
		command.clear();
		cin >> command;

		if (command == "c") //connect to server
		{
			cout << "Enter the server's ip" << endl;
			cin >> ip;
			if (!client.connectToServer(ip,SERVER_PORT))
				cout << "Connection error with the server..." <<  endl;
			else
				client.isOnline = true;
			continue;
		}
		else if (command == "login") //login to server
		{
			if(client.isLoggedIn){
				cout<<"You are already logged in, perform another action..."<<endl;
				continue;
			}
			if (!client.isOnline)
			{
				cout << "ERROR!!! Can't do that before connecting..." <<  endl;
				continue;
			}
			cout << "Enter username:" <<  endl;
			cin >> user;
			cout << "Enter password:" << endl;
			cin >> pass;

			client.login(user,pass);
			sleep(5);
		}
		else if (command == "register") //signup to server
		{
			if(client.isLoggedIn){
				cout<<"You are already logged in, perform another action..."<<endl;
				continue;
			}
			if (!client.isOnline)
			{
				cout << "ERROR!!! Can't do that before connecting..." <<  endl;
				continue;
			}
			cout << "Enter username:" <<  endl;
			cin >> user;
			cout << "Enter password:" << endl;
			cin >> pass;
			client.signup(user,pass);
		}
		else if (command == "lu") //list all users from server
		{
			if (!client.isOnline )
			{
				cout << "ERROR!!! Can't do that before connecting..." <<  endl;
				continue;
			}
			client.getUsers();
		}
		else if (command == "lcu") //list all connected users
		{
			if (!client.isOnline )
			{
				cout << "ERROR!!! Can't do that before connecting..." <<  endl;
				continue;
			}
			client.listAllUsers();
		}
		else if (command == "o") //open a session with a user
		{
			if (!client.isOnline )
			{
				cout << "ERROR!!! Can't do that before connecting..." <<  endl;
				continue;
			}
			client.openSession();
		}
		else if (command == "cs") //close the current session
		{
			if (!client.isOnline )
			{
				cout << "ERROR!!! Can't do that before connecting..." <<  endl;
				continue;
			}
			client.closeSession();
		}
		else if (command == "s") //send a msg to user in session
		{
			if (!client.isOnline )
			{
				cout << "ERROR!!! Can't do that before connecting..." <<  endl;
				continue;
			}
			client.sendMsgToSession();
		}
		else if (command == "r") //reply to session
		{
			if (!client.isOnline )
			{
				cout << "ERROR!!! Can't do that before connecting..." <<  endl;
				continue;
			}
			client.replyToSession();
		}
		else if (command == "l") //prints connection status
		{
			if (!client.isOnline )
			{
				cout << "ERROR!!! Can't do that before connecting..." <<  endl;
				continue;
			}
			if (client.ConnectionStatus())
			{
				if (client.sessionStatus())
					cout << "You are connected to a session" <<  endl;
				else
					cout << "Your connection status is on but your'e not connected to anything!" <<  endl;
			}
			else
				cout << "You are not connected" <<  endl;
		}
		else if (command == "d") //disconnect from server
		{
			if (!client.isOnline)
			{
				cout << "ERROR!!! Can't do that before connecting..."<<  endl;
				continue;
			}
			client.disconnectFromServer();
		}
		else if (command == "x"){//close application
			writeCommand(client.server_sock,EXIT);
			client.close();

		}

		else if (command == "p")
			print_instructions();
		else
			cout << "No such Command exists" << endl;
	}
	while(command != "x");

	return 0;
}

void print_instructions()
{
	cout << endl;
	cout << "c - connect to server" << endl;
	cout << "login - Login to server" << endl;
	cout << "register - Signup to server(and login)" <<  endl;
	cout << "lu - print all users from server" << endl;
	cout << "lcu - print all connected users from server" << endl;
	cout << "o- open a session with a user" << endl;//
	cout << "cs - close the current session" << endl;
	cout << "s - send a message to session with user" << endl;
	cout << "r - reply to session with user" << endl;
	cout << "l - print connection status" << endl;
	cout << "d - disconnect from server" << endl;
	cout << "p - print instructions" << endl;
	cout << "x - close application" << endl;
	cout << endl;
}

//C'Tor
Client::Client(){
	this->server_sock = NULL;
	this->udp_sock = NULL;
	this->connectionStatus = false;
	this->isLoggedIn = false;
	this->isInSession = false;
	this->isOnline = false;
}

// Prints the server's registered users
void Client::getUsers()
{
	writeCommand(server_sock,GET_ALL_USERS);
}

// Replies a msg to an active session peer
void Client::replyToSession()
{
	string msg;

	cout << "Enter msg: ";
	getline(cin,msg);
	getline(cin,msg);

	udp_sock->reply("[" + client_name + "]" + msg);
}

// Connects (TCP) the client to a given server ip (the port is Prefixed by the protocol)
bool Client::connectToServer(string ip,int port)
{
	server_sock = new TCPSocket(ip,port);
	if (server_sock->getSocketFd() < 0)
	{
		return false;
	}
	else
	{
		connectionStatus = true;
		this->start();
	}
	return true;
}

// Login to server (by user's credentials)
void Client::login(string name,string pass)
{
	writeCommand(server_sock,LOGIN);
	writeMsg(server_sock,name);
	writeMsg(server_sock,pass);
}

// Signup as new user on the server
void Client::signup(string user,string pass)
{
	writeCommand(server_sock,SIGNUP);
	writeMsg(server_sock,user);
	writeMsg(server_sock,pass);
}

// Send a msg to the server (Only when connected)
void Client::sendMsgToServer(string msg)
{
	writeMsg(server_sock,msg);
}

// Clean disconnection from Server
void Client::disconnectFromServer()
{
	writeCommand(server_sock,EXIT);
	this->connectionStatus = false;
	this->isInSession = false;
	this->isOnline = false;
}

// Prints all online users (not just logged in)
void Client::listAllUsers()
{
	writeCommand(server_sock,GET_ALL_CONNECTED_USERS);
}


// Opens UDP session between 2 clients
void Client::openSession()
{
	string name;
	cout << "What's your friend's Username?" << endl;
	cin >> name;

	writeCommand(server_sock,OPEN_SESSION_WITH_USER);
	writeMsg(server_sock,name);
}

// Close all sessions
void Client::closeSession()
{
	writeCommand(server_sock,CLOSE_SESSION_WITH_USER);
	session_peer.Clean();
	this->isInSession = false;
}


/*******************handler methods*******************/

// send msg to the other client on the session
void Client::sendMsgToSession()
{
	string msg;

	if (isInSession == true)
	{
		cout << "Enter msg to send: ";

		getline(cin,msg);
		getline(cin,msg);
		udp_sock->sendTo("[" + this->client_name + "]" +  msg,this->session_peer.ip,session_peer.port);
	}
	else
	{
		cout << "You don't have an open session" <<  endl;
	}
}

// get the ip and port of a remote peer
void Client::gotIpAndPort()
{
	string msg;
	string port;
	char delim = ':';
	bool foundDelim = false;

	msg = readMsg(server_sock); //msg is ip & port
	for (unsigned int i=0 ; i < msg.length() ; i++)
	{
		if (msg[i] != delim)
		{
			if (foundDelim == false)
				session_peer.ip += msg[i];
			else
				port += msg[i];
		}
		else
		{
			foundDelim = true;
		}
	}
	session_peer.port = atoi(port.c_str());
	cout << "You are now in session " << "(" +  msg + ")" << endl;
	isInSession = true;

}

// Create new session
void Client::loggedIn()
{
	string ipPort;
	string ip,port;
	char delim = ':';
	bool foundDelim = false;
	int int_port;

	this->client_name = readMsg(this->server_sock);
	ipPort = readMsg(this->server_sock);

	this->isLoggedIn = true;

	for (unsigned int i=0 ; i < ipPort.length() ; i++)
	{
		if (ipPort[i] != delim)
		{
			if (foundDelim == false)
				ip += ipPort[i];
			else
				port += ipPort[i];
		}
		else
		{
			foundDelim = true;
		}
	}
	int_port = atoi(port.c_str());
	this->udp_sock = new Session(int_port);
}

// bool - is the client connected?
bool Client::ConnectionStatus()
{
	return connectionStatus;
}

// bool - is the client in session?
bool Client::sessionStatus()
{
	return isInSession;
}


// accept commands from server in loop
void Client::run()
{
	int command;
	string username;
	string pass;
	string ipPort;
	string msg;

	//A loop which listens for TCP command & data that is sent from the server
	while (connectionStatus)
	{
		//Receiving commands from the tcp socket (connected to the server)
		command = readCommand(server_sock);

		if (!command)
			continue;

		switch(command)
		{
		case LOGIN_OR_SIGNUP:
			cout << "Connection established..."<<endl<<"Login or Register?" <<  endl;
			break;
		case LOGIN_DENIED:
			cout << MSG_FROM_SERVER + "Your password or username may be incorrect, Have you signed up already?" << endl;
			break;
		case USERNAME_TAKEN:
			cout <<  MSG_FROM_SERVER + "This Username already exists" << endl;
			break;
		case ALL_USERS_LIST:	// list all users
			gotAllUsers();
			break;
		case CONFIRM_USER:
			cout << MSG_FROM_SERVER + "Welcome! you are logged in..." << endl;
			loggedIn();
			break;
		case BAD_COMMAND:
			cout << MSG_FROM_SERVER +  "Must login/register first..." <<  endl;
			break;
		case INCOMIMG_SESSION:
			gotIncomingSession();
			break;
		case OPEN_SESSION_ERROR:
			cout << MSG_FROM_SERVER + "friend`s name isn't there" << endl;
			break;
		case CLOSE_SESSION_ERROR:
			cout << MSG_FROM_SERVER + "You are not in session" << endl;
			break;
		case OPEN_CONNECTION_ERROR:
			cout << MSG_FROM_SERVER + "You have an open connection" << endl;
			break;
		case SESSION_ENDED:
			cout << MSG_FROM_SERVER + "Session is done!" << endl;
			isInSession = false;
			session_peer.Clean();
			udp_sock->running=false;
			udp_sock->sock->cclose();
			break;
		case OPEN_SESSION_ERROR2:
			cout << MSG_FROM_SERVER + "You have an open session" << endl;
			break;
		case CONNECTION_ERROR:
			cout << MSG_FROM_SERVER + "You don't have any open connections" << endl;
			break;
		case USERS_LIST:
			gotUsersList();
			break;
		case TARGET_IP_AND_PORT:
			gotIpAndPort();
			break;
		case END_MSG:
			cout << MSG_FROM_SERVER + "See you soon! ;)" << endl;
			exit(1);
			break;
		default:
			cout << "Bad command from server" << endl;
		}
	}
}

// setting up the incoming session data
void Client::gotIncomingSession()
{
	string ipPort;
	string ip,port,name;
	int int_port;
	char delim = ':';
	bool found = false;

	name = readMsg(server_sock);
	ipPort = readMsg(server_sock);
	cout << MSG_FROM_SERVER + "You have an incoming session from: " + name + "(" + ipPort + "),"<<endl;
	cout<<"use 'S' to send a msg or 'r' to reply" <<endl;
	isInSession = true;

	for (unsigned int i = 0 ; i < ipPort.length() ; i++)
	{
		if (ipPort[i] != delim)
		{
			if (found == false)
				ip += ipPort[i];
			else
				port += ipPort[i];
		}
		else
		{
			found = true;
		}
	}
	int_port = atoi(port.c_str());
	this->session_peer.name = name;
	this->session_peer.ip = ip;
	this->session_peer.port = int_port;
}

// print user list
void Client::gotUsersList()
{
	int usersCount;
	string userName;

	usersCount = readCommand(server_sock); //size of list
	for (int i=0 ; i<usersCount ; i++)
	{
		userName = readMsg(server_sock);
		cout << "User #" << i+1 << ": " << userName << endl;
	}
}

// print all ther users list
void Client::gotAllUsers()
{
	int size;
	string name;
	size = readCommand(server_sock);

	cout << "*** ALL USERS ***" << endl;

	for (int i=0 ; i < size ; i++)
	{
		name = readMsg(server_sock);
		cout << "User #" << i+1 << ": " << name << endl;
	}
}

// Close connection
void Client::close()
{
	connectionStatus = false;
	server_sock->cclose();
}


// D'tor
Client::~Client()
{
	this->close();
}
