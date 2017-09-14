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

//Tempo para uma volta dentro do carro
#define TEMPO_VOLTA 3

const int Carro::CAPACIDADE = 5;
const int Carro::MAX_VOLTAS = 10;

//Variaveis atomicas, para controle do fluxo
atomic<int> Carro::numPassageiros = ATOMIC_VAR_INIT(0);
atomic_bool Carro::voltaAcabou = ATOMIC_VAR_INIT(false);
atomic_flag Carro::lock;

int Carro:voltas = 0;

Carro::Carro() {
	this->voltas = 0;
}

Carro::~Carro() {
}

void Carro::esperaEncher() {
	while(Carro::numPassageiros < Carro::CAPACIDADE && this->voltas < Carro::MAX_VOLTAS){
		;
	}
/*	
	while (Carro::numPassageiros < Carro::CAPACIDADE && Parque::numPessoas) {
		this_thread::sleep_for(chrono::seconds(5));
	}

	if(!Parque::numPessoas){
		cout << "O CARRO encerrou as suas atividades." << endl;
	} else{
		atomic_fetch_add(&carroCheio, 1);
		Carro::cheio = carroCheio;
	}
*/
}


void Carro::daUmaVolta() {
	Carro::voltaAcabou = false;
	this_thread::sleep_for(chrono::seconds(TEMPO_VOLTA));
	Carro::voltaAcabou = true;
/*	
	this_thread::sleep_for(chrono::seconds(10));

	atomic_fetch_add(&carroVoltaAcabou, 1);
	Carro::voltaAcabou = carroVoltaAcabou;
*/
}

void Carro::esperaEsvaziar() {
	while(Carro::numPassageiros > 0){
		;
	}
/*
	while (Carro::numPassageiros > 0)
		this_thread::sleep_for(chrono::seconds(5));

	cout << "O CARRO foi esvaziado." << endl;
*/
}

int Carro::getNVoltas() {
	return this->voltas;
}

void Carro::run() {
	while (Parque::numPessoas.load(std::memory_order_acquire) > 0) {

		cout << "CARRO: Esperando lotar" << endl;
		esperaEncher();


		if(Parque::numPessoas.load(std::memory_order_acquire) == 0) {
			break;
		}
		// Verifica se alguém está "locando" a variável
		while(Carro::lock.test_and_set()){
			;
		}
		cout << "CARRO: Esperando a volta " << Carro::voltas+1 << " ser completada" << endl;
		Carro::lock.clear();
		

		daUmaVolta();
		Carro::voltas++;

		cout << "CARRO: Esperando o carro ser esvaziado" << endl;
		esperaEsvaziar();
		Carro::voltaAcabou = false;


	}
}
