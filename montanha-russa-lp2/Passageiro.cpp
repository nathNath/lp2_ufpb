/*
 * Passageiro.cpp
 *
 *  Created on: 17 de abr de 2017
 *      Authors: Bruno, Nathália e Janse
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

using namespace std;


Passageiro::Passageiro(int id, Carro *c, Parque *p) {
	this->id = id;
	this->carro = c;
	this->parque = p;
	this->ticket = 0;
}

Passageiro::~Passageiro() {
}

void Passageiro::entraNoCarro() {

	// Protocolo de entrada do Algoritmo de Lamport da Padaria

	int max = 0;

	// Pegar a maior ficha disponivel
	for(auto &pass : parque->getPassageiros()){
		if(pass->ticket > max){
			max = pass->ticket;
		}
	}
	Passageiro::ticket = max + 1;

	cout << "O passageiro " << this->id << " pegou o ticket: " << Passageiro::ticket << endl;

	for(auto &pass : parque->getPassageiros()){

		if(Passageiro::id != pass->id){
			while((pass->ticket != 0 && (Passageiro::ticket > pass->ticket || (ticket == pass->ticket && Passageiro::id > pass->id)))
				|| Carro::numPassageiros >= Carro::CAPACIDADE || Carro::voltaAcabou){
				;
			}
		}
	}

		// Incrementa o numero de passageiros no carro
		atomic_fetch_add(&Carro::numPassageiros, 1);

		// Protocolo de Saída
		Passageiro::ticket = 0;

		cout << "O carro possui " << Carro::numPassageiros << " passageiros atualmente." << endl;
}

void Passageiro::esperaVoltaAcabar(){
	while(!Carro::voltaAcabou){
		;
	}
}

void Passageiro::saiDoCarro() {

	// Fins de depuração
	cout << "O passageiro " << this->id << " esta saindo do carro" << endl;

	// Decrementa o numero de passageiros no carro
	atomic_fetch_sub(&Carro::numPassageiros, 1);

}

void Passageiro::passeiaPeloParque() {

	// Fins de depuração
	cout << this->id << " passeia pelo parque aleatoriamente." << endl;

	while (Carro::lock.test_and_set()) {
		;
	}
	if(carro->getNVoltas() < Carro::MAX_VOLTAS){
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
	atomic_fetch_sub(&Parque::numPessoas, 1);
	cout << "Num pessoas no parque: " << Parque::numPessoas << endl;

	while(Carro::lock.test_and_set()){
		;
	}

	cout << "Qtde de voltas no CARRO: " << qtdVoltas << endl;

	Carro::lock.clear();
}

