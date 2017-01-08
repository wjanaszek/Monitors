#ifndef _MyMonitor_h
#define _MyMonitor_h

#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include "monitor.h"
#include "binary_sem.h"
#include "defines.h"
#include <semaphore.h>

#include "Buffer.h"

/* ================================================================================= */
/* 						Klasa reprezentujaca monitor 								 */
/* ================================================================================= */
class MyMonitor : public Monitor {
	/* Properties: */
	int X, Y, Z;			// wartosci X, Y, Z
	Condition full, fullB2, empty;
	Condition proc_A1_condition, 
	Condition proc_A2_condition;		// zmienne warunkowe
	Condition proc_A3_condition;
	Buffer * B1;			// bufor B1
	Buffer * B2;			// bufor B2
	int max_size;				// maksymalny rozmiar bufora

public:
	/* Konstruktor
	* @param max_size - maksymalny rozmiar bufora
	*/
	MyMonitor(int X, int Y, int Z, int max_size){
		
		this->X = X;
		this->Y = Y;
		this->Z = Z;
		this->max_size = max_size;

		// wspolna pamiec (bufory):
		B1 = new Buffer(B1key, max_size);
		B2 = new Buffer(B2key, max_size);
	} 

	~MyMonitor(){
		delete B1;
		delete B2;
	}

	void proc_A1(int value){
		enter();
		
		if(B1->getSize() >= X){
			wait(proc_A1_condition);
		}
		B1->insert(value);
		this->toString();
		if(B1->getSize() < X){
			signal(proc_A1_condition);
		}
		if(B1->getSize() > 0){
			signal(empty);
		}
		if(B2->getSize() < Y){
			signal(proc_A2_condition);
		}
		if(B2->getSize() > Z){
			signal(proc_A3_condition);
		}

		leave();
	}

	void proc_A2(){
		int item = -1;
		enter();

		if(B1->getSize() == 0){
			wait(empty);
		}
		if(B2->getSize() >= Y){
			wait(proc_A2_condition);
		}
		item = B1->pop();
		B2->insert(item + 1);
		this->toString();
		if(B1->getSize() > 0){
			signal(empty);
		}
		if(B2->getSize() < Y){
			signal(proc_A2_condition);
		}
		if(B1->getSize() < X){
			signal(proc_A1_condition);
		}
		if(B2->getSize() > Z){
			signal(proc_A3_condition);
		}

		leave();
	}

	void proc_A3(){
		int item = -1;
		enter();

		if(B2->getSize() <= Z){
			wait(proc_A3_condition);
		}
		item = B2->pop();
		if(item % 3 == 0){
			B1->insert(item);
		}
		this->toString();
		if(B2->getSize() > Z){
			signal(proc_A3_condition);
		}
		if(B1->getSize() < X){
			signal(proc_A1_condition);
		}
		if(B1->getSize() > 0){
			signal(empty);
		}
		if(B2->getSize() < Y){
			signal(proc_A2_condition);
		}

		leave();
	}

	void toString(){
		setbuf(stdout, NULL);
		printf("--> B1 size = %d\n", B1->getSize());
		B1->toString();
		printf("--> B2 size = %d\n", B2->getSize());
		B2->toString();
	}
};

#endif