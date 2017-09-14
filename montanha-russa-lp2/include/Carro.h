/*
 * Carro.h
 *
 *  Created on: 17 de abr de 2017
 *      Author: bruno
 */

#ifndef CARRO_H_
#define CARRO_H_

#include <atomic>

using namespace std;

class Parque;

class Carro {
private:
	static int voltas;
public:
	static const int CAPACIDADE;
	static const int MAX_VOLTAS;
	static atomic<int> numPassageiros;
	static atomic_bool voltaAcabou;
	static atomic_flag lock;

	Carro();
	virtual ~Carro();
	void esperaEncher();
	void daUmaVolta();
	void esperaEsvaziar();
	int getNVoltas();
	void run();

	void operator()()
	{
		run();
	}

};

#endif /* CARRO_H_ */
