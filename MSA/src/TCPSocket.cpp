
#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <stdio.h>
#include "TCPSocket.h"
using namespace std;

// private constructor to create a secondary server socket to communicate with a remote peer
TCPSocket::TCPSocket(int connected_sock,struct sockaddr_in serverAddr,struct sockaddr_in peerAddr){
	this->serverAddr =  serverAddr;
	this->peerAddr =  peerAddr;
	this->socket_fd = connected_sock;
}


// Constructor create a TCP server socket
TCPSocket::TCPSocket(int port){

	this->socket_fd = socket (AF_INET, SOCK_STREAM, 0);

	// clear the s_in struct
	bzero((char *) &this->serverAddr, sizeof(this->serverAddr));  /* They say you must do this    */

	//sets the sin address
	this->serverAddr.sin_family = (short)AF_INET;
	this->serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);
	this->serverAddr.sin_port = htons((u_short)port);

	//bind the socket on the address
	printf("TCP server binding...\n");
	if (bind(this->socket_fd, (struct sockaddr *)&this->serverAddr, sizeof(this->serverAddr)) < 0)
	{
		perror ("Error naming channel");
	}
}

// Constructor creates TCP client socket
TCPSocket::TCPSocket(string peerIp, int port){
	cout<<"Opening new client socket"<<endl;

	this->socket_fd = socket (AF_INET, SOCK_STREAM, 0);

	// clear the s_in struct
	bzero((char *) &this->peerAddr, sizeof(this->peerAddr));

	//sets the sin address
	this->peerAddr.sin_family = (short)AF_INET;
	this->peerAddr.sin_addr.s_addr = inet_addr(peerIp.data());
	this->peerAddr.sin_port = htons((u_short)port);

	if (connect(this->socket_fd, (struct sockaddr *)&this->peerAddr, sizeof(this->peerAddr)) < 0)
	{
		perror ("Error establishing communications");
		close(this->socket_fd);
	}
}

// Perform listen and accept on server socket
TCPSocket* TCPSocket::listenAndAccept(){
	int rc = listen(this->socket_fd, 1);
	if (rc<0){
		return NULL;
	}
	socklen_t len = sizeof(this->peerAddr);
	bzero((char *) &this->peerAddr, sizeof(this->peerAddr));

	int connect_sock = accept(this->socket_fd, (struct sockaddr *)&this->peerAddr, &len);
	if (connect_sock<0)return NULL;
	return new TCPSocket(connect_sock,this->serverAddr,this->peerAddr);
}

// Read from socket into the given buffer up to the buffer given length and return the number of bytes read
int TCPSocket::recv(char* buffer, int length){
	return read(socket_fd,buffer,length);
}

// send the given buffer to the socket
int TCPSocket::send(const char* msg, int len){
	return write(this->socket_fd,msg,len);
}

// close the socket and free all resources
void TCPSocket::cclose(){
	cout<<"Closing socket..."<<endl;
	shutdown(this->socket_fd,SHUT_RDWR);
	close(this->socket_fd);
}

// return the address of the connected peer
string TCPSocket::fromAddr(){
	return inet_ntoa(this->peerAddr.sin_addr);
}

string TCPSocket::destIpAndPort(){
	string str = inet_ntoa(this->peerAddr.sin_addr);
	str.append(":");
	char buff[10];
	sprintf(buff,"%d",ntohs(this->peerAddr.sin_port));
	str.append(buff);
	return str;
}

/**
 * returns the c socket fd
 */
int TCPSocket::getSocketFd(){
	return socket_fd;
}
