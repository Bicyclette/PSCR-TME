#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <csignal>
#include "rsleep.h"

int lifebar = 3;

void handler(int sig)
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

void luke_handler(int sig)
{
	std::cout << "coup paré !" << std::endl;
}

void attaque(pid_t adversaire)
{
	// if attacked by other process with SIGINT
	// then decrement lifebar
	std::signal(SIGINT, handler);

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
	randsleep();
}

void luke_defense()
{
	// SIGINT handler data for Luke
	struct sigaction signal_action;
	signal_action.sa_handler = luke_handler;
	sigset_t sigset;
	sigfillset(&sigset);
	sigdelset(&sigset, SIGINT);
	signal_action.sa_mask = sigset;
	signal_action.sa_flags = 0;
	sigset_t ctrlc;
	sigemptyset(&ctrlc);
	sigaddset(&ctrlc, SIGINT);
	
	sigaction(SIGINT, &signal_action, nullptr);
	sigprocmask(SIG_BLOCK, &ctrlc, nullptr);
	// sleep for random duration
	randsleep();
	sigsuspend(&sigset);
}

void combat(pid_t adversaire, bool is_luke)
{
	while(true)
	{
		if(is_luke)
		{
			luke_defense();
		}
		else
		{
			defense();
		}
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
		combat(vader, true);
	}
	else
	{
		srand(getpid());
		combat(luke, false);
	}
	return 0;
}
