#ifndef SRC_SOCKET_H_
#define SRC_SOCKET_H_

#include <netinet/ip.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <iosfwd>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <cstring>
#include <unistd.h>

namespace pr {

class Socket {
	int fd;

public :
	Socket():fd(-1){}
	Socket(int fd):fd(fd){}

	// tente de se connecter à l'hôte fourni
	void connect(const std::string & host, int port)
	{
		struct addrinfo * addr;
		if(getaddrinfo(host.c_str(), nullptr, nullptr, &addr) < 0)
		{
			perror("addrinfo failed");
			return;
		}
		in_addr ipv4 = ((struct sockaddr_in *)addr->ai_addr)->sin_addr;
		freeaddrinfo(addr);
		connect(ipv4, port);
	}
	void connect(in_addr ipv4, int port)
	{
		struct sockaddr_in dest;
		dest.sin_family = AF_INET;
		dest.sin_addr = ipv4;
		dest.sin_port = htons(port);

		fd = socket(AF_INET, SOCK_STREAM, 0);
		if(fd < 0)
		{
			perror("socket error");
			return;
		}

		if(::connect(fd, (struct sockaddr*)&dest, sizeof(dest)) < 0)
		{
			perror("connect error");
			::close(fd);
			return;
		}
	}

	bool isOpen() const {return fd != -1;}
	int getFD() { return fd ;}

	void close()
	{
		if(fd != -1)
		{
			shutdown(fd, SHUT_WR);
			::close(fd);
		}
	}
};

inline std::ostream& operator<<(std::ostream& os, struct sockaddr_in * addr)
{
	char hname[1024];
	if(getnameinfo((struct sockaddr*)addr, sizeof(*addr), hname, 1024, nullptr, 0, 0) == 0)
	{
		os << "\"" << hname << "\"" << " ";
	}
	os << inet_ntoa(addr->sin_addr) << ":" << ntohs(addr->sin_port) << std::endl;
	return os;
}

}

#endif /* SRC_SOCKET_H_ */
