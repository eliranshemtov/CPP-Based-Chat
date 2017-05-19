
#include "Connector.h"
#include "Session.h"


typedef struct
{
public:
	string name;
	string ip;
	int port;
	void Clean()
	{
		name.clear();
		ip.clear();
		port = 0;
	}
}peer;

class Client : public MThread
{
public:
	string client_name;
	string password;

	TCPSocket* server_sock;
	Session* udp_sock;

	peer session_peer;
	bool connectionStatus;
	bool isInSession;
	bool isLoggedIn;
	bool isOnline;

	// C'tor
	Client();

	// accept commands from server in loop
	void run();

	// Connects (TCP) the client to a given server ip (the port is Prefixed by the protocol)
	bool connectToServer(string ip,int port); // TCP

	// Send a msg to the server (Only when connected)
	void sendMsgToServer(string msg);

	// Login to server (by user's credentials)
	void login(string name,string pass);

	// Signup as new user on the server
	void signup(string name,string pass);

	// Replies a msg to an active session peer
	void replyToSession();

	// Prints all online users (not just logged in)
	void listAllUsers();

	// Prints the server's registered users
	void getUsers();

	// prints all the users list
	void gotAllUsers();

	// setting up the incoming session data
	void gotIncomingSession();

	// print related user list
	void gotUsersList();

	// Clean disconnection from Server
	void disconnectFromServer();

	// Opens UDP session between 2 clients
	void openSession();

	// Close all sessions
	void closeSession();

	// send msg to the other client on the session
	void sendMsgToSession();

	// get the ip and port of a remote peer
	void gotIpAndPort();

	// confirming user and creating his new session
	void loggedIn();

	// bool - is the client connected?
	bool ConnectionStatus();

	// bool - is the client in session?
	bool sessionStatus();

	// Close connection
	void close();

	// Summons Close method
	~Client();

};

