#pragma once

#include <WinSock2.h>
#include <stdio.h>

#pragma comment(lib, "ws2_32.lib")

#define SOCKET_SUCCESS			 0
#define SOCKET_ERR_WSASTARTUP	-1
#define SOCKET_ERR_SOCKET		-2
#define SOCKET_ERR_CONNECT		-3
#define SOCKET_ERR_SEND			-4
#define SOCKET_ERR_RECV			-5

class CClientSocket
{
public:
	CClientSocket()
	{

	}

	virtual ~CClientSocket()
	{
		close();
		//WSACleanup();
	}

	int connect(int port, const char* address)
	{
		int err;
		WSADATA wsaData;

		err = WSAStartup(MAKEWORD(1, 1), &wsaData);
		if (err != NO_ERROR)
		{
			printf("WSAStartup() failed as : %d\n", err);

			return SOCKET_ERR_WSASTARTUP;
		}

		m_socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		if (m_socket == INVALID_SOCKET)
		{
			printf("::socket() faield as : %d\n", WSAGetLastError());
			return SOCKET_ERR_SOCKET;
		}

		sockaddr_in sockaddrServer;
		sockaddrServer.sin_family = AF_INET;
		sockaddrServer.sin_port = port;
		sockaddrServer.sin_addr.s_addr = inet_addr(address);

		err = ::connect(m_socket, (sockaddr*)&sockaddrServer, sizeof(sockaddrServer));

		if (err < 0)
		{
			printf("::connect() failed as : %d\n", err);
			return SOCKET_ERR_CONNECT;
		}

		return SOCKET_SUCCESS;
	}

	int connect(int port, SOCKADDR_IN address)
	{
		return connect(port, inet_ntoa(address.sin_addr));
	}

	int send(const char* buffer, size_t size)
	{
		int err = ::send(m_socket, buffer, size, 0);

		if (err < 0)
		{
			printf("::send() failed as : %d\n", err);

			return SOCKET_ERR_SEND;
		}

		return SOCKET_SUCCESS;
	}

	int recv(char* buffer, size_t size)
	{
		int err = ::recv(m_socket, buffer, size, 0);

		if (err < 0)
		{
			printf("::recv() failed as : %d\n", err);

			return SOCKET_ERR_RECV;
		}

		return SOCKET_SUCCESS;
	}

	void close()
	{
		::closesocket(m_socket);
	}

protected:
	CClientSocket(const CClientSocket& rhs);
	const CClientSocket& operator=(const CClientSocket& rhs);

private:
	SOCKET		m_socket;
};
