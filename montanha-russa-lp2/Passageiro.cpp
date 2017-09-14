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
bool Passageiro::entrada[10] = {0};

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

	// Sinaliza a intencao de pegar uma ficha
	Passageiro::entrada[id] = true;

	// Pegar a maior ficha disponivel
	for(auto &pass : parque->getPassageiros()){
		if(pass->ticket[id] > max){
			max = pass->ticket[id];
		}
	}
	atomic_fetch_add(&Passageiro::ticket[id], 1);

	// Sinaliza que não tem mais intencao de pegar ficha
	Passageiro::entrada[id] = false;

	// Fins de depuração
/*
	thread::id this_id = this_thread::get_id();
	cout << "A thread " << this_id << " pegou o ticket: " << Passageiro::ticket[id] << endl;
	
	this_thread::sleep_for(chrono::seconds((rand()%10) + 1));
*/
	for(auto &pass : parque->getPassageiros()){
		if(Passageiro::id != pass->id){
			while((Passageiro::ticket[id] != 0 && (Passageiro::ticket[id] > pass->ticket[id] || (ticket[id] == pass->ticket[id] && Passageiro::id > pass->id)))
				|| Carro::numPassageiros >= Carro::CAPACIDADE || Carro::voltaAcabou){
				;
			}
		}
	}
	/*
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
*/
		// Incrementa o numero de passageiros no carro
		atomic_fetch_add(&Carro::numPassageiros, 1);
/*
		if(Carro::numPassageiros < Carro::CAPACIDADE) {

			cout << "Ticket " << Passageiro::ticket[id] << " foi liberado" << endl;

			// Devolver a ficha
			Passageiro::ticket[id] = 0;
		}
*/
		Passageiro::ticket[id] = 0;
		cout << "O carro possui " << Carro::numPassageiros << " passageiros atualmente." << endl;
}

void Passageiro::esperaVoltaAcabar(){
	while(!Carro::voltaAcabou){
		;
	}
}

void Passageiro::saiDoCarro() {
/*
	while(!Carro::voltaAcabou) {
		this_thread::sleep_for(chrono::seconds(5));
	}
*/
	// Fins de depuração
	thread::id this_id = this_thread::get_id();
	cout << this_id << " saindo do carro" << endl;

	// Decrementa o numero de passageiros no carro
	atomic_fetch_add(&Carro::numPassageiros, -1);
/*

	while(Carro::numPassageiros > 0) {
		this_thread::sleep_for(chrono::seconds(5));
	}

	Carro::voltaAcabou = 0;

	// Protocolo de saida do Algoritmo da Padaria

	// Devolver a ficha
	Passageiro::ticket[id] = 0;
*/
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
/*
	// Fins de depuração
	cout << "Existem " << Parque::numPessoas << " pessoas no parque." << endl;
*/
	while(Carro::lock.test_and_set()){
		;
	}
	cout << "Quantidade de voltas da Thread " <<this->id << " : " << qtdVoltas << endl;
	Carro::lock.clear();
}

