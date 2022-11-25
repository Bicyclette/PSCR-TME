#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <csignal>

int ret_timed_wait = -1;

void handler_alrm(int sig)
{
	if(sig == SIGALRM)
	{
		ret_timed_wait = 0;
	}
}

void handler_end_ps(int sig)
{
	if(sig == SIGCHLD)
	{
		ret_timed_wait = 7;
	}
}

int wait_till_pid(pid_t pid)
{
	pid_t r;
	do {
		r = wait(nullptr);
	} while(r != pid && r != -1);
	return r;
}

int wait_till_pid(pid_t pid, int sec)
{
	int status;
	std::signal(SIGALRM, handler_alrm);
	std::signal(SIGCHLD, handler_end_ps);
	alarm(sec);
	while(ret_timed_wait >= -1)
	{
		if(ret_timed_wait == 0){
			ret_timed_wait = -1;
			return 0;
		}
		if(ret_timed_wait == 7){
			pid_t ret_pid = wait(nullptr);
			if(ret_pid == pid){
				ret_timed_wait = -1;
				return pid;
			}
			ret_timed_wait = -1;
		}
	}
}

int main()
{
	int timeout = 2;
	pid_t pere = getpid();
	pid_t fils = fork();

	if(fils == 0)
	{
		sleep(1);
	}
	else
	{
		int ret = wait_till_pid(fils, timeout);
		if(ret != 0)
		{
			std::cout << ret << " finished its execution." << std::endl;
		}
		else
		{
			std::cout << "Son process did not finished its execution before timeout." << std::endl;
		}
	}
	return 0;
}
