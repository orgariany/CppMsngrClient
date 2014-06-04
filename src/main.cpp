//============================================================================
// Name        : CLient Messenger APP
// Author      : Ben Elany
// Version     :
// Copyright   :
// Description :
//============================================================================

#include <iostream>
#include "TCPSocket.h"
#include "MThread.h"
#include "TCPMessengerProtocol.h"
#include "ClientMessenger.h"
using namespace std;

void printInstructions() {
	cout << "c <server ip> to open connect to server type:" << endl;
	cout << "Opening session will close any previously active sessions" << endl;
	cout << "lu - to print the user list from the server" << endl;
	cout << "lcu - to print the connected users list" << endl;
	cout << "lr - to print all rooms" << endl;
	cout << "lru <room name> -  to print all users in this room" << endl;
	cout<< "o <username | room name> - to open a session with the user / enter a room"<< endl;
	cout << "r <roomname> -  to open a new room" << endl;
	cout << "s <message> -  to send a message to a cleint or room" << endl;
	cout<< "l - print the current status of the client (connected to “xyz”/not connected)"<< endl;
	cout << "cs - to disconnect the open session / exit from a room" << endl;
	cout << "d - To disconnect from server" << endl;
	cout << "x- To exit " << endl;

}
//TO DO :
// *UDP random port - check if work
// *check if the 'o' function is working ( spliting string(the ip's))
// *if need - find a way to send the server the random UDP Port
// *to check and delete(if neccesery) the class members in 'clientMessenger'

int main() {
	ClientMessenger* client;
	cout << "Welcome to messenger Client" << endl;
	printInstructions();
	client = new ClientMessenger();
	bool loop = true;
	while (loop) {
		string command;
		cin >> command;
		if (command == "c") {
			string serverIP;
			cin >> serverIP;
			client->connect(serverIP);
		} else if (command == "lu") {
			client->getUserList();
		} else if (command == "lcu") {
			client->getConnectedUserList();
		} else if (command == "lr") {
			client->getAllRooms();
		} else if (command == "lru") {
			client->getAllUsersInTheRoom();
		} else if (command == "o") {
			string input;
			cin >> input;
			client->connectTopeerOrRoom(input);
		} else if (command == "r") {
			client->createRoom();
		} else if (command == "s") {
			string msg;
			getline(cin,msg);
			client->sendMessege(msg);
		} else if (command == "l") {
			string user;
			cin >> user;
			client->clientStatus(user);
		} else if (command == "cs") {
			client->ActivecloseSessionWithPeer();
		} else if (command == "d") {
			client->closeConnection();
		} else if (command == "x") {
			client->closeConnection();
			loop = false;
		} else {
			cout << "Unknown command" << endl;
			printInstructions();
		}
	}
	return 0;
}
