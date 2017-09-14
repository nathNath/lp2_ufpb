/*
 * Carro.cpp
 *
 *  Created on: 17 de abr de 2017
 *      Author: Bruno, Nathália e Janse
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
// #define TEMPO_VOLTA 3

const int Carro::TEMPO_VOLTA = 3;
const int Carro::CAPACIDADE = 5;
const int Carro::MAX_VOLTAS = 2;

//Variaveis atomicas, para controle do fluxo
atomic<int> Carro::numPassageiros(0);
atomic_bool Carro::voltaAcabou(false);
atomic_flag Carro::lock;

int Carro::voltas = 0;

Carro::Carro() {
	this->voltas = 0;
}

Carro::~Carro() {
}

void Carro::esperaEncher() {
	while(Carro::numPassageiros < Carro::CAPACIDADE && this->voltas < Carro::MAX_VOLTAS){
		this_thread::sleep_for(chrono::seconds(5));
	}
}


void Carro::daUmaVolta() {

	Carro::voltaAcabou = false;

	this_thread::sleep_for(chrono::seconds(Carro::TEMPO_VOLTA));

	Carro::voltaAcabou = true;
}

void Carro::esperaEsvaziar() {

	while(Carro::numPassageiros > 0){
		;
	}
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
		// Verifica se alguém está travando a variável
		while(Carro::lock.test_and_set()){;}

		cout << "CARRO: Esperando a volta " << Carro::voltas << " ser completada" << endl;

		Carro::lock.clear();
		

		daUmaVolta();
		Carro::voltas++;

		while(Carro::lock.test_and_set()){;}

		cout << "CARRO: Esperando o carro ser esvaziado" << endl;

		Carro::lock.clear();

		esperaEsvaziar();
		Carro::voltaAcabou = false;

	}

	cout << "CARRO: Atividades encerradas." << endl;

}
