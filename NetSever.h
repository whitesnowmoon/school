#ifndef _NETIO_H_
#define _NETIO_H_
/*

NetIO处理TCP流，对流进行抽象

*/

#include<iostream>
#include"SeverSocket.h"
#include"SeverSSL.h"
#include"SeverThreadPool.h"
#include"SeverLoginRouter.h"


typedef struct _SEVER_SOCKET_HANDLE
{
	_SEVERADDRESS_ listenaddr;             //服务器地址
	_SEVERADDRESS_ clientaddr;             //客户端地址
	SEVERSOCKET listensocket;              //服务器socket
	SEVERSOCKET clientsocket;              //客户端socket
	NetStreamSSL stream;                   //SSL流端口
}_SEVER_SOCKET_HANDLE_;


class SeverHandle :public SeverTask
{
public:
	//获取一个SeverHandle，交给线程池;
	static SeverHandle* GetSeverHandle(_SEVER_SOCKET_HANDLE_ handle) {
		SeverHandle* handletask = new SeverHandle();
		handletask->handle = handle;
		handletask->instance = handletask;
		return handletask;
	}
private:
	_SEVER_SOCKET_HANDLE_ handle;
	SeverHandle* instance;
	void run() {
		std::cout << std::this_thread::get_id() << "\n";
		bool set = SeverSocket::_setsock_(handle.clientsocket, 102400, 5000, 5000, 0);
		NetStreamSSL stream;
		if (stream.openstream(handle.clientsocket)) {
			handle.stream = stream;
			int res = 1;
			while (res > 0)
			{
				SeverRequest http(handle.stream);
				res=http.analysis();
				SeverLoginRouter router;
				router.respond(http);
			}
	        handle.stream.closestream();
			SeverSocket::_closesocket_(handle.clientsocket);


		}
		else
		{
			SeverSocket::_closesocket_(handle.clientsocket);
			std::cout << "socket end\n";
		}
		delete instance;
	}
};


class NetSever 
{
public:
	static NetSever* instance;
	static NetSever* Get_NetIO_Instance();   //获取NetIO单例对象
	void ListenLoop(_SEVERADDRESS_ Localaddr);    //进入监听循环
private:
	NetSever()=default;
	~NetSever()=default;
	SEVERSOCKET listensocket=-1;
	_SEVERADDRESS_ Localaddr;
	class clear
	{
	public:
		~clear() {
			if (instance)
				delete instance;
		}
	};
};




NetSever* NetSever::instance=nullptr;

NetSever* NetSever::Get_NetIO_Instance() {
	static clear clr_NetIO;
	if (instance == nullptr) {
		SeverSocket::_init_();
		instance = new NetSever();
	}
	return instance;
}
void NetSever::ListenLoop(_SEVERADDRESS_ Localaddr) {
	this->Localaddr = Localaddr;
	listensocket=SeverSocket::_socket_();
	SeverSocket::_bind_(listensocket, Localaddr);
	SeverSocket::_listen_(listensocket,128);
	while (true)
	{
		std::cout << "wait\n";
		_SEVER_SOCKET_HANDLE_ handle;
		handle.listenaddr = this->Localaddr;
		handle.listensocket = this->listensocket;
		handle.clientsocket=SeverSocket::_accept_(this->listensocket, &handle.clientaddr);
		if(handle.clientsocket!=-1)
			SeverThreadPool::Get_SeverThreadPool_Instance()->push_task(SeverHandle::GetSeverHandle(handle));
	}
}


#endif // !_NETIO_H_

