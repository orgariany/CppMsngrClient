//============================================================================
// Name        : UDPMessenger
// Author      : Eliav Menachi
// Version     :
// Copyright   : Your copyright notice
// Description : UDP Messenger application
//============================================================================

#include "UDPMessenger.h"

using namespace std;

void UDPMessenger::run() {
	char buff[256];
	size_t pos = 0;

	while (this->running) {
		size_t pos = 0;
		int rc = udpSocket->recv(buff, 1500);
		if (rc > 0) {
			buff[rc] = 0;
			cout << buff << endl;
		} else {
			perror("Error at the UDPrecv");
		}
	}
	cout << "closing receiver thread" << endl;
}

UDPMessenger::UDPMessenger(int myPORT) {
	running = true;
	udpSocket = new UDPSocket(myPORT);
	this->start();
}

void UDPMessenger::sendTo(string msg, string ip, int port) {
	udpSocket->sendTo(msg, ip, port);
}

void UDPMessenger::reply(string msg) {
	udpSocket->reply(msg);
}

void UDPMessenger::close() {
	running = false;
	udpSocket->cclose();
	this->waitForThread();
	delete udpSocket;
	cout << "The UDP session was successfully closed" << endl;
}

