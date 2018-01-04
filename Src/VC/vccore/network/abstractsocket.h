
#ifndef _HR_ABSTRACT_SOCKET_H_
#define _HR_ABSTRACT_SOCKET_H_

class CAbstractSocket
{
public:
	enum SocketType {
		TcpSocket,
		UdpSocket,
		UnknownSocketType = -1
	};
	enum NetworkLayerProtocol {
		IPv4Protocol,
		IPv6Protocol,
		AnyIPProtocol,
		UnknownNetworkLayerProtocol = -1
	};
	enum SocketError {
		ConnectionRefusedError,
		RemoteHostClosedError,
		HostNotFoundError,
		SocketAccessError,
		SocketResourceError,
		SocketTimeoutError,                     /* 5 */
		DatagramTooLargeError,
		NetworkError,
		AddressInUseError,
		SocketAddressNotAvailableError,
		UnsupportedSocketOperationError,        /* 10 */
		UnfinishedSocketOperationError,
		ProxyAuthenticationRequiredError,
		SslHandshakeFailedError,
		ProxyConnectionRefusedError,
		ProxyConnectionClosedError,             /* 15 */
		ProxyConnectionTimeoutError,
		ProxyNotFoundError,
		ProxyProtocolError,
		OperationError,
		SslInternalError,                       /* 20 */
		SslInvalidUserDataError,
		TemporaryError,

		UnknownSocketError = -1
	};
	enum SocketState {
		UnconnectedState,
		HostLookupState,
		ConnectingState,
		ConnectedState,
		BoundState,
		ListeningState,
		ClosingState
	};
	enum SocketOption {
		LowDelayOption, // TCP_NODELAY
		KeepAliveOption, // SO_KEEPALIVE
		MulticastTtlOption, // IP_MULTICAST_TTL
		MulticastLoopbackOption, // IP_MULTICAST_LOOPBACK
		TypeOfServiceOption //IP_TOS
	};
	enum BindMode {
		DefaultForPlatform = 0x0,
		ShareAddress = 0x1,
		DontShareAddress = 0x2,
		ReuseAddressHint = 0x4
	};
	enum PauseMode {
		PauseNever = 0x0,
		PauseOnSslErrors = 0x1
	};

public:
	CAbstractSocket(SocketType socketType)
	{

	}

	virtual ~CAbstractSocket()
	{

	}

};

#endif  // _HR_ABSTRACT_SOCKET_H_
