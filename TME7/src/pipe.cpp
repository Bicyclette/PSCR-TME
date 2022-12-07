#include <iostream>
#include <unistd.h>		// pipe, dup
#include <sys/types.h>	// fork
#include <stdio.h>		// perror
#include <string.h>		// strncmp
#include <vector>

int main(int argc, char* argv[])
{
	// get command line arguments
	int pos_pipe = -1;
	char* const* c_argv = (char*const*)argv;
	std::vector<char*> cmd1;
	std::vector<char*> cmd2;

	for(int i = 1; i < argc; ++i)
	{
		if(strcmp(argv[i], "|") == 0)
		{
			pos_pipe = i;
		}
		if(pos_pipe == -1)
		{
			cmd1.push_back(argv[i]);
		}
		else if(i > pos_pipe)
		{
			cmd2.push_back(argv[i]);
		}
	}
	char* const* c_argv_cmd1 = (char*const*)cmd1.data();
	char* const* c_argv_cmd2 = (char*const*)cmd2.data();

	/*
	std::cout << "Command 1 : ";
	for(int i = 0; i < cmd1.size(); ++i)
	{
		std::cout << cmd1[i] << " ";
	}
	
	std::cout << "\nCommand 2 : ";
	for(int i = 0; i < cmd2.size(); ++i)
	{
		std::cout << cmd2[i] << " ";
	}
	std::cout << std::endl;
	*/

	// create pipe
	int tube[2];
	if(pipe(tube) == -1)
	{
		perror("Failed pipe creation.\n");
		std::exit(1);
	}

	// fork
	if(fork() == 0) // child process
	{
		// exécute la deuxième commande
		// donc lecture dans le tube
		if(dup2(tube[0], STDIN_FILENO) == -1)
		{
			perror("dup2 failed in child process.");
			std::exit(1);
		}
		close(tube[0]);
		close(tube[1]);
		if(execv(cmd2[0], c_argv_cmd2) == -1)
		{
			perror("execv failed in child process.");
			std::exit(1);
		}
	}
	else // parent process
	{
		// exécute la première commande
		// donc ecriture dans le tube
		if(dup2(tube[1], STDOUT_FILENO) == -1)
		{
			perror("dup2 failed in parent process.");
			std::exit(1);
		}
		close(tube[0]);
		close(tube[1]);
		if(execv(cmd1[0], c_argv_cmd1) == -1)
		{
			perror("execv failed in parent process.");
			std::exit(1);
		}
	}
	return 0;
}
