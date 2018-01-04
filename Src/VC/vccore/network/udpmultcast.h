
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
		//���ͷ�ָ�룬���ɲ�ѯ������ӵ��
		Close();
	}

private:
	//��ֹ�����͸���
	CUdpMultcast(const CUdpMultcast& rhs);
	const CUdpMultcast& operator=(const CUdpMultcast& rhs);

public:
	const int Init(std::string localAddr, std::string mcastAddr, unsigned short localPort, unsigned short mcastPort,
		bool enalbeMcastLoopBack = false, unsigned int msgBufferSize = 204800, bool enableReuse = false)
	{
		if (-1 != m_socket)
		{
			return 1;   //�Ѿ����ˣ��������Ǵ��󣬷�����1
		}

		this->m_localAddr = localAddr;
		this->m_mcastAddr = mcastAddr;
		this->m_localPort = localPort;
		this->m_mcastPort = mcastPort;

		//�����׽���
		m_socket = (int)socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (m_socket < 0)
		{
			return -1;
		}

		//�趨�Ƿ������ظ���
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

		/*���ص�ַ������Vxworks��bug������ֱ��ʹ�ñ����ƶ���ַ�󶨶ಥ��Ҫ�Ȱ������ַ��Ȼ���Ϊָ����ַ*/
		m_localif.sin_family = AF_INET;
		m_localif.sin_port = htons(localPort);
		m_localif.sin_addr.s_addr = htonl(INADDR_ANY);
		/*�󶨱�����ַ*/
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

		//����ಥ
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

		/*�趨�ಥ�ı��ؽӿڵ�ַ*/
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

		//�鲥������
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

		//�ಥ�ػ�
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

		//��ʼ���鲥Ŀ�ĵص�ַ
		m_toif.sin_family = AF_INET;
		m_toif.sin_port = htons(mcastPort);
		m_toif.sin_addr.s_addr = inet_addr(m_mcastAddr.c_str());

		return 0;
	}

	//��������
	const int Send(void * pdata, unsigned int len)
	{
		if (-1 == m_socket)
		{
			return -1;
		}
		return sendto(m_socket, (char *)pdata, len, 0, (struct sockaddr *)&m_toif, sizeof(m_toif));
	}

	//�������ݣ�������Ҫ��һ��С��������ļ��
	//�����鲥������һ��һ�����Ƚ�С������Ȳ���С�������������������Ժ���������ٴ���
	//ֻ�Ǽ򵥷�װ�˵ײ�����ݣ�ʹ�ü򵥡�
	const int Recvfrom(void * pbuffer, const unsigned int maxLen, int flags, sockaddr & fromAddr, int & fromAddrLen)
	{
		if (-1 == m_socket)
		{
			return -1;
		}
		return recvfrom(m_socket, (char *)pbuffer, maxLen, flags, &fromAddr, (socklen_t *)&fromAddrLen);
	}

	//���ղ������Ժη�
	const int Recv(void * pbuffer, const unsigned int maxLen, int flags)
	{
		if (-1 == m_socket)
		{
			return -1;
		}
		return recv(m_socket, (char *)pbuffer, maxLen, flags);
	}

	//�ر��鲥
	const int Close(void)
	{
		if (-1 == m_socket)
		{
			return 0;
		}

		//�˳��鲥
		if (setsockopt(m_socket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&m_mreq, sizeof(m_mreq)) < 0)
		{
#if defined(_DEBUG)
			printf("Error, the global status udp exit multcast fail!\n");
#endif // _DEBUG
			//return -1;
		}

		//�رն˿�
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
	std::string     m_localAddr;        //���ص�ַ�������Ͽ���Ϊ�գ����Ǻܶ�ʱ�򣬲���ϵͳ���⣬Ϊ�տ���������
	std::string     m_mcastAddr;        //�ಥ��ַ
	unsigned short  m_localPort;        //���ض˿�
	unsigned short  m_mcastPort;        //�ಥ�˿�

	int             m_socket;           //�����׽���
	sockaddr_in     m_localif;			/*��������ӿ�  */
	sockaddr_in     m_toif;			    /*Ŀ�ĵ�����ӿ�*/
	struct ip_mreq  m_mreq;				/*�ಥ��ַ      */
};

#endif // _HR_UDP_MULTICAST_H_
