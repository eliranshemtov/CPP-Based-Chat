
#ifndef SERVER_H_
#define SERVER_H_


#include "LoginAndSignUp.h"
#include "Connector.h"


class Server : public MThread {

	TCPSocket* sock; //listen socket
	LoginAndSignUp* loginAndSign; // incoming connection are handled by this object
	bool flag;



public:
	Server();
	void printAllUsers();
	void printUsers();
	void ptintSessions();
	void close();

	virtual ~Server();
	void run();
};


#endif /* SERVER_H_ */
