
#ifndef LOGINANDSIGNUP_H_
#define LOGINANDSIGNUP_H_
#define LOGIN_OR_SIGNUP 			42

#include "Dispatcher.h"
#include "Connector.h"

class LoginAndSignUp : public MThread {

	vector <TCPSocket*> peers;
	vector <TCPSocket*>::iterator iter;

	MultipleTCPSocketsListener* socketsListener;
	Dispatcher* dispatcher;
	bool status;

public:
	//C'tor
	LoginAndSignUp();

	// add peer that just connected but hasnt logged in yet
	void addPeer(TCPSocket* peer);

	// get username and password and check if they exist in the users file
	bool login(TCPSocket* peer,string name,string pass);

	// get username and password and register it
	bool signup(TCPSocket* peer,string name,string pass);

	//print all the connected users data
	void printUsers();

	//return a vector<string> of all the users in the file
	vector<string> getAllUsers();

	// print the sessions information
	void ptintSessions();


	void close();

	//LoginAndSignUp main loop
	void run();

	//D'tor
	virtual ~LoginAndSignUp();
};

#endif /* LOGINANDSIGNUP_H_ */
