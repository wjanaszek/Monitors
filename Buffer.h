#ifndef _Buffer_h
#define _Buffer_h

#include <stdio.h>
#include <sys/types.h> 
#include <sys/stat.h> 
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>

/* ================================================================================= */
/* 						Klasa reprezentujaca bufor  		  	    				 */
/* ================================================================================= */
class Buffer {
private:
	int segment_id;
	int size;
	int * buffer;

public:
	/* Konstruktor */
	Buffer(int mem_key, int size) {
		if((segment_id = shmget(mem_key, (size + 1)*sizeof(int), IPC_CREAT | 0666)) < 0){
			throw "Failed to create memory segment!";
		}
		buffer = (int *) shmat(segment_id, 0, 0);
		buffer[0] = 0;
		size = buffer[0];
	}

	~Buffer(){
		shmdt(buffer);
		shmctl(segment_id, IPC_RMID, 0);
	}

	/* Zwroc rozmiar */
	int getSize(){
		return buffer[0];
	}

	/* Wstaw element o wartosci value */
	void insert(int value){
		buffer[buffer[0] + 1] = value;
		++buffer[0];
		size = buffer[0];
	}

	/* Zdejmij pierwszy element z kolejki */
	int pop(){
		int item;
		item = buffer[1];
		for(int i = 1; i < buffer[0] + 1; i++){
			buffer[i] = buffer[i + 1];
		}
		--buffer[0];
		return item;
	}

	void toString(){
		setbuf(stdout, NULL);
		for(int i = 1; i < buffer[0] + 1; i++){
			printf("%d, ", buffer[i]);
		}
		printf("\n");
	}
};

#endif
