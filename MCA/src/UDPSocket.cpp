
#include "UDPSocket.h"
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>

UDPSocket::UDPSocket(int port){

	this->socket_fd = socket (AF_INET, SOCK_DGRAM, 0);

	// clear the s_in struct
	bzero((char *) &this->s_in, sizeof(this->s_in));  /* They say you must do this    */

	//sets sin address
	this->s_in.sin_family = (short)AF_INET;
	this->s_in.sin_addr.s_addr = htonl(INADDR_ANY);
	this->s_in.sin_port = htons((u_short)port);

	fsize = sizeof(this->from);

	//bind the socket on the address
	if(bind(this->socket_fd, (struct sockaddr *)&this->s_in, sizeof(this->s_in))<0){
		perror ("Error naming channel");
	}
}

int UDPSocket::recv(char* buffer, int length){
	return recvfrom(this->socket_fd,buffer,length,0,(struct sockaddr *)&this->from,&this->fsize);
}

int UDPSocket::sendTo(string msg, string ip, int port){
	struct sockaddr_in toAddr;
	bzero((char *) &toAddr, sizeof(toAddr));
	toAddr.sin_family = AF_INET;
	toAddr.sin_addr.s_addr = inet_addr(ip.data());
	toAddr.sin_port = htons(port);
	return sendto(this->socket_fd,msg.data(),msg.length(),0,(struct sockaddr *)&toAddr,sizeof(toAddr));
}

int UDPSocket::reply(string msg){
	return sendto(this->socket_fd,msg.data(),msg.length(),0,(struct sockaddr *)&this->from,sizeof(this->from));
}

void UDPSocket::cclose(){
	cout<<"Closing UDP socket .."<<endl;
	shutdown(this->socket_fd,SHUT_RDWR);
	close(this->socket_fd);
}

string UDPSocket::fromAddr(){
	return inet_ntoa(this->from.sin_addr);
}



