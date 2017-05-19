

#include "Dispatcher.h"
#include "MessengerProtocol.h"

pthread_mutex_t login_dispatcher_mutex = PTHREAD_MUTEX_INITIALIZER;

Dispatcher::Dispatcher(vector<string> names)
{
	listener = NULL;
	usersMapMutex = PTHREAD_MUTEX_INITIALIZER;
	usersMapSize = 0;
	status = false;
	allUsersVec = names;
}

// adding user to the allusers vector
void Dispatcher::addUserToVector(string name)
{
	allUsersVec.push_back(name);
}

// adding new user
void Dispatcher::addNewUser(TCPSocket* peer,string name)
{
	if (pthread_mutex_lock(&login_dispatcher_mutex)!=0)
	{
		printf("Error locking mutex");
		exit(1);
	}
	User* user = new User;

	user->socket = peer;
	user->name = name;
	user->connection = false;
	user->connectedToUser = NULL;


	cout << user->name << " is now connected" << endl;

	usersMap[name] = user;
	usersVector.push_back(peer);

	if (status == false)
	{
		status = true;
		this->start();
	}
	if (pthread_mutex_unlock(&login_dispatcher_mutex)!=0)
	{
		printf("Error unlocking mutex");
		exit(1);
	}
}

// Dispatcher running loop: pulls a peer and listen to its commands
void Dispatcher::run()
{
	int command;

	while (status == true)
	{
		User* currentuser = new User();
		listener = new MultipleTCPSocketsListener();
		listener->addSockets(usersVector);
		TCPSocket* temp_sock = listener->listenToSocket(2);	//listener is pulling readypeer
		delete(listener);		//which will be the one who will send the commands

		if (temp_sock != NULL)
		{
			command = readCommand(temp_sock);
			currentuser->socket = temp_sock;
		}
		else
			continue;

		if (command == 0)
		{
			continue;
		}
		else if (command == OPEN_SESSION_WITH_USER)
		{
			openNewSession(currentuser);
		}
		else if (command == CLOSE_SESSION_WITH_USER)
		{
			closeSession(currentuser);
		}
		else if (command == EXIT)
		{
			userExit(currentuser);
		}
		else if (command == GET_ALL_USERS)
		{
			this->allUsersList(currentuser);
		}

		else if (command == GET_ALL_CONNECTED_USERS)
		{
			this->usersList(currentuser);
		}
	}
}


void Dispatcher::openNewSession(User* oPuser)
{
	string msg,user_ip_and_port,target_ip_and_port;
	User* target;
	string with;
	User* user;

	msg = readMsg(oPuser->socket);
	// go over the usersMap
	for (usersIterator = usersMap.begin() ; usersIterator != usersMap.end() ; usersIterator++)
	{
		user = usersIterator->second;
		if (user->socket == oPuser->socket)
		{
			break;
		}
	}
	// if there is no earlier connection
	if (user->connection == false)
	{		// if the user is in the users map (exists)
		if ( usersMap.find(msg) != usersMap.end() )
		{
			target = usersMap[msg];
			target_ip_and_port = target->socket->destIpAndPort();

			writeCommand(oPuser->socket,TARGET_IP_AND_PORT);
			writeMsg(oPuser->socket,target_ip_and_port);
			user_ip_and_port = oPuser->socket->destIpAndPort();

			writeCommand(target->socket,INCOMIMG_SESSION);
			writeMsg(target->socket,user->name);
			writeMsg(target->socket,user_ip_and_port);
			target->connection = true;
			target->connectedToUser = user;


			usersInSession[user->name] = target->name;
		}
		else	// else it doesnt exists or other problem
		{
			writeCommand(oPuser->socket,OPEN_SESSION_ERROR);
		}
	}
	else
	{
		writeCommand(oPuser->socket,OPEN_CONNECTION_ERROR);
	}

}

// send to the user all user's list
void Dispatcher::allUsersList(User* user)
{
	writeCommand(user->socket,ALL_USERS_LIST);
	writeCommand(user->socket,allUsersVec.size());	// send the size of the list

	for (unsigned int i=0 ; i < allUsersVec.size() ; i++)
	{
		writeMsg(user->socket,allUsersVec[i]);
	}
}

// close the session
void Dispatcher::closeSession(User* CLuser )
{

	string partner;
	User* user;
	bool first = false;
	bool second = false;


	for (usersIterator = usersMap.begin() ; usersIterator != usersMap.end() ; usersIterator++)
	{
		user = usersIterator->second;
		if (user->socket == CLuser->socket)
		{
			break;
		}
	}

	for (usersInSessionIterator = usersInSession.begin() ; usersInSessionIterator != usersInSession.end() ; usersInSessionIterator++)
	{
		if (usersInSessionIterator->first== user->name) //user has an open session that he started
		{
			partner = usersInSessionIterator->second;
			first = true;

		}
		else if (usersInSessionIterator->second == user->name) //user has an open session that he DIDNT started
		{
			partner = usersInSessionIterator->first;
			second = true;

		}
	}

	if (first == true || second == true)
	{
		writeCommand((usersMap[partner])->socket,SESSION_ENDED);
		usersMap[partner]->connection = false;
		usersMap[partner]->connectedToUser = NULL;
		usersMap[user->name]->connection = false;
		usersMap[user->name]->connectedToUser = NULL;

		if (first == true)
		{
			usersInSession.erase(user->name);
		}
		else if (second == true)
		{
			usersInSession.erase(partner);
		}
	}
	else
	{
		writeCommand(user->socket,CLOSE_SESSION_ERROR);
	}

}



// send the connected users list
void Dispatcher::usersList(User* user)
{
	string user_name;
	writeCommand(user->socket,USERS_LIST);
	writeCommand(user->socket,usersMap.size());
	for (usersIterator = usersMap.begin() ; usersIterator != usersMap.end() ; usersIterator++)
	{
		user_name = usersIterator->first;
		writeMsg(user->socket,user_name);
	}
}


// user disconnection
void Dispatcher::userExit(User* deleteuser)
{
	string name;
	User* user;
	for (usersIterator = usersMap.begin() ; usersIterator != usersMap.end() ; usersIterator++)
	{
		user = usersIterator->second;
		if (user->socket == deleteuser->socket)
			break;
	}

	writeCommand(deleteuser->socket,END_MSG);

	cout << user->name << " disconnected" << endl;
	usersMap.erase(user->name);
}


// print connected users list
void Dispatcher::printConnectedUsers()
{
	if (pthread_mutex_lock(&usersMapMutex)!=0)
	{
		printf("Error");
		exit(1);
	}

	if (usersMap.empty() == true)
	{
		cout <<"No connected users" << endl;
	}
	else
	{
		for (usersIterator = usersMap.begin() ; usersIterator != usersMap.end() ; usersIterator++)
		{
			cout << "Connected user: " << usersIterator->first << endl;
		}
	}
	if (pthread_mutex_unlock(&usersMapMutex)!=0)
	{
		printf("Error");
		exit(1);
	}
}

//	print active session list
void Dispatcher::ptintSessions()
{
	if (this->usersInSession.size() != 0)
	{
		cout << "Sessions:" << endl;
		for (usersInSessionIterator = usersInSession.begin() ; usersInSessionIterator != usersInSession.end() ; usersInSessionIterator++)
		{
			cout << usersInSessionIterator->first;
			cout << " is in session with ";
			cout << usersInSessionIterator->second << endl;
		}
	}
	else
	{
		cout << "There are no active sessions at the moment" << endl;
	}
}


// gets a username and checks if it exists
bool Dispatcher::checkExistUser(string name)
{
	return usersMap.find(name) != usersMap.end();
}

void Dispatcher::close()
{
	status = false;
	delete(listener);
}

Dispatcher::~Dispatcher()
{

}

