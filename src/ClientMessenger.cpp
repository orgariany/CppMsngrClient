#include "ClientMessenger.h"
#include "TCPMessengerProtocol.h"

ClientMessenger::ClientMessenger() {
	connectedtopeer = false;
	connected = false;
	tcpSocket = NULL;
	udpMessenger = NULL;
	dispatcher = NULL;
	myUDPport = 0;
}
void ClientMessenger::connect(string serverIP) {
	tcpSocket = new TCPSocket(serverIP, MSNGR_PORT);
	cout << readDataFromServer() << endl;
	cin >> username;
	sendDataToServer(username);
	cout << readDataFromServer() << endl;
	cin >> password;
	sendDataToServer(password);
	if ((readCommandFromServer()) == SESSION_ESTABLISHED_WITH_SERVER) {
		srand(time(NULL));
		myUDPport = rand() % 10500 + 10000;
		cout << "My UDP port is: " << myUDPport << endl;
		ostringstream convert;
		convert << myUDPport;
		cout << convert.str() << endl;
		sendDataToServer(convert.str());
		cout << "My UDP port is: " << myUDPport << endl;
		dispatcher = new ServerCommandDispatcher(this);
		dispatcher->start();
		connected = true;
		cout << "The server has approved your login" << endl;
	} else {
		perror("Error at login into the server");
		cout << "pls try again" << endl;
	}
}
void ClientMessenger::closeConnection() {
	if (connected) {
		sendCommandToServer(EXIT);
		tcpSocket->cclose();
		ips.clear();
		ports.clear();
		connected = false;
		cout << "The connection with the server has been shut down." << endl;
	} else {
		cout << "you have to connect a server first" << endl;
	}

}

void ClientMessenger::getUserList() {
	if (connected) {
		this->sendCommandToServer(PRINT_USER_LIST);
	} else {
		cout << "you have to connect a server first" << endl;
	}

}

void ClientMessenger::getConnectedUserList() {
	if (connected) {
		this->sendCommandToServer(PRINT_CONNECTED_USERS);
	}

	else {
		cout << "you have to connect a server first" << endl;
	}
}

void ClientMessenger::getAllRooms() {
	if (connected) {
		this->sendCommandToServer(PRINT_ALL_ROOMS);

	}

	else {
		cout << "you have to connect a server first" << endl;
	}
}

void ClientMessenger::getAllUsersInTheRoom() {
	if (connected) {
		string roomname;
		cin >> roomname;
		this->sendCommandToServer(PRINT_USERS_IN_ROOM);
		this->sendDataToServer(roomname);
	}

	else {
		cout << "you have to connect a server first" << endl;
	}
}

void ClientMessenger::connectTopeerOrRoom(string input) {
	if (connected) {
		if (!this->connectedtopeer) {
			sendCommandToServer(OPEN_SESSION_WITH_PEER);
			sendDataToServer(input);
		} else {
			perror("you need to close the current session first buddy");
		}
	} else {
		cout << "you have to connect a server first" << endl;
	}
}

void ClientMessenger::sendCommandToServer(int command) {
	command = htonl(command);
	tcpSocket->send((char*) &command, 4);
}
void ClientMessenger::sendMessege(string msg) {
	vector<string>::iterator it = ips.begin();
	vector<int>::iterator pt = ports.begin();
	if (connectedtopeer) {
		for (; it != ips.end() && pt != ports.end(); it++, pt++) {
			udpMessenger->sendTo(">[" + this->username + "] " + msg, *it, *pt);
		}
	} else {
		perror("you need to connect to a peer or a room first");
	}

}

void ClientMessenger::sendDataToServer(string msg) {
	int msgLen = msg.length();
	msgLen = htonl(msgLen);
	tcpSocket->send((char*) &msgLen, 4);
	tcpSocket->send(msg.data(), msg.length());
}

void ClientMessenger::clientStatus(string user) {
	sendCommandToServer(CLIENT_STATUS);
	sendDataToServer(user);
}
string ClientMessenger::readDataFromServer() {
	string msg;
	char buff[1500];
	int msgLen;
	tcpSocket->recv((char*) &msgLen, 4);
	msgLen = ntohl(msgLen);
	int totalrc = 0;
	int rc;
	while (totalrc < msgLen) {
		rc = tcpSocket->recv((char*) &buff[totalrc], msgLen - totalrc);
		if (rc > 0) {
			totalrc += rc;
		} else {
			break;
		}
	}
	if (rc > 0 && totalrc == msgLen) {
		buff[msgLen] = 0;
		msg = buff;
	} else {
		tcpSocket->cclose();
	}
	return msg;
}
int ClientMessenger::readCommandFromServer() {
	int command = 0;
	int rt = tcpSocket->recv((char*) &command, 4);
	if (rt < 1)
		return rt;
	command = ntohl(command);
	return command;
}

void ClientMessenger::createRoom() {
	string room;
	cin >> room;
	sendCommandToServer(CREATE_NEW_ROOM);
	sendDataToServer(room);
}
void ServerCommandDispatcher::run() {
	cout << "dispatcher started" << endl;
	int com;
	string delimiter = ",";
	string name;
	string res;
	while (messenger->connected) {
		com = messenger->readCommandFromServer();
		switch (com) {
		case CLOSE_SESSION_WITH_PEER:
			perror("It appear that the other peer is disconnected");
			messenger->PassivecloseSessionWithPeer();
			break;

		case EXIT:
			perror("It appear that the server gone bye bye ");
			messenger->tcpSocket->cclose();
			if (messenger->connectedtopeer) {
				messenger->udpMessenger->close();
			}
			break;

		case OPEN_SESSION_WITH_PEER:
			name = messenger->readDataFromServer();
			cout << name << " wants to open a connection with you" << endl;

			messenger->sendCommandToServer(SESSION_ESTABLISHED);
			messenger->sendDataToServer(name);
			messenger->udpMessenger = new UDPMessenger(messenger->myUDPport);
			messenger->connectedtopeer = true;

			cout << "Connected to user: " << name << endl;
			break;
		case SESSION_ESTABLISHED:
			messenger->udpMessenger = new UDPMessenger(messenger->myUDPport);
			messenger->connectedtopeer = true;
			cout << "UDP connection established" << endl;
			break;
		case CREATE_NEW_ROOM:
			cout << messenger->readDataFromServer() << endl;
			break;
		case SESSION_REFUSED:
			perror("Connection refused");
			break;

		case CLIENT_STATUS:
			cout << messenger->readDataFromServer() << endl;
			break;
		case PRINT_USER_LIST:
			cout << "Here are the total userList:" << endl
					<< messenger->readDataFromServer() << endl;
			break;

		case PRINT_CONNECTED_USERS:
			cout << "Here are the connected userList:" << endl
					<< messenger->readDataFromServer() << endl;
			break;

		case PRINT_ALL_ROOMS:
			cout << "Here are the total users in the room:" << endl
					<< messenger->readDataFromServer() << endl;
			break;

		case PRINT_USERS_IN_ROOM:

			cout << "Here are the total users in the room:" << endl
					<< messenger->readDataFromServer() << endl;
			break;

		case UPDATE_IP_LIST:
			res = messenger->readDataFromServer();
			messenger->updateUdpIPs(res);
			break;

		default:
			break;
		}
	}

}

void ClientMessenger::updateUdpIPs(string update) {

	ips.clear();
	ports.clear();
	stringstream data(update);
	string segment;
	string ip;
	int port;
	int size = 0;

	while (getline(data, segment, ',')) {
		stringstream word(segment);
		word >> ip >> port;
		ips.push_back(ip);
		ports.push_back(port);
		size++;
	}

	for (int i = 0; i < size; ++i) {
		cout << "ip " << i << " " << ips[i];
		cout << ":";
		cout << "port " << i << " " << ports[i];
		cout << endl;
	}
}

void ClientMessenger::PassivecloseSessionWithPeer() {

	this->udpMessenger->close();
	this->connectedtopeer = false;
	free(this->udpMessenger);
	this->udpMessenger = NULL;

}

void ClientMessenger::ActivecloseSessionWithPeer() {
	if (connected) {
		if (connectedtopeer) {
			sendCommandToServer(CLOSE_SESSION_WITH_PEER);
			this->udpMessenger->close();
			this->connectedtopeer = false;
			free(this->udpMessenger);
			this->udpMessenger = NULL;
		} else {
			perror(
					"you are not connected to anyone.. all alone in the world.. kill yourself.. bye.");
		}

	} else {
		cout << "you have to connect a server first" << endl;
	}

}

ServerCommandDispatcher::ServerCommandDispatcher(ClientMessenger *msgr) {
this->messenger = msgr;
}
