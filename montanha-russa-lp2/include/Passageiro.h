/*
 * Passageiro.h
 *
 *  Created on: 17 de abr de 2017
 *      Author: bruno
 */

#ifndef PASSAGEIRO_H_
#define PASSAGEIRO_H_

#include <atomic>

using namespace std;

class Carro;
class Parque;

class Passageiro {
private:
	int id;
	Carro *carro;
	Parque *parque;
public:

	static atomic<int> ticket[];
	static bool entrada[];

	Passageiro(int id, Carro *c, Parque *p);
	virtual ~Passageiro();
	void entraNoCarro();
	void esperaVoltaAcabar();
	void saiDoCarro();
	void passeiaPeloParque();
	bool parqueFechado();

	void run();

};

#endif /* PASSAGEIRO_H_ */
