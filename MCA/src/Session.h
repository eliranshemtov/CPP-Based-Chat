
#ifndef SESSION_H_
#define SESSION_H_

#include "Connector.h"

class Session: public MThread {
public:
	UDPSocket* sock;
	int port;
	char* buff;
	bool running;

	// C'tor that gets a port int
	Session(int port);

	// session - run reciving loop
	void run();

	// the udpSocket sends data to given port+ip
	void sendTo(string msg,string ip,int port);

	// the udpSocket replies data to the session peer
	void reply(string msg);

	// end the session
	void close();

	// D'tor - invokes close
	virtual ~Session();
};

#endif /* SESSION_H_ */
