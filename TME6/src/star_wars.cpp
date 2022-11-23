#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <csignal>

int lifebar = 3;

void lightsaber(int sig)
{
	--lifebar;
}

int luke_skywalker(pid_t vador_pid)
{
	std::cout << "LUKE SKYWALKER ENTERS IN THE ARENA WITH " << lifebar;
	std::cout << " HEALTH POINTS." << std::endl;
	std::signal(SIGINT, lightsaber);

	while(lifebar != 0)
	{
		if(kill(vador_pid, SIGINT) == -1)
		{
			std::cout << "Vador is dead !" << std::endl;
			std::cout << "Luke won the duel !" << std::endl;
			return 0;
		}
	}
	return 1;
}

int vador_skywalker(pid_t luke_pid)
{
	std::cout << "VADER SKYWALKER ENTERS IN THE ARENA WITH " << lifebar;
	std::cout << " HEALTH POINTS." << std::endl;
	std::signal(SIGINT, lightsaber);
	
	while(lifebar != 0)
	{
		if(waitpid(luke_pid, nullptr, WNOHANG) != luke_pid)
		{
			if(kill(luke_pid, SIGINT) == -1)
			{
				std::cout << "Luke is dead !" << std::endl;
				std::cout << "Vador won the duel !" << std::endl;
				return 0;
			}
		}
	}
	return 1;
}

int main ()
{
	pid_t vador_pid = getpid();
	std::signal(SIGINT, SIG_IGN);

	pid_t luke_pid = fork();
	if(luke_pid == 0)
	{
		int ret = luke_skywalker(vador_pid);
		std::cout << "Luke returned " << ret << std::endl;
		return ret;
	}
	else
	{
		int ret = vador_skywalker(luke_pid);
		std::cout << "Vader returned " << ret << std::endl;
		if(ret == 1){
			wait(nullptr);
		}
		return ret;
	}
}
