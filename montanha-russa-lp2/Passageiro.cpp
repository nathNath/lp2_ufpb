/*
 * Passageiro.cpp
 *
 *  Created on: 17 de abr de 2017
 *      Author: bruno
 */

#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <atomic>

#include "include/Thread.h"
#include "include/Passageiro.h"
#include "include/Carro.h"
#include "include/Parque.h"

#define NUM_PASSAGEIROS 10

using namespace std;

atomic<int> Passageiro::ticket[10];

Passageiro::Passageiro(int id, Carro *c, Parque *p) {
	this->id = id;
	this->carro = c;
	this->parque = p;
}

Passageiro::~Passageiro() {
}

void Passageiro::entraNoCarro() {

	// Protocolo de entrada o Algoritmo da Padaria

	int max = 0;

	// Pegar a maior ficha disponivel
	for(auto &pass : parque->getPassageiros()){
		if(pass->ticket[id] > max){
			max = pass->ticket[id];
		}
	}
	atomic_fetch_add(&Passageiro::ticket[id], 1);

	thread::id this_id = this_thread::get_id();
	cout << "A thread " << this_id << " pegou o ticket: " << Passageiro::ticket[id] << endl;

	for(auto &pass : parque->getPassageiros()){

		if(Passageiro::id != pass->id){
			while((pass->ticket[id] != 0 && (Passageiro::ticket[id] > pass->ticket[id] || (ticket[id] == pass->ticket[id] && Passageiro::id > pass->id)))
				|| Carro::numPassageiros >= Carro::CAPACIDADE || Carro::voltaAcabou){
				;
			}
		}
	}

		// Incrementa o numero de passageiros no carro
		atomic_fetch_add(&Carro::numPassageiros, 1);

		Passageiro::ticket[id] = 0;
		cout << "O carro possui " << Carro::numPassageiros << " passageiros atualmente." << endl;
}

void Passageiro::esperaVoltaAcabar(){
	while(!Carro::voltaAcabou){
		;
	}
}

void Passageiro::saiDoCarro() {

	// Fins de depuração
	thread::id this_id = this_thread::get_id();
	cout << this_id << " saindo do carro" << endl;

	// Decrementa o numero de passageiros no carro
	atomic_fetch_add(&Carro::numPassageiros, -1);

}

void Passageiro::passeiaPeloParque() {

	// Fins de depuração
	thread::id this_id = this_thread::get_id();
	cout << this_id << " passeia pelo parque aleatoriamente." << endl;

	while (Carro::lock.test_and_set()) {
		;
	}
	if(carro->getNVoltas() < 5){
		this_thread::sleep_for(chrono::seconds((rand()%10) + 1));
	}
	Carro::lock.clear();
}

bool Passageiro::parqueFechado() {
	return carro->getNVoltas() >= Carro::MAX_VOLTAS;
}

void Passageiro::run() {

	int qtdVoltas = 0;

	while (!parqueFechado()) {

		entraNoCarro(); // Protocolo de Entrada

		esperaVoltaAcabar();

		saiDoCarro(); // Protocolo de Saída
		qtdVoltas++;
		passeiaPeloParque(); // Seção Não Crítica
	}

	// Decrementa o numero de pessoas no parque
	atomic_fetch_add(&Parque::numPessoas, -1);

	while(Carro::lock.test_and_set()){
		;
	}
	cout << "Quantidade de voltas da Thread " << this->id << " : " << qtdVoltas << endl;
	Carro::lock.clear();
}

