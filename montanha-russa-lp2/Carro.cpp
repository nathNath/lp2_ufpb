/*
 * Carro.cpp
 *
 *  Created on: 17 de abr de 2017
 *      Author: bruno
 */

#include <iostream>
#include <chrono>
#include <thread>
#include <atomic>
#include "include/Thread.h"
#include "include/Carro.h"
#include "include/Parque.h"

using namespace std;

const int Carro::CAPACIDADE = 5;

int Carro::numPassageiros = 0;
int Carro::voltaAcabou = 0;
int Carro::cheio = 0;
int Carro::voltas = 0;

atomic<int> carroVoltaAcabou = ATOMIC_VAR_INIT(Carro::voltaAcabou);
atomic<int> carroCheio = ATOMIC_VAR_INIT(Carro::cheio);

Carro::Carro() {
}

Carro::~Carro() {
}

void Carro::esperaEncher() {
	while (Carro::numPassageiros < Carro::CAPACIDADE && Parque::numPessoas) {
		this_thread::sleep_for(chrono::seconds(5));
	}

	if(!Parque::numPessoas){
		cout << "O CARRO encerrou as suas atividades." << endl;
	} else{
		atomic_fetch_add(&carroCheio, 1);
		Carro::cheio = carroCheio;
	}
}

void Carro::daUmaVolta() {
	this_thread::sleep_for(chrono::seconds(10));

	atomic_fetch_add(&carroVoltaAcabou, 1);
	Carro::voltaAcabou = carroVoltaAcabou;
}

void Carro::esperaEsvaziar() {

	while (Carro::numPassageiros > 0)
		this_thread::sleep_for(chrono::seconds(5));

	cout << "O CARRO foi esvaziado." << endl;
}

int Carro::getNVoltas() {
	return this->voltas;
}

void Carro::run() {
	while (Parque::numPessoas) {

		cout << "CARRO: Esperando lotar" << endl;
		esperaEncher();


		if(!Parque::numPessoas) {
			break;
		}

		cout << "CARRO: Esperando a volta " << Carro::voltas+1 << " ser completada" << endl;

		Carro::voltas++;

		daUmaVolta();


		cout << "CARRO: Esperando o carro ser esvaziado" << endl;
		esperaEsvaziar();


	}
}
