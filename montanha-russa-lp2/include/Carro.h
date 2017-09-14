/*
 * Carro.h
 *
 *  Created on: 17 de abr de 2017
 *      Author: bruno
 */

#ifndef CARRO_H_
#define CARRO_H_

class Parque;

class Carro {
public:
	static const int CAPACIDADE;
	static int numPassageiros;
	static int voltaAcabou;
	static int cheio;
	static int voltas;

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
