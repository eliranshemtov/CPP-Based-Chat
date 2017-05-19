#include "Session.h"

// C'tor that gets a port int
Session::Session(int port)
{
	this->buff = NULL;
	this->sock = new UDPSocket(port);
	this->port = port;
	running = true;
	this->start();
}


// session - run reciving loop (UDPSOCKET RECV)
void Session::run()
{
	buff = (char*)calloc(100 , sizeof(char));
	while(running)
	{
		buff = (char*)calloc(100 , sizeof(char));

		if(sock->recv(buff , 100)< 0)
			cout<<"could not receive a message"<<endl;
		else
			cout<<buff<<endl;

		delete[] buff;
	}
}

// the udpSocket sends data to given port+ip
void Session::sendTo(string msg,string ip,int port)
{
	int num = this->sock->sendTo(msg,ip,port);
	if(num < 0)
		cout<<"could not send a message"<<endl;

}

// the udpSocket replies data to the session peer
void Session::reply(string msg)
{
	if(this->sock->reply(msg) < 0)
		cout<< "could not replay a message"<<endl;
}
// end the session
void Session::close()
{
	this->~MThread();
	this->sock->cclose();
	this->waitForThread();
	free(this->buff);
}

// D'tor - invokes close
Session::~Session() {
	close();
}

