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

#define MAX_NUM_VOLTAS 2
#define NUM_PASSAGEIROS 10

using namespace std;

int Passageiro::ticket[10] = {0};
bool Passageiro::entrada[10] = {0};

atomic<int> numPassageiros = ATOMIC_VAR_INIT(Carro::numPassageiros);
atomic<int> numPessoas = ATOMIC_VAR_INIT(Parque::numPessoas);
atomic<int> next_id = ATOMIC_VAR_INIT(0);

Passageiro::Passageiro(int id, Carro *c) {
	this->id = id;
	this->carro = c;
}

Passageiro::~Passageiro() {
}

void Passageiro::entraNoCarro() {

	// Protocolo de entrada o Algoritmo da Padaria

	this_thread::sleep_for(chrono::seconds((rand()%10) + 1));

	// Pegar uma ficha
	Passageiro::entrada[id] = true;

	// Pegar a maior ficha disponivel
	atomic_fetch_add(&next_id, 1);
	Passageiro::ticket[id] = next_id;

	// Não quer mais pegar ficha
	Passageiro::entrada[id] = false;

	// Fins de depuração
	thread::id this_id = this_thread::get_id();
	cout << "A thread " << this_id << " pegou o ticket: " << Passageiro::ticket[id] << endl;

	this_thread::sleep_for(chrono::seconds((rand()%10) + 1));

	for(int j = 0; j < NUM_PASSAGEIROS; j++){
		if(j == id) continue;

		while(Passageiro::entrada[id]){
			this_thread::yield();
		}

		while(Passageiro::ticket[j] != 0 && (Passageiro::ticket[id] > Passageiro::ticket[j] || (Passageiro::ticket[id] == Passageiro::ticket[j] && id > j))){
			this_thread::yield();
		}
	}

	if(parqueFechado()){

		// Fins de depuração
		cout << this_id << " foi retirado do parque pois o mesmo está fechado." << endl;

		// Devolver a ficha
		Passageiro::ticket[id] = 0;

		// Decrementa o numero de pessoas no parque
		atomic_fetch_sub(&numPessoas, 1);
		Parque::numPessoas = numPessoas;

	} else{

		// Fins de depuração
		cout << this_id << " entrou no carro!" << endl;

		this_thread::sleep_for(chrono::seconds((rand()%10) + 1));

		// Incrementa o numero de passageiros no carro
		atomic_fetch_add(&numPassageiros, 1);
		Carro::numPassageiros = numPassageiros;

		if(Carro::numPassageiros < Carro::CAPACIDADE) {

			cout << "Ticket " << Passageiro::ticket[id] << " foi liberado" << endl;

			// Devolver a ficha
			Passageiro::ticket[id] = 0;
		}

		cout << "O carro possui " << Carro::numPassageiros << " passageiros atualmente." << endl;
	}
}

void Passageiro::saiDoCarro() {

	while(!Carro::voltaAcabou) {
		this_thread::sleep_for(chrono::seconds(5));
	}

	// Fins de depuração
	thread::id this_id = this_thread::get_id();
	cout << this_id << " saindo do carro" << endl;

	// Decrementa o numero de passageiros no carro
	atomic_fetch_add(&numPassageiros, -1);
	Carro::numPassageiros = numPassageiros;

	this_thread::sleep_for(chrono::seconds((rand()%10) + 1));

	while(Carro::numPassageiros > 0) {
		this_thread::sleep_for(chrono::seconds(5));
	}

	Carro::voltaAcabou = 0;

	// Protocolo de saida do Algoritmo da Padaria

	// Devolver a ficha
	Passageiro::ticket[id] = 0;

}

void Passageiro::passeiaPeloParque() {

	// Fins de depuração
	thread::id this_id = this_thread::get_id();
	cout << this_id << " passeia pelo parque aleatoriamente." << endl;

	this_thread::sleep_for(chrono::seconds((rand()%10) + 1));
}

bool Passageiro::parqueFechado() {
	return Carro::voltas >= MAX_NUM_VOLTAS;
}

void Passageiro::run() {
	while (!parqueFechado()) {

		entraNoCarro(); // Protocolo de Entrada

		saiDoCarro(); // Protocolo de Saída

		passeiaPeloParque(); // Seção Não Crítica
	}

	// Decrementa o numero de pessoas no parque
	atomic_fetch_add(&numPessoas, -1);
	Parque::numPessoas = numPessoas;

	// Fins de depuração
	cout << "Existem " << Parque::numPessoas << " pessoas no parque." << endl;
}

