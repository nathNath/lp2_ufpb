/*
 * Parque.h
 *
 *  Created on: 17 de abr de 2017
 *      Author: bruno
 */

#ifndef PARQUE_H_
#define PARQUE_H_

#include <vector>

class Passageiro;

using namespace std;

class Parque {
public:
	static int numPessoas;

	Parque();
	virtual ~Parque();

	void addPassageiro(Passageiro *p);
	std::vector<Passageiro*>& getPassageiros();

private:
	std::vector<Passageiro*> passageiros;
};

#endif /* PARQUE_H_ */
