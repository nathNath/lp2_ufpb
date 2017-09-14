/*
 Thread.cpp
 *
 *  Created on: 16 de abr de 2017
 *      Author: Suporte
 */

#include "include/Thread.h"
#include <iostream>


Thread::Thread() {
	// TODO Auto-generated constructor stub
}

Thread::~Thread() {
	// TODO Auto-generated destructor stub
}

void Thread::start() {
	/*creating thread*/
	pthread_create(&tid, NULL, staticRun, this);
}

void Thread::join() {
	pthread_join(tid, NULL);
}

pthread_t Thread::getId() {
	return tid;
}

void *Thread::staticRun(void *args) {
	Thread *t = (Thread*) args;

	t->run();

	return NULL;
}


