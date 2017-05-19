

#ifndef DISPATCHER_H_
#define DISPATCHER_H_

#include "Connector.h"
#include "User.h"


class Dispatcher : public MThread {

	public:
	bool status;
	vector<string> allUsersVec;
	map<string,User*> usersMap;
	pthread_mutex_t usersMapMutex;
	map<string,User*>::iterator usersIterator;
	map<string,User*>::size_type usersMapSize;
	map<string,string> usersInSession;
	map<string,string>::iterator usersInSessionIterator;
	vector<TCPSocket*> usersVector;
	MultipleTCPSocketsListener* listener;


	Dispatcher(vector<string> usersNames);
	void run();
	void close();
	virtual ~Dispatcher();

	void addNewUser(TCPSocket* peer,string name); // adding new user
	void addUserToVector(string name); // adding user to the allusers vector
	void usersList(User* user); //The list of connect users
	void userExit(User* user); //Delete user
	void allUsersList(User* user); //To send the list of users to the user
	void printConnectedUsers();//print the list of the connected users
	void ptintSessions();//print all sessions
	bool checkExistUser(string name);//check if user exist

	void openNewSession(User* user); //To open a new session with user
	void closeSession(User* user);//To close a session with user


};

#endif /* DISPATCHER_H_ */
