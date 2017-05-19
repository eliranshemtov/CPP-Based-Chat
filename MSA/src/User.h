#ifndef USER_H_
#define USER_H_

#include "Connector.h"

class User {

public:
	TCPSocket* socket;
	bool connection;//flag
	User* connectedToUser;
	string name;
public:
	User();
	virtual ~User();
};

#endif /* USER_H_ */
