
#include "LoginAndSignUp.h"
#include "MessengerProtocol.h"
#include <fstream>

LoginAndSignUp::LoginAndSignUp()
{
	dispatcher = new Dispatcher(getAllUsers());
	this->socketsListener = NULL;
	status = false;
}

LoginAndSignUp::~LoginAndSignUp()
{}
void LoginAndSignUp::close()
{
	dispatcher->close();
	dispatcher->waitForThread();
	status = false;
	delete(dispatcher);
}

void LoginAndSignUp::run()
{
	int command;
	string username,password;

	status = true;

	while (status == true)
	{
		//mutex lock
		TCPSocket* ready_peer;
		this->socketsListener = new MultipleTCPSocketsListener();
		this->socketsListener->addSockets(peers);

		ready_peer = this->socketsListener->listenToSocket(2);

		delete(this->socketsListener);

		if (ready_peer != NULL)
		{
			command = readCommand(ready_peer);
		}
		else
			continue;

		if (command == 0)
		{
			continue;
		}
		else if (command == LOGIN)
		{
			username = readMsg(ready_peer);
			password = readMsg(ready_peer);
			if  (login(ready_peer,username,password) == true)
			{
				writeCommand(ready_peer,CONFIRM_USER);
				writeMsg(ready_peer,username);
				writeMsg(ready_peer,ready_peer->destIpAndPort());
				dispatcher->addNewUser(ready_peer,username);
				//peers.clear();
				iter = find(peers.begin(), peers.end(), ready_peer);
				if (iter != peers.end()) //peer was found
				{
					peers.erase(iter);
				}

			}
			else
			{
				writeCommand(ready_peer,LOGIN_DENIED);
			}
		}
		else if (command == SIGNUP)
		{
			username = readMsg(ready_peer);
			password = readMsg(ready_peer);
			if (signup(ready_peer,username,password) == true)
			{
				writeCommand(ready_peer,CONFIRM_USER);
				writeMsg(ready_peer,username);
				writeMsg(ready_peer,ready_peer->destIpAndPort());

				dispatcher->addNewUser(ready_peer,username);
				dispatcher->addUserToVector(username);
				iter = find(peers.begin(), peers.end(), ready_peer);
				if (iter != peers.end()) //peer was found
				{
					peers.erase(iter);
				}
			}
			else
			{
				writeCommand(ready_peer,USERNAME_TAKEN);
			}
		}
		else
		{
			writeCommand(ready_peer,BAD_COMMAND);
		}

	}
}

void LoginAndSignUp::addPeer(TCPSocket* peer)
{
	peers.push_back(peer);
	writeCommand(peer,LOGIN_OR_SIGNUP);
	if (status == false)
	{
		status = true;
		this->start();
	}

}

void LoginAndSignUp::printUsers()
{
	dispatcher->printConnectedUsers();
}
void LoginAndSignUp::ptintSessions()
{
	dispatcher->ptintSessions();
}


vector<string> LoginAndSignUp::getAllUsers()
{
	vector<string> names;
	string line;
	string name;
	bool found = false;
	char delim = ':';
	ifstream readFromFile;

	readFromFile.open("users.txt");
	if (!readFromFile)
	{
		cout << "Unable to open file " << readFromFile <<  endl;
		return names;
	}

	while (getline(readFromFile,line))
	{
		name.clear();
		for (unsigned int i=0 ; i < line.length() ; i++)
		{
			if (line[i] != delim)
			{
				name += line[i];
			}
			else
			{
				break;
			}
		}
		names.push_back(name);
	}

	return names;
}

bool  LoginAndSignUp::login(TCPSocket* peer,string name,string pass)
{
	string file_line;
	string user_and_pass;
	ifstream readFromFile;
	bool flag = false;
	bool nameExist = false;

	readFromFile.open("users.txt");
	if (!readFromFile)
	{
		cout << "Unable to open file " << readFromFile <<  endl;
		return false;
	}

	user_and_pass = name + ':' + pass;

	while (flag == false && getline(readFromFile,file_line))
	{
		if (file_line == user_and_pass)
		{
			flag = true;
			break;
		}

	}
	nameExist = dispatcher->checkExistUser(name);

	if (flag == true && nameExist == false) //name was found and isn't logged in yet
	{
		return true;
	}
	else
		return false;

}
bool LoginAndSignUp::signup(TCPSocket* peer,string name,string pass)
{
	ofstream writeToFile;
	ifstream readFromFile;
	string line;
	string user,password;
	char tmp;
	unsigned int i;
	bool flag = false;

	readFromFile.open("users.txt" );
	if (!readFromFile)
	{
		cout << "Unable to open file " << readFromFile <<  endl;
		return false;
	}

	while (flag == false && getline(readFromFile,line))
	{
		user.clear();
		password.clear();
		for (i = 0 ; i < line.length() ; i++)
		{
			tmp=line[i];
			if (tmp != ':')
				user += tmp;
			else
				break;
		}
		i++;
		for(; i<line.length(); i++)
			if(line[i])
				password+=line[i];
		if (user == name)
				flag = true;
		if(flag)
			break;
	}
	if (flag == true) //user is taken
		return false;
	readFromFile.close();
	line = name + ':' + pass;

	writeToFile.open("users.txt", ios::out | ios::app);
	if (!writeToFile)
	{
		cout << "Unable to open file: " << writeToFile << endl;
	}

	writeToFile << line << "\n";
	writeToFile.close();
	return true;
}
