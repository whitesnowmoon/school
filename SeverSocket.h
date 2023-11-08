#ifndef _SEVERSOCKET_H
#define _SEVERSOCKET_H

#include<iostream>

typedef int SEVERSOCKET; //socket
typedef unsigned short SEVERNETFAMILY; //协议簇
typedef unsigned short SEVERPORT; //端口
typedef std::string SEVERIP;  //ip地址

typedef struct _SEVERADDRESS_
{
	SEVERIP severIp;
	SEVERPORT severPort;
	SEVERNETFAMILY severFamily;
}SEVERADDRESS;     //网络地址描述


#ifdef _SEVER_LINUX_

class SeverSocket                                      //不同平台网络接口抽象层
{
public:
	static  int _init_() {
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != NO_ERROR) {
			return NO_ERROR;
		}
		return 0;
	}
	static  SEVERSOCKET _socket_() {
		SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
		else
		{
			return sock;
		}
	}
	static  int _bind_(SEVERSOCKET sock, const _SEVERADDRESS_& addr) {
		sockaddr_in service = { 0 };
		service.sin_family = addr.severFamily;
		service.sin_addr.s_addr = inet_addr(addr.severIp.c_str());
		service.sin_port = htons(addr.severPort);
		if (bind(sock, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
			closesocket(sock);
			WSACleanup();
			return SOCKET_ERROR;
		}
		return 0;
	}

	static  SEVERSOCKET _accept_(SEVERSOCKET sock, _SEVERADDRESS_* recvaddr) {
		int len; SEVERSOCKET result;
		sockaddr_in service = { 0 };
		len = sizeof(service);
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = htonl(INADDR_ANY);
		if (recvaddr != NULL) {
			result = accept(sock, (SOCKADDR*)&service, &len);
			if (result == SOCKET_ERROR) {
				recvaddr->severIp.clear();
				recvaddr->severPort = 0;
				recvaddr->severFamily = 0;
				return result;
			}
			recvaddr->severFamily = service.sin_family;
			recvaddr->severPort = ntohs(service.sin_port);
			recvaddr->severIp.append(inet_ntoa(service.sin_addr));
		}
		else
		{
			result = accept(sock, NULL, NULL);
			if (result == SOCKET_ERROR) {
				return result;
			}
		}
		return result;
	}

	static  int _listen_(SEVERSOCKET sock, int num) {
		int result = 0;
		result = listen(sock, num);
		if (result == SOCKET_ERROR)
		{
			closesocket(sock);
			WSACleanup();
		}
		return result;

	}
	static inline int _recv_(SEVERSOCKET sock, char* buff, int len, int flag) {
		return recv(sock, buff, len, flag);
	}
	static inline int _send_(SEVERSOCKET sock, char* buff, int len, int flag) {
		return send(sock, buff, len, flag);
	}

	static inline int _closesocket_(SEVERSOCKET sock) {
		return closesocket(sock);
	}

};

#endif

#ifdef _SEVER_WIN_
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include<WinSock2.h>
#pragma comment(lib,"Ws2_32.lib")
#endif

class SeverSocket                                      //不同平台网络接口抽象层
{
public:
	static  int _init_() {
		WSADATA wsaData;
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != NO_ERROR) {
			return NO_ERROR;
		}
		return 0;
	}
	static  SEVERSOCKET _socket_() {
		SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sock == SOCKET_ERROR) {
			return SOCKET_ERROR;
		}
		else
		{
			return sock;
		}
	}
	static  int _bind_(SEVERSOCKET sock, const _SEVERADDRESS_& addr) {
		sockaddr_in service = { 0 };
		service.sin_family = addr.severFamily;
		service.sin_addr.s_addr = inet_addr(addr.severIp.c_str());
		service.sin_port = htons(addr.severPort);
		if (bind(sock, (SOCKADDR*)&service, sizeof(service)) == SOCKET_ERROR) {
			closesocket(sock);
			WSACleanup();
			return SOCKET_ERROR;
		}
		return 0;
	}

	static  SEVERSOCKET _accept_(SEVERSOCKET sock, _SEVERADDRESS_* recvaddr) {
		int len; SEVERSOCKET result; 
		sockaddr_in service = { 0 };
		len = sizeof(service);
		service.sin_family = AF_INET;
		service.sin_addr.s_addr = htonl(INADDR_ANY);
		if (recvaddr != NULL) {
			result = accept(sock, (SOCKADDR*)&service, &len);
			if (result == SOCKET_ERROR) {
				recvaddr->severIp.clear();
				recvaddr->severPort = 0;
				recvaddr->severFamily = 0;
				return result;
			}
			recvaddr->severFamily = service.sin_family;
			recvaddr->severPort = ntohs(service.sin_port);
			recvaddr->severIp.append(inet_ntoa(service.sin_addr));
		}
		else
		{
			result = accept(sock, NULL, NULL);
			if (result == SOCKET_ERROR) {
				return result;
			}
		}
		return result;
	}

	static  int _listen_(SEVERSOCKET sock, int num) {
		int result = 0;
		result = listen(sock, num);
		if (result == SOCKET_ERROR)
		{
			closesocket(sock);
			WSACleanup();
		}
		return result;

	}
	static inline int _recv_(SEVERSOCKET sock,char* buff,int len,int flag) {
		return recv(sock, buff, len, flag);
	}
	static inline int _send_(SEVERSOCKET sock, char* buff, int len, int flag) {
		return send(sock, buff, len, flag);
	}

	static inline int _closesocket_(SEVERSOCKET sock) {
		return closesocket(sock);
	}

	//设置非阻塞socket,缓冲区buff,接受和发送超时;
	static bool _setsock_(SEVERSOCKET sock, int size, unsigned long recv_time_temp, unsigned long send_time_temp,bool non_blocking) {  
		unsigned long on_off = non_blocking;
		int result = ioctlsocket(sock, FIONBIO, &on_off);
		if (result != NO_ERROR) {
			printf("set _non_blocking_ error\n");
			return false;
		}
		DWORD time_r = recv_time_temp;//毫秒		
		DWORD time_s = send_time_temp;//毫秒	
		int optVal = size;//缓冲
		int optLen = sizeof(int);
		int re = setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (char*)&time_r, sizeof(time_r));
		int rb = setsockopt(sock, SOL_SOCKET, SO_RCVBUF, (char*)&optVal, optLen);
		int se = setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, (char*)&time_s, time_s);
		if (!re && !rb && !se) {
			return true;
		}
		printf("set _out_time_ error\n");
		return false;
	}
};

#endif // !SEVERSOCKET_H


