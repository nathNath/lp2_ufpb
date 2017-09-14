/*
 * Passageiro.h
 *
 *  Created on: 17 de abr de 2017
 *      Author: bruno
 */

#ifndef PASSAGEIRO_H_
#define PASSAGEIRO_H_

class Carro;
class Parque;

class Passageiro {
private:
	int id;
	Carro *carro;
public:

	static int ticket[];
	static bool entrada[];

	Passageiro(int id, Carro *c);
	virtual ~Passageiro();
	void entraNoCarro();
	void esperaVoltaAcabar();
	void saiDoCarro();
	void passeiaPeloParque();
	bool parqueFechado();

	void run();

};

#endif /* PASSAGEIRO_H_ */
