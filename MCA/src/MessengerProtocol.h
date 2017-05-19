
#ifndef TCPMESSENGERPROTOCOL_H_
#define TCPMESSENGERPROTOCOL_H_

/**
 * TCP Messenger protocol:
 * all messages are of the format [Command 4 byte int]
 * and optionally data follows in the format [Data length 4 byte int][ Data ]
 */
#include <iostream>
#include <sys/types.h>
#include <netinet/in.h>
#include <inttypes.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <algorithm>
#include "MThread.h"
#include "MultipleTCPSocketsListener.h"
#include "TCPSocket.h"
#include "UDPSocket.h"


using namespace std;

#define SERVER_PORT 3346

#define CLOSE_SESSION_WITH_USER 	1
#define OPEN_SESSION_WITH_USER 		2
#define EXIT						3
#define SEND_MSG_TO_PEER			4
#define SESSION_REFUSED				5
#define SESSION_ESTABLISHED			6
#define LOGIN						7
#define SIGNUP 						8
#define GET_ALL_USERS				9
#define ENTER_USERNAME				10
#define ENTER_PASSWORD				11
#define CONFIRM_USER 				12
#define LOGIN_DENIED 				13
#define USERNAME_TAKEN 				14
#define BAD_COMMAND 				15
#define ENTER_USER_NAME 			16	//"Enter user name: "
#define INCOMIMG_SESSION 			17 	//"Incoming session from: "
#define OPEN_SESSION_ERROR 			18 	//"Username not found "
#define CLOSE_SESSION_ERROR 		19 	//"You are not in session "
#define OPEN_CONNECTION_ERROR 		20 	//"You have an open connection"
#define SESSION_ENDED 				21
#define OPEN_SESSION_ERROR2			22  //"You have an open session"
#define CONNECTION_ERROR 			23  //"You don't have any open connections"
#define END_MSG 					24
#define TARGET_IP_AND_PORT 			25
#define LOGIN_OR_SIGNUP 			26
#define USERS_LIST 					27
#define ALL_USERS_LIST 				28
#define GET_ALL_CONNECTED_USERS		29


#define TEST_PEER_NAME "test"
#define SESSION_REFUSED_MSG "Problem with connection to peer , peer is busy or disconnected,please try again later"


#endif /* TCPMESSENGERPROTOCOL_H_ */
