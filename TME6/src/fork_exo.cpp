#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <vector>

int main () {
	const int N = 3;
	pid_t son;
	std::cout << "main pid=" << getpid() << std::endl;

	int i = 1;
	for (int j=N; i<=N && j==N && (son=fork()) == 0; i++ )
	{
		std::cout << " i:j " << i << ":" << j << std::endl;
		for (int k=1; k<=i && j==N ; k++)
		{
			if ( fork() == 0)
			{
				j=0;
				std::cout << " k:j " << k << ":" << j << std::endl;
				return 0;
			}
		}
		for (int k=1; k<=i && j==N ; k++)
		{
			pid_t pid = wait(nullptr);
		}
	}

	if(i < N+1)
	{
		waitpid(son, nullptr, 0);
	}
	return 0;
}
