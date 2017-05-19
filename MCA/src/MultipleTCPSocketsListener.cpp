
#include "MultipleTCPSocketsListener.h"

using namespace std;



void MultipleTCPSocketsListener::addSocket(TCPSocket* socket){
	sockets.push_back(socket);
}

void MultipleTCPSocketsListener::addSockets(vector<TCPSocket*> socketVec){
	sockets = socketVec;
}

TCPSocket* MultipleTCPSocketsListener::listenToSocket(int timeout){
	struct timeval tv = {timeout, 0};
	tSocketsContainer::iterator iter = sockets.begin();
	tSocketsContainer::iterator endIter = sockets.end();
	fd_set fdset;
	FD_ZERO(&fdset);
	int highfd = 0;

	for (;iter != endIter;iter++) {
		highfd++;
		FD_SET((*iter)->getSocketFd(), &fdset);
	}

	int returned;
	if (timeout>0){
		returned = select(sizeof(fdset)*8, &fdset, NULL, NULL, &tv);
	}else{
		returned = select(sizeof(fdset)*8, &fdset, NULL, NULL, NULL);
	}
	if (returned) {
		for (int i = 0; i < highfd; i++) {
			TCPSocket* tmpSocket = sockets[i];
			if (FD_ISSET(tmpSocket->getSocketFd(), &fdset)) {
				return tmpSocket;
			}
		}
	}
	return NULL;
}

