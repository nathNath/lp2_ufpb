/*
 * Thread.h
 *
 *  Created on: 16 de abr de 2017
 *      Author: Suporte
 */

#ifndef THREAD_H_
#define THREAD_H_

#include <pthread.h>

class Thread {
public:
	Thread();
	virtual ~Thread();
	void start();
	virtual void run() = 0;
	void join();
	pthread_t getId();

private:
	pthread_t tid;

	static void *staticRun(void *args);
};

#endif /* THREAD_H_ */
