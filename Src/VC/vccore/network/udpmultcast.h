
#ifndef _HR_UDP_MULTICAST_H_
#define _HR_UDP_MULTICAST_H_

#include <string>

#ifdef WIN32
#include <Winsock2.h>
#include <WS2tcpip.h>
#include <windows.h>
#include <PROCESS.h>
#include <malloc.h>

#elif  QNX
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#elif  VXWORKS
#include <vxworks.h>
#include <sockLib.h>
#include <inetLib.h> 
#endif

#if defined(_DEBUG)
#include <stdio.h>
#endif // _DEBUG

class CUdpMultcast
{
public:
	CUdpMultcast()
		: m_localPort(0)
		, m_mcastPort(0)
		, m_socket(-1)
	{
	}

	virtual ~CUdpMultcast()
	{
		//不释放指针，它由查询对象所拥有
		Close();
	}

private:
	//禁止拷贝和复制
	CUdpMultcast(const CUdpMultcast& rhs);
	const CUdpMultcast& operator=(const CUdpMultcast& rhs);

public:
	const int Init(std::string localAddr, std::string mcastAddr, unsigned short localPort, unsigned short mcastPort,
		bool enalbeMcastLoopBack = false, unsigned int msgBufferSize = 204800, bool enableReuse = false)
	{
		if (-1 != m_socket)
		{
			return 1;   //已经打开了，不能算是错误，返回正1
		}

		this->m_localAddr = localAddr;
		this->m_mcastAddr = mcastAddr;
		this->m_localPort = localPort;
		this->m_mcastPort = mcastPort;

		//建立套接字
		m_socket = (int)socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (m_socket < 0)
		{
			return -1;
		}

		//设定是否允许重复打开
		if (enableReuse)
		{
			int isen = 1;
			if (0 > setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, (char*)&isen, sizeof(isen)))
			{
#ifdef WIN32
				closesocket(m_socket);
#elif QNX
				close(m_socket);
#elif  VXWORKS
				close(m_socket);
#endif
				m_socket = -1;
				return -2;
			}
		}

		/*本地地址，由于Vxworks的bug，不能直接使用本地制定地址绑定多播，要先绑定任意地址，然后改为指定地址*/
		m_localif.sin_family = AF_INET;
		m_localif.sin_port = htons(localPort);
		m_localif.sin_addr.s_addr = htonl(INADDR_ANY);
		/*绑定本机地址*/
		if (bind(m_socket, (sockaddr *)&m_localif, sizeof(m_localif)) < 0)
		{
#ifdef WIN32
			closesocket(m_socket);
#elif QNX
			close(m_socket);
#elif  VXWORKS
			close(m_socket);
#endif
			m_socket = -1;
			return -3;
		}

		//加入多播
		m_mreq.imr_multiaddr.s_addr = inet_addr(m_mcastAddr.c_str());
		if (!m_localAddr.empty())
		{
			m_mreq.imr_interface.s_addr = inet_addr(m_localAddr.c_str());
		}
		else
		{
			m_mreq.imr_interface.s_addr = htonl(INADDR_ANY);
		}
		if (setsockopt(m_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&m_mreq, sizeof(m_mreq)) < 0)
		{
#ifdef WIN32
			closesocket(m_socket);
#elif QNX
			close(m_socket);
#elif  VXWORKS
			close(m_socket);
#endif
			m_socket = -1;
			return -4;
		}

		/*设定多播的本地接口地址*/
		UINT32 localaddr;
		if (!m_localAddr.empty()){
			localaddr = inet_addr(m_localAddr.c_str());
		}
		else{
			localaddr = htonl(INADDR_ANY);
		}
		if (setsockopt(m_socket, IPPROTO_IP, IP_MULTICAST_IF, (char *)&localaddr, sizeof(localaddr)) < 0)
		{
			setsockopt(m_socket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&m_mreq, sizeof(m_mreq));
#ifdef WIN32
			closesocket(m_socket);
#elif QNX
			close(m_socket);
#elif  VXWORKS
			close(m_socket);
#endif
			m_socket = -1;
			return -5;
		}

		//组播缓冲区
		int SendBufferLen = (msgBufferSize > 8192) ? msgBufferSize : 8192;
		int len = sizeof(SendBufferLen);
		//getsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, (char *)&SendBufferLen, &len);
		if (0 > setsockopt(m_socket, SOL_SOCKET, SO_SNDBUF, (char *)&SendBufferLen, len))
		{
			setsockopt(m_socket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&m_mreq, sizeof(m_mreq));
#ifdef WIN32
			closesocket(m_socket);
#elif QNX
			close(m_socket);
#elif  VXWORKS
			close(m_socket);
#endif
			m_socket = -1;
			return -6;
		}

		//多播回环
		char loopflag = enalbeMcastLoopBack ? 1 : 0;
		len = sizeof(loopflag);
		if (0 > setsockopt(m_socket, IPPROTO_IP, IP_MULTICAST_LOOP, &loopflag, len))
		{
			setsockopt(m_socket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&m_mreq, sizeof(m_mreq));
#ifdef WIN32
			closesocket(m_socket);
#elif QNX
			close(m_socket);
#elif  VXWORKS
			close(m_socket);
#endif
			m_socket = -1;
			return -7;
		}

		//初始化组播目的地地址
		m_toif.sin_family = AF_INET;
		m_toif.sin_port = htons(mcastPort);
		m_toif.sin_addr.s_addr = inet_addr(m_mcastAddr.c_str());

		return 0;
	}

	//发送数据
	const int Send(void * pdata, unsigned int len)
	{
		if (-1 == m_socket)
		{
			return -1;
		}
		return sendto(m_socket, (char *)pdata, len, 0, (struct sockaddr *)&m_toif, sizeof(m_toif));
	}

	//接收数据，这里需要做一个小包到大包的检查
	//由于组播的数据一般一包都比较小，因此先不做小包到大包的组包，留待以后出现问题再处理
	//只是简单封装了底层的数据，使用简单。
	const int Recvfrom(void * pbuffer, const unsigned int maxLen, int flags, sockaddr & fromAddr, int & fromAddrLen)
	{
		if (-1 == m_socket)
		{
			return -1;
		}
		return recvfrom(m_socket, (char *)pbuffer, maxLen, flags, &fromAddr, (socklen_t *)&fromAddrLen);
	}

	//接收不管来自何方
	const int Recv(void * pbuffer, const unsigned int maxLen, int flags)
	{
		if (-1 == m_socket)
		{
			return -1;
		}
		return recv(m_socket, (char *)pbuffer, maxLen, flags);
	}

	//关闭组播
	const int Close(void)
	{
		if (-1 == m_socket)
		{
			return 0;
		}

		//退出组播
		if (setsockopt(m_socket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&m_mreq, sizeof(m_mreq)) < 0)
		{
#if defined(_DEBUG)
			printf("Error, the global status udp exit multcast fail!\n");
#endif // _DEBUG
			//return -1;
		}

		//关闭端口
#ifdef WIN32
		int ret = closesocket(m_socket);
#elif QNX
		int ret = close(m_socket);
#elif  VXWORKS
		int ret = close(m_socket);
#endif
		if (0 != ret)
		{
#if defined(_DEBUG)
			printf("Error, close global status udp socket fail!\n");
#endif // _DEBUG
		}
		else
		{
			m_socket = -1;
		}

		return ret;
	}

private:
	std::string     m_localAddr;        //本地地址，理论上可以为空，但是很多时候，操作系统问题，为空可能有问题
	std::string     m_mcastAddr;        //多播地址
	unsigned short  m_localPort;        //本地端口
	unsigned short  m_mcastPort;        //多播端口

	int             m_socket;           //服务套接字
	sockaddr_in     m_localif;			/*本地网络接口  */
	sockaddr_in     m_toif;			    /*目的地网络接口*/
	struct ip_mreq  m_mreq;				/*多播地址      */
};

#endif // _HR_UDP_MULTICAST_H_
