#include "Stack.h"
#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>
#include <sys/mman.h>
#include <fcntl.h>

using namespace std;
using namespace pr;

#define N 10
#define M 7

void producteur (Stack<char> * stack) {
	char c ;
	while (cin.get(c)) {
		stack->push(c);
	}
}

void consomateur (Stack<char> * stack) {
	while (true) {
		char c = stack->pop();
		cout << c << flush ;
	}
}

void handler1(int sig)
{
	if(sig == SIGINT){}
}

void handler2(int sig)
{
	if(sig == SIGINT)
	{
		std::exit(0);
	}
}

int main () {
	signal(SIGINT, handler1);
	size_t shmsize = sizeof(Stack<char>);
	int fd = shm_open(
			"/myshm",
			O_CREAT | O_EXCL | O_RDWR,
			0666);
	if(fd < 1)
	{
		perror("shm open failed");
		std::exit(1);
	}
	if(ftruncate(fd, shmsize) != 0)
	{
		perror("ftruncate failed");
		std::exit(1);
	}
	void* addr = mmap(
			nullptr,
			shmsize,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			fd, 0);
	if(addr == MAP_FAILED)
	{
		perror("map failed");
		std::exit(1);
	}
	Stack<char>* s = new(addr)Stack<char>();

	for(int prod = 0; prod < N; ++prod)
	{
		pid_t pp = fork();
		if (pp==0) {
			signal(SIGINT, handler2);
			producteur(s);
			return 0;
		}
	}

	for(int cons = 0; cons < M; ++cons)
	{
		pid_t pc = fork();
		if (pc==0) {
			signal(SIGINT, handler2);
			consomateur(s);
			return 0;
		}
	}

	for(int i = 0; i < (N+M); ++i)
	{
		wait(0);
	}

	close(fd);
	if(munmap(addr, shmsize) != 0)
	{
		perror("munmap failed");
		std::exit(1);
	}
	if(shm_unlink("/myshm") != 0)
	{
		perror("unlink error");
	}
	return 0;
}

