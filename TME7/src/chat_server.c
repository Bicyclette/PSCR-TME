#include <iostream>
#include <unistd.h>
#include <vector>
#include <sys/mman.h>
#include <fcntl.h>
#include "chat_common.h"

struct shm_server
{
	char* id;
	size_t shmsize;
	int fd;
	struct myshm* addr;
};

struct shm_server server_shared_memory;
std::vector<char*> client_id;
std::vector<struct message*> client_shm;

void handler(int sig)
{
	if(sig == SIGINT)
	{
		close(server_shared_memory.fd);
		if(munmap(server_shared_memory.addr, server_shared_memory.shmsize) != 0)
		{
			perror("Server's munmap failed");
			std::exit(1);
		}
		if(shm_unlink(server_shared_memory.id) != 0)
		{
			perror("Server's shared memory unlink error");
		}
		std::exit(1);
	}
}

void register_client_shm(char* id)
{
	int fd = shm_open(id, O_CREAT | O_EXCL | O_RDWR, 0666);
	if(fd < 1)
	{
		perror("Client's shared memory open failed");
		std::exit(1);
	}
	// attach memory segment to current process
	void* addr = mmap(nullptr, sizeof(struct message), PROT_WRITE, MAP_SHARED, fd, 0);
	if(addr == MAP_FAILED)
	{
		perror("Client map failed");
		std::exit(1);
	}
	struct message* client = new (addr)struct message;
	client_shm.push_back(client);
}

// max capacity is 50 messages in messages field
// int read, int write, int nb, sem_t sem, messages{long type, chat content[10]}
void server(struct myshm* addr)
{
	// init server state
	addr->read = 0;		// num messages forwarded
	addr->write = 0;	// num messages not yet forwarded
	addr->nb = 0;		// num messages emitted by server
	sem_init(&addr->sem, 1, 1);
	//addr->sem = sem_open("/server_sem", O_CREAT | O_EXCL | O_RDWR, 0666, 1);
	while(true)
	{
		sem_wait(&addr->sem);
		if(addr->write > 0)
		{
			size_t num_clients = client_shm.size();
			for(int i = 0; i < MAX_MESS; ++i)
			{
				struct message& msg = addr->messages[i];
				if(msg.type == 1) // connect
				{
					client_id.push_back(msg.content);
					register_client_shm(msg.content);
				}
				else if(msg.type == 2) // send message
				{
					for(int i = 0; i < num_clients; ++i)
					{
						memcpy(client_shm[i]->content, msg.content, TAILLE_MESS);
					}
				}
				else if(msg.type == 3) // disconnect
				{	
					int index = 0;
					for(char* c_id : client_id)
					{
						if(strcmp(c_id, msg.content) == 0)
						{
							client_id.erase(client_id.begin() + index);
							break;
						}
						++index;
					}
				}
			}
		}
		sem_post(&addr->sem);
	}
}

int main(int argc, char* argv[])
{
	if(argc == 2)
	{
		server_shared_memory.id = argv[1];
	}
	else
	{
		std::cerr << "ERROR: NO SERVER IDENTIFIER PROVIDED !" << std::endl;
		std::exit(1);
	}
	

	signal(SIGINT, handler);

	// create server shared memory
	server_shared_memory.shmsize = sizeof(struct myshm);
	server_shared_memory.fd = shm_open(
			server_shared_memory.id,
			O_CREAT | O_EXCL | O_RDWR,
			0666);
	if(server_shared_memory.fd < 1)
	{
		perror("Server's shared memory open failed");
		std::exit(1);
	}
	// allocate enough size for memory segment
	if(ftruncate(server_shared_memory.fd, server_shared_memory.shmsize) != 0)
	{
		perror("Server ftruncate failed");
		std::exit(1);
	}
	// attach memory segment to current process
	void* addr = mmap(
			nullptr,
			server_shared_memory.shmsize,
			PROT_READ | PROT_WRITE,
			MAP_SHARED,
			server_shared_memory.fd, 0);
	if(addr == MAP_FAILED)
	{
		perror("Server map failed");
		std::exit(1);
	}

	struct myshm* server_shm = new (addr)struct myshm;
	server_shared_memory.addr = server_shm;

	// run server
	server(server_shared_memory.addr);

	close(server_shared_memory.fd);
	if(munmap(addr, server_shared_memory.shmsize) != 0)
	{
		perror("Server's munmap failed");
		std::exit(1);
	}
	if(shm_unlink(server_shared_memory.id) != 0)
	{
		perror("Server's shared memory unlink error");
	}
	return 0;
}

