#ifndef SRC_SERVERSOCKET_H_
#define SRC_SERVERSOCKET_H_

#include "Socket.h"

namespace pr {

class ServerSocket {
	int socketfd;

public :
	// Demarre l'ecoute sur le port donne
	ServerSocket(int port)
	{
		int fd = socket(AF_INET, SOCK_STREAM, 0);
		if(fd < 0)
		{
			perror("socket error");
			return;
		}
		struct sockaddr_in sin;
		memset(&sin, 0, sizeof(sin));
		sin.sin_family = AF_INET;
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
		sin.sin_port = htons(port);

		if(bind(fd, (struct sockaddr*)&sin, sizeof(sin)) < 0)
		{
			perror("bind error");
			::close(fd);
			return;
		}

		if(listen(fd, 50) < 0)
		{
			perror("listen error");
			::close(fd);
			return;
		}
		socketfd = fd;
	}

	int getFD() { return socketfd;}
	bool isOpen() const {return socketfd != -1;}

	Socket accept()
	{
		struct sockaddr_in exp;
		socklen_t len = sizeof(exp);
		int scom = ::accept(socketfd, (struct sockaddr*)&exp, &len);
		if(scom < 0)
		{
			perror("accept error");
		}
		else
		{
			std::cout << "accept ok" << std::endl;
		}
		return scom;
	}

	void close()
	{
		if(socketfd != -1)
		{
			::close(socketfd);
		}
	}
};

} // ns pr
#endif /* SRC_SERVERSOCKET_H_ */
