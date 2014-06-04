//============================================================================
// Name        : UDPMessenger.h
// Author      : Eliav Menachi
// Version     :
// Copyright   : Your copyright notice
// Description : UDP Messenger application
//============================================================================

#include <pthread.h>
#include <time.h>
#include "UDPSocket.h"
#include "MThread.h"
#include <stdlib.h>

using namespace std;

class UDPMessenger: public MThread{
	UDPSocket* udpSocket;
	bool running;

public:
	UDPMessenger(int myPORT);

	void sendTo(string msg,string ip,int port);

	void reply(string msg);

	void close();

	bool getRunning(){return this->running;}

	/**
	 * This method runs in a separate thread, it reads the incoming messages
	 * from the socket and prints the content on the terminal.
	 * The thread should exist when the socket is closed
	 */
	void run();
};
