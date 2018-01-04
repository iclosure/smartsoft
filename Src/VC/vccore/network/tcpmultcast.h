
#ifndef _HR_TCP_MULTICAST_H_
#define _HR_TCP_MULTICAST_H_


#include <WinSock2.h>
#include <stdio.h>
#include <assert.h>

#pragma comment(lib, "ws2_32.lib")

#define SOCKET_SUCCESS			 0
#define SOCKET_ERR_WSASTARTUP	-1
#define SOCKET_ERR_SOCKET		-2
#define SOCKET_ERR_CONNECT		-3
#define SOCKET_ERR_LIST			-4
#define SOCKET_ERR_SEND			-5
#define SOCKET_ERR_RECV			-6
#define SOCKET_ERR_DISCONNECTED	-7

class CTcpSocket
{
public:
	CTcpSocket()
	{

	}

	virtual ~CTcpSocket()
	{
		disconnect();
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

		sockaddr_in remote_addr;
		remote_addr.sin_family = AF_INET;
		remote_addr.sin_port = htons(port);
		remote_addr.sin_addr.s_addr = inet_addr(address);

		err = ::connect(m_socket, (PSOCKADDR)&remote_addr, sizeof(SOCKADDR));

		if (err < 0)
		{
			printf("::connect() failed as : %d\n", err);
			return SOCKET_ERR_CONNECT;
		}

		m_isConnected = true;

		return SOCKET_SUCCESS;
	}

	int connect(int port, SOCKADDR_IN address)
	{
		return connect(port, inet_ntoa(address.sin_addr));
	}

	int listen(int backlog)
	{
		int err = ::listen(m_socket, backlog);

		if (err < 0)
		{
#if defined(_DEBUG)
			printf("::listen() failed as : %d\n", err);
#endif  // _DEBUG

			return SOCKET_ERR_LIST;
		}

		return SOCKET_SUCCESS;
	}

	int send(const char* buffer, size_t size)
	{
		if (!m_isConnected)
		{
			assert(false);
			return SOCKET_ERR_DISCONNECTED;
		}

		int err = ::send(m_socket, buffer, size, 0);

		if (err < 0)
		{
			assert(false);
#if defined(_DEBUG)
			printf("::send() failed as : %d\n", err);
#endif  // _DEBUG
			return SOCKET_ERR_SEND;
		}

		return SOCKET_SUCCESS;
	}

	int recv(char* buffer, size_t size)
	{
		if (!m_isConnected)
		{
			assert(false);
			return SOCKET_ERR_DISCONNECTED;
		}

		int err = ::recv(m_socket, buffer, size, 0);

		if (err < 0)
		{
			assert(false);
#if defined(_DEBUG)
			printf("::recv() failed as : %d\n", err);
#endif  // _DEBUG
			return SOCKET_ERR_RECV;
		}

		return SOCKET_SUCCESS;
	}

	void disconnect()
	{
		::closesocket(m_socket);
		WSACleanup();

		m_isConnected = false;
	}

	bool isConnected()
	{
		return m_isConnected;
	}

protected:
	CTcpSocket(const CTcpSocket& rhs);
	const CTcpSocket& operator=(const CTcpSocket& rhs);

private:
	bool m_isConnected;
	SOCKET m_socket;
};

#endif  // _HR_TCP_MULTICAST_H_
