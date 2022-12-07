#pragma once

#include <cstring> // size_t,memset
#include <semaphore.h>
#include <iostream>

namespace pr {

#define STACKSIZE 100

template<typename T>
class Stack {
	T tab [STACKSIZE];
	size_t sz;
	sem_t smutex;
	sem_t sempop;
	sem_t sempush;
public :
	Stack () : sz(0)
	{
		sem_init(&smutex, 1, 1);
		sem_init(&sempop, 1, 0);
		sem_init(&sempush, 1, STACKSIZE);
		memset(tab,0,sizeof tab);
	}

	~Stack()
	{
		sem_destroy(&smutex);
		sem_destroy(&sempop);
		sem_destroy(&sempush);
	}

	T pop () {
		// bloquer si vide
		sem_wait(&sempop);
		sem_wait(&smutex);
		T toret = tab[--sz];
		sem_post(&smutex);
		sem_post(&sempush);
		return toret;
	}

	void push(T elt) {
		//bloquer si plein
		sem_wait(&sempush);
		sem_wait(&smutex);
		tab[sz++] = elt;
		sem_post(&smutex);
		sem_post(&sempop);
	}
};

}
