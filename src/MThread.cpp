/*
 * Mthread.cpp
 *
 *  Created on: Feb 13, 2013
 *      Author: Eliav Menachi
 */

#include "MThread.h"
#include <iostream>
#include <stdio.h>
using namespace std;

void* worker(void* arg) {
	MThread* threadObj = (MThread*) arg;
	threadObj->run();
	threadObj->threadId = -1;
	return NULL;
}

void MThread::start() {
	pthread_create(&threadId, NULL, worker, (void*) this);
}

void MThread::waitForThread() {
	pthread_join(threadId, NULL);
}

MThread::~MThread() {
	if (threadId > 0) {
		//kills the thread if exist
		pthread_cancel(threadId);
		printf("Thread was canceled\n");
	}
}

