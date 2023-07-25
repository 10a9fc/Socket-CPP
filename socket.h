#ifndef SOCKET_H
#define SOCKET_H

#include <iostream>
#include <exception>

#ifdef _WIN32
	#ifdef _MSC_VER
		#include <WinSock2.h>
		#pragma comment(lib,"ws2_32.lib")
		#pragma warning(disable:4996)
	#endif 
#endif

#ifdef _linux_
	#ifdef __GUNC__
		#include<sys/types.h>
		#include<sys/socket.h>
		#include<netinet/in.h>
	#endif
#endif


namespace Net
{
	class IPAddress
	{
	public:
		static IPAddress Parse(std::string addr);
		IPAddress(unsigned long addr);

		unsigned long GetAddress();
	private:
		unsigned long address;
	};

	class IPEndPoint
	{
	public:
		IPEndPoint() {}
		IPEndPoint(IPAddress&& addr, unsigned short port);

		unsigned long GetIPAddress();
		unsigned short GetPort();
	private:
		unsigned long ip = 0;
		unsigned short port = 0;
	};

	enum class AddressFamily { Ipv4 = AF_INET, Ipv6 = AF_INET6 };
	enum class SocketType{ Stream = SOCK_STREAM, Dgram = SOCK_DGRAM };

	class Socket
	{
	public:
		Socket(AddressFamily addressFamily, SocketType socketType);

		static void Init();
		void Bind(IPEndPoint ipEndPoint);
		void Listen(int num);
		Socket Accept();
		void Receive(char* data, int len);
		void Send(const char* data, int len);
		void ReceiveFrom(IPEndPoint point, char* data, int len);
		void SendTo(IPEndPoint point, const char* data, int len);
		void Connect(IPEndPoint ipEndPoint);
		void Close();

	private:
		AddressFamily addressFamily;
		SocketType socketType;

		IPEndPoint ipEndPoint;
		SOCKET socket_id = 0;
	};
}

#endif

