#define _CRT_SECURE_NO_WARNINGS
#define _SEVER_WIN_

#include"NetSever.h"

int main() {
#ifdef _SEVER_WIN_
	SetConsoleOutputCP(CP_UTF8);
#endif
	_SSL_::Get__SSL__Instance()->initSSL("moonclient.crt", "moonclient.key");
	_SEVERADDRESS_ addr;
	addr.severFamily = AF_INET;
	addr.severIp = "127.0.0.1";
	addr.severPort = 443;

	SeverDatabase::open("HIS_Database.db");

	NetSever::Get_NetIO_Instance()->ListenLoop(addr);
	SeverDatabase::close();
	return 0;
}
//#include<stdio.h>
//#include"sqlite3.h"
//int main() {
//	sqlite3* db;
//	sqlite3_open("HIS_Database.db", &db);
//	sqlite3_close(db);
//	return 0;
//}