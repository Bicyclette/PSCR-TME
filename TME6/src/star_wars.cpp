#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <csignal>
#include "rsleep.h"

int lifebar = 3;

void lightsaber(int sig)
{
	if(sig == SIGINT)
	{
		--lifebar;
		pid_t my_pid = getpid();
		std::cout << my_pid << " HEALTH POINTS = " << lifebar << std::endl;
		if(lifebar == 0)
		{
			std::cout << my_pid << " is now dead." << std::endl;
			std::exit(1);
		}
	}
}

void attaque(pid_t adversaire)
{
	// if attacked by other process with SIGINT
	// then decrement lifebar
	std::signal(SIGINT, lightsaber);

	int status;
	waitpid(adversaire, &status, WNOHANG);
	if(!WIFEXITED(status))
	{
		int ret = kill(adversaire, SIGINT);
		if(ret < 0)
		{
			std::exit(0);
		}
	}
	else
	{
		std::exit(0);
	}
	// sleep for random duration
	randsleep();
}

void defense()
{
	std::signal(SIGINT, SIG_IGN);
	// sleep for random duration
	randsleep();
}

void combat(pid_t adversaire)
{
	while(true)
	{
		defense();
		attaque(adversaire);
	}
}

int main ()
{
	pid_t vader = getpid();
	std::cout << "Vader PID = " << vader << std::endl;
	pid_t luke = fork();
	if(luke == 0)
	{
		srand(getpid());
		combat(vader);
	}
	else
	{
		srand(getpid());
		combat(luke);
	}
	return 0;
}
