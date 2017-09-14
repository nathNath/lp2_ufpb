#include <iostream>
#include <vector>
#include <thread>
#include "include/Parque.h"
#include "include/Carro.h"
#include "include/Passageiro.h"

using namespace std;

int main() {

	Parque parque;
	Carro carro;
	Passageiro *passageiros[10];

    for (int i = 0; i < 10; i++) {
    	passageiros[i] = new Passageiro(i, &carro, &parque);
    	parque.addPassageiro(passageiros[i]);
    }

    thread tCarro(&Carro::run, carro);

    vector<thread> tPassageiros;
    for (auto &p : parque.getPassageiros()){
        tPassageiros.push_back(thread(&Passageiro::run, p));  
    } 

    tCarro.join();

    for (auto &th : tPassageiros){
        th.join();  
    } 

    for (int i = 0; i < 10; i++) {
    	delete passageiros[i];
    }

    while(Carro::lock.test_and_set()){;}

    cout << "O parque encerrou suas atividades." << endl;

    for (auto &p : parque.getPassageiros()){

    	cout << "O passageiro " << p->getId() << " deu " << p->qtdVoltas << " voltas." <<endl;
    }
    Carro::lock.clear();

	return 0;
}
