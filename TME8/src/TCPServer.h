#ifndef SRC_TCPSERVER_H_
#define SRC_TCPSERVER_H_

#include <thread>
#include "ServerSocket.h"
#include "ConnectionHandler.h"

namespace pr {

// un serveur TCP, la gestion des connections est déléguée
class TCPServer {
	ServerSocket * ss; // la socket d'attente si elle est instanciee
	ConnectionHandler * handler; // le gestionnaire de session passe a la constru
	// a completer
	std::thread* waitT;
	int killpipe;
public :
	TCPServer(ConnectionHandler * handler):
		ss(nullptr),
		handler(handler),
		waitT(nullptr),
		killpipe(-1)
	{}
	
	// Tente de creer une socket d'attente sur le port donné
	bool startServer (int port)
	{
		if(waitT == nullptr)
		{
			ss = new ServerSocket(port);
			if(ss->isOpen())
			{
				int pipefd[2];
				if(pipe(pipefd) < 0)
				{
					perror("pipe error");
					return false;
				}

				waitT = new std::thread([](TCPServer* serv, int readpipe){
							while(1)
							{
								fd_set read;
								FD_ZERO(&read);
								FD_SET(readpipe, &read);
								FD_SET(serv->ss->getFD(), &read);
								
								select(std::max(readpipe, serv->ss->getFD())+1, &read, nullptr, nullptr, nullptr);
								if(FD_ISSET(readpipe, &read))
								{
									std::cout << "Demande de quitter." << std::endl;
									return false;
								}
								std::cout << "En attente..." << std::endl;
								auto sc = serv->ss->accept();
								if(sc.isOpen())
								{
									auto copy = serv->handler->clone();
									copy->handleConnection(sc);
									delete copy;
								}
							}
						}, this, pipefd[0]);
				killpipe = pipefd[1];
				return true;
			}
		}
		return false;
	}

	// stoppe le serveur
	void stopServer()
	{
		if(waitT != nullptr)
		{
			int n = 1;
			write(killpipe, &n, sizeof(int));
			waitT->join();
			delete waitT;
			delete ss;
			close(killpipe);
			killpipe = -1;
			waitT = nullptr;
		}
	}
};

} // ns pr

#endif /* SRC_TCPSERVER_H_ */
