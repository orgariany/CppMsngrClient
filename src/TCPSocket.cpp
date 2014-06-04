/*
 * TCPSocket.cpp
 *
 *  Created on: Feb 6, 2013
 *      Author: Eliav Menachi
 */

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
#include <stdlib.h>
#include "TCPSocket.h"
using namespace std;

void clean_up(int cond, int *sock) {
	printf("Exiting now.\n");
	close(*sock);

}

TCPSocket::TCPSocket(string peerIp, int port) {
	this->sock = socket(AF_INET, SOCK_STREAM, 0);
	if (this->sock < 0) {
		perror("Error opening channel");
		close(sock);
		exit(1);
	}

	bzero(&this->peerInfo, sizeof(peerInfo));
	peerInfo.sin_family = AF_INET;
	peerInfo.sin_addr.s_addr = inet_addr(peerIp.c_str());
	peerInfo.sin_port = htons(port);

	if (connect(this->sock, (struct sockaddr *) &peerInfo, sizeof(peerInfo))
			< 0) {
		perror("Error establishing communications");
		close(sock);
		cout<<"pls stry again"<<endl;
	} else {
		cout << "Client and server are connected" << endl;
	}

}


int TCPSocket::recv(char* buffer, int length) {
	return read(sock, buffer, length);
}

int TCPSocket::send(const char* msg, int len) {
	return write(sock, msg, len);
}

void TCPSocket::cclose() {
	close(sock);
	clean_up(0, &sock);
}

string TCPSocket::fromAddr() {
	char str[INET_ADDRSTRLEN];
	return inet_ntop(AF_INET, &(peerInfo.sin_addr), str, INET_ADDRSTRLEN);
}
