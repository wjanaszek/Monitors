#include <stdio.h>
#include <iostream>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "defines.h"
#include "MyMonitor.h"
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "monitor.h"

using namespace std;

int X, Y, Z;
int buffer_size;
MyMonitor * monitor;

void * proc_A1(void * ptr){
	int item = 0;
	while(1){
		//sleep(1);
		monitor->proc_A1(item);
		item += 2;
	}
}

void * proc_A2(void * ptr){
	int item = 0;
	while(1){
		//sleep(1);
		monitor->proc_A2();
	}
}

void * proc_A3(void * ptr){
	int item = 0;
	while(1){
		//sleep(1);
		monitor->proc_A3();
	}
}

int main(int argc, char ** argv){
	int seg_size, n;
	int A1, A2, A3;

	pthread_t thread_A1, thread_A2, thread_A3;

	if(argc < 6){
		cout << "Zbyt malo argumentow!\n";
		return 1;
	}
	else if(argc == 6){
		buffer_size = atoi(argv[1]);
		X = atoi(argv[2]);
		Y = atoi(argv[3]);
		Z = atoi(argv[4]);
		n = atoi(argv[5]);
		seg_size = (buffer_size + 1)*sizeof(int);
	}
	else	{
		exit(1);
	}

	monitor = new MyMonitor(X, Y, Z, seg_size);
	setbuf(stdout, NULL);
	for(int i = 0; i < n; i++){
		if((A1 = pthread_create(&thread_A1, NULL, proc_A1, NULL)) != 0){
			cout << "Blad przy tworzeniu watku A1!" << endl;
			break;
		}
		if((A2 = pthread_create(&thread_A2, NULL, proc_A2, NULL)) != 0){
			cout << "Blad przy tworzeniu watku A2!" << endl;
			break;
		}
		if((A3 = pthread_create(&thread_A3, NULL, proc_A3, NULL)) != 0){
			cout << "Blad przy tworzeniu watku A3!" << endl;
			break;
		}
	}
	pthread_join(thread_A1, NULL);
	pthread_join(thread_A2, NULL);
	pthread_join(thread_A3, NULL);
	pthread_exit(NULL);
	delete monitor;
	return 0;
}

//Kompilacja:
//g++ -o p main.cc -lpthread -lrt
