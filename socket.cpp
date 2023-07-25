#include "socket.h"
using namespace Net;

IPAddress IPAddress::Parse(std::string addr)
{
	return IPAddress(inet_addr(addr.c_str()));
}

unsigned long IPAddress::GetAddress()
{
	return address;
}

IPAddress::IPAddress(unsigned long addr)
{
	address = addr;
}

IPEndPoint::IPEndPoint(IPAddress&& addr, unsigned short port)
{
	this->ip = addr.GetAddress();
	this->port = htons(port);
	//this->port = port;
}

unsigned long IPEndPoint::GetIPAddress()
{
	return this->ip;
}

unsigned short IPEndPoint::GetPort()
{
	return this->port;
}

Socket::Socket(AddressFamily addressFamily, SocketType socketType)
{
	this->addressFamily = addressFamily;
	this->socketType = socketType;
	//his->protocalFamily = protocalFamily;
}

void Socket::Init()
{
#ifdef _WIN32
	WORD wVersion = MAKEWORD(2, 2);
	WSADATA wsadata;
	if (WSAStartup(wVersion, &wsadata) < 0)
	{
		throw std::exception("初始化失败");
	}
#endif
}

void Socket::Connect(IPEndPoint ipEndPoint)
{
	sockaddr_in addr;
	addr.sin_family = (ADDRESS_FAMILY)addressFamily;
	addr.sin_addr.S_un.S_addr = ipEndPoint.GetIPAddress();
	addr.sin_port = ipEndPoint.GetPort();

	if (connect(socket_id, (sockaddr*)&addr, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		throw std::exception("连接失败");
	}
}

void Socket::Bind(IPEndPoint ipEndPoint)
{
	this->ipEndPoint = ipEndPoint;
	socket_id = socket((int)addressFamily, (int)socketType, 0);

	sockaddr_in addr;
	addr.sin_family = (ADDRESS_FAMILY)addressFamily;
	addr.sin_addr.S_un.S_addr = ipEndPoint.GetIPAddress();
	addr.sin_port = ipEndPoint.GetPort();

	if (bind(socket_id, (sockaddr*)&addr, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		throw std::exception("绑定端口失败");
	}
}

void Socket::Listen(int num)
{
	if (listen(socket_id, num) == SOCKET_ERROR)
	{
		throw std::exception("监听失败");
	}
}

Socket Socket::Accept()
{
	sockaddr_in addr;
	int len = sizeof(sockaddr_in);
	
	SOCKET id = accept(socket_id, (sockaddr*)&addr, &len);
	if (id == SOCKET_ERROR)
	{
		throw std::exception("等待请求失败");
	}

	Socket ret((AddressFamily)addr.sin_family, socketType);
	ret.socket_id = id;

	return ret;
}

void Socket::Receive(char* data, int len)
{
	if (recv(socket_id, data, len, 0) == SOCKET_ERROR)
	{
		throw std::exception("接收失败");
	}
}

void Socket::Send(const char* data, int len)
{
	if (send(socket_id, data, len, 0) == SOCKET_ERROR)
	{
		throw std::exception("发送失败");
	}
}

void Socket::ReceiveFrom(IPEndPoint point, char* data, int len)
{
	if (recvfrom(socket_id, data, len, 0, NULL, NULL) == SOCKET_ERROR)
	{
		throw std::exception("接收失败");
	}
}

void Socket::SendTo(IPEndPoint point, const char* data, int len)
{
	sockaddr_in addr;
	addr.sin_family = (ADDRESS_FAMILY)addressFamily;
	addr.sin_addr.S_un.S_addr = point.GetIPAddress();
	addr.sin_port = point.GetPort();
	if (sendto(socket_id, data, len, 0, (sockaddr*)&addr, sizeof(sockaddr_in)) == SOCKET_ERROR)
	{
		throw std::exception("发送失败");
	}
}

void Socket::Close()
{
	if (socket_id == 0)
	{
		throw std::exception("没有创建socket");
	}
#ifdef _WIN32
	closesocket(socket_id);
#endif
#ifdef _linux
	close(socket_id);
#endif
	socket_id = 0;
}
