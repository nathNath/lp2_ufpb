/*
 * Parque.cpp
 *
 *  Created on: 17 de abr de 2017
 *      Author: bruno
 */

#include "include/Parque.h"
#include "include/Passageiro.h"
#include "include/Thread.h"

int Parque::numPessoas = 10;

Parque::Parque() {
}

Parque::~Parque() {
}

void Parque::addPassageiro(Passageiro *p) {
	passageiros.push_back(p);
}

vector<Passageiro*>& Parque::getPassageiros() {
	return passageiros;

}
