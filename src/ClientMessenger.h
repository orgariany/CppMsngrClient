//============================================================================
// Nme        : TCP Messenger Server
// Author      : Eliav Menachi
// Version     :
// Copyright   :
// Description : TCP Messenger application
//============================================================================
#include <strings.h>
#include "MThread.h"
#include "TCPSocket.h"
#include "TCPMessengerProtocol.h"
#include <vector>
#include "UDPMessenger.h"
#include <sstream>
#include <iterator>
#include <string>

using namespace std;
class ClientMessenger;

class ServerCommandDispatcher: public MThread {
	ClientMessenger* messenger;
public:
	ServerCommandDispatcher(ClientMessenger* msngr);
	void run();
};

class ClientMessenger {
	friend class ServerCommandDispatcher;

private:
	bool connected;
	bool connectedtopeer;
	int myUDPport;
	string respownd;
	string connectedUsers;
	string username;
	string password;
	string command;
	vector<string> ips;
	vector<int> ports;
	UDPMessenger* udpMessenger;
	TCPSocket* tcpSocket;
	ServerCommandDispatcher *dispatcher;

public:
	ClientMessenger();
	void connect(string serverIP);
	void connectTopeerOrRoom(string input);
	void getUserList();
	void getConnectedUserList();
	void getAllRooms();
	void getAllUsersInTheRoom();
	void sendMessege(string msg);
	int readCommandFromServer();
	void sendCommandToServer(int command);
	void sendDataToServer(string msg);
	void closeConnection();
	void updateUdpIPs(string update);
	void ActivecloseSessionWithPeer();
	void PassivecloseSessionWithPeer();
	void exit();
	void createRoom();
	void clientStatus(string user);
	UDPMessenger* getUdpMessenger() {
		return this->udpMessenger;
	}

	string readDataFromServer();
};

