#ifndef _SEVERDATABASE_H_
#define _SEVERDATABASE_H_
#include<stdio.h>
#include<iostream>
#include<map>
#include<vector>
#include"sqlite3.h"
#include"SeverConst.h"

//有user表
class SeverDatabase
{
public:
	static sqlite3* db;                   //全局句柄
	SeverDatabase() = default;
	virtual ~SeverDatabase()= default;
	static int open(const char* filename);  //开启数据库，只需全局开启一次
	static int close();            //关闭数据库，全局关闭一次
	//继承特殊实现，参数不限，留参考
	virtual int AddData() { return 0; } //增添数据
	virtual int DeleteData() { return 0; }; //删除数据
	virtual int FindData() { return 0; };  //查找数据
	virtual int ModifyData() { return 0; }; //修改数据
};

sqlite3* SeverDatabase::db = nullptr;
int SeverDatabase::open(const char* filename) {
	return sqlite3_open(filename, &SeverDatabase::db);
}
int SeverDatabase::close() {
	if (SeverDatabase::db) {
		return sqlite3_close(SeverDatabase::db);
	}
	else
	{
		return SQLITE_ERROR;
	}
}


//使用user表
//return 0表示成功
class Login_Database :public SeverDatabase
{
public:
	Login_Database()=default;
	~Login_Database()=default;
	int AddData(std::map<std::string, std::string> data) {    //map必须有username,password,authority字段
		if ((data["username"].size() > 20 || data["username"].size() <= 0) ||
			(data["password"].size() > 20 || data["password"].size() <= 0) ||
			data["authority"].size() == 0)
			return -1;//检查数据长度是否符合要求
		sqlite3_stmt* stmt;
		int res = sqlite3_prepare(this->db, "insert into user(username,password,authority) values (?,?,?);", -1, &stmt, nullptr);
		if (res == SQLITE_OK) {
			sqlite3_bind_text(stmt, 1, data["username"].c_str(), data["username"].size(), NULL);
			sqlite3_bind_text(stmt, 2, data["password"].c_str(), data["password"].size(), NULL);
			sqlite3_bind_text(stmt, 3, data["authority"].c_str(), data["authority"].size(), NULL);
			res = sqlite3_step(stmt);
			if (res != SQLITE_DONE) {
				sqlite3_finalize(stmt);
				return -1;
			}
		}
		sqlite3_finalize(stmt);
		return 0;
	}
	std::string FindData(std::map<std::string, std::string> data) {    //map必须有username,password字段,找不到用户返回"Null"
		sqlite3_stmt* stmt; int Count = 0; std::string authority;
		int res = sqlite3_prepare(this->db, "select* from user where username=? and password=?;", -1, &stmt, nullptr);
		if (res == SQLITE_OK) {
			sqlite3_bind_text(stmt, 1, data["username"].c_str(), data["username"].size(), NULL);
			sqlite3_bind_text(stmt, 2, data["password"].c_str(), data["password"].size(), NULL);
			while (SQLITE_ROW == sqlite3_step(stmt)) {
				printf("login:  id: %d,username: %s, password: %s  authority: %s\n",
					sqlite3_column_int(stmt, 0),
					sqlite3_column_text(stmt, 1),
					sqlite3_column_text(stmt, 2),
					authority.append((char*)sqlite3_column_text(stmt, 3)));
				Count++;
			}
		}
		sqlite3_finalize(stmt);
		if (Count != 1) {
			authority = "NULL";
		}
		return authority;
	}

};

//使用doctorschedule表
class Doctorschedule_Database :public SeverDatabase
{
public:
	Doctorschedule_Database() = default;
	~Doctorschedule_Database() = default;
	long long time_str(std::string strTime) {
		tm _tm;
		int year, month, day, hour, minute;
		sscanf(strTime.c_str(), "%d-%d-%dT%d:%d", &year, &month, &day, &hour, &minute);
		_tm.tm_year = year - 1900;
		_tm.tm_mon = month - 1;
		_tm.tm_mday = day;
		_tm.tm_hour = hour;
		_tm.tm_min = minute;
		_tm.tm_sec = 0;
		_tm.tm_isdst = 0;
		time_t t = mktime(&_tm);
		return t;
	}
	int AddData(std::map<std::string, std::string> data) {    //map必须有name,,addr,timeup,timedown字段 正确返回0 错误-1
		sqlite3_stmt* stmt;
		int res = sqlite3_prepare(this->db, "insert into doctorschedule(number,name,addr,timeup,timedown) values (?,?,?,?,?);", -1, &stmt, nullptr);
		if (res == SQLITE_OK) {
			sqlite3_bind_int64(stmt, 1, time_str(data["timeup"]));//重复没关系
			//sqlite3_bind_int64(stmt, 1, time(NULL));//方案2不可排序
			sqlite3_bind_text(stmt, 2, data["name"].c_str(), data["name"].size(), NULL);
			sqlite3_bind_text(stmt, 3, data["addr"].c_str(), data["addr"].size(), NULL);
			sqlite3_bind_text(stmt, 4, data["timeup"].c_str(), data["timeup"].size(), NULL);
			sqlite3_bind_text(stmt, 5, data["timedown"].c_str(), data["timedown"].size(), NULL);
			res = sqlite3_step(stmt);
			if (res != SQLITE_DONE) {
				sqlite3_finalize(stmt);
				return -1;
			}
		}
		sqlite3_finalize(stmt);
		return 0;
	}
	int DeleteData(std::map<std::string, std::string> data) {    //map必须有number
		sqlite3_stmt* stmt; int Count = 0; std::string authority;
		int res = sqlite3_prepare(this->db, "delete from doctorschedule where doctorschedule.number=? AND doctorschedule.name=?;", -1, &stmt, nullptr);
		if (res == SQLITE_OK) {
			sqlite3_bind_int64(stmt, 1, std::atoll(data["number"].c_str()));
			sqlite3_bind_text(stmt, 2, data["name"].c_str(), data["name"].size(), NULL);
			res = sqlite3_step(stmt);
			if (res != SQLITE_DONE) {
				sqlite3_finalize(stmt);
				return -1;
			}
		}
		sqlite3_finalize(stmt);
		return 0;
	}

	std::string GetData() {   
		sqlite3_stmt* stmt; int Count = 0; std::string _data_; char buff[2048];
		int res = sqlite3_prepare(this->db, "select* from doctorschedule ORDER BY doctorschedule.number DESC", -1, &stmt, nullptr);
		if (res == SQLITE_OK) {
			int num = 1000;//内存限制，其他地方没改，以后再说
			while (SQLITE_ROW == sqlite3_step(stmt)&&num>0) {
				const char* format = DOCTOR_FORM_HTML;
				int sizestr=sprintf(buff, format,
					std::to_string(sqlite3_column_int64(stmt, 0)).c_str(),
					sqlite3_column_text(stmt, 1),
					sqlite3_column_text(stmt, 2),
					sqlite3_column_text(stmt, 3),
					sqlite3_column_text(stmt, 4)
				);
				_data_.append(buff,sizestr);
			}
		}
		sqlite3_finalize(stmt);
		return _data_;
	}

};

//使用drugin表
class Drugin_Database :public SeverDatabase
{
public:
	int AddData(std::map<std::string, std::string> data) {    //map必须有name,count,effectivedate,productiondate,business,head,inputdate字段 正确返回0 错误-1
		sqlite3_stmt* stmt;
		int res = sqlite3_prepare(this->db, "insert into drugin(name,count,effectivedate,productiondate,business,head,inputdate,number) values (?,?,?,?,?,?,?,?);", -1, &stmt, nullptr);
		if (res == SQLITE_OK){
			sqlite3_bind_text(stmt, 1, data["name"].c_str(), data["name"].size(), NULL);
			sqlite3_bind_int(stmt, 2, std::atoi(data["count"].c_str()));
			sqlite3_bind_text(stmt, 3, data["effectivedate"].c_str(), data["effectivedate"].size(), NULL);
			sqlite3_bind_text(stmt, 4, data["productiondate"].c_str(), data["productiondate"].size(), NULL);
			sqlite3_bind_text(stmt, 5, data["business"].c_str(), data["business"].size(), NULL);
			sqlite3_bind_text(stmt, 6, data["head"].c_str(), data["head"].size(), NULL);
			sqlite3_bind_text(stmt, 7, data["inputdate"].c_str(), data["inputdate"].size(), NULL);
			sqlite3_bind_int64(stmt, 8, time_str(data["inputdate"]));
			res = sqlite3_step(stmt);
			if (res != SQLITE_DONE) {
				sqlite3_finalize(stmt);
				return -1;
			}
		}
	    sqlite3_finalize(stmt);
		return 0;
	}
	std::string GetData() {
		sqlite3_stmt* stmt; int Count = 0; std::string _data_;  char* buff = new char[204800];
		int res = sqlite3_prepare(this->db, "select* from drugin ORDER BY drugin.number DESC", -1, &stmt, nullptr);
		if (res == SQLITE_OK) {
			while (SQLITE_ROW == sqlite3_step(stmt)) {
				const char* format = DRUGIN_FORM_HTML;
				int sizestr = sprintf(buff, format,
					sqlite3_column_text(stmt, 1),
					sqlite3_column_text(stmt, 2),
					sqlite3_column_text(stmt, 3),
					sqlite3_column_text(stmt, 4),
					sqlite3_column_text(stmt, 5),
					sqlite3_column_text(stmt, 6),
					sqlite3_column_text(stmt, 7)
				);
				_data_.append(buff, sizestr);
			}
		}
		delete buff;
		sqlite3_finalize(stmt);
		return _data_;
	}
private:
	long long time_str(std::string strTime) {
		int year, month, day;
		sscanf(strTime.c_str(), "%d-%d-%d", &year, &month, &day);
		return year*10000+month*100+day;
	}
};

//使用drugout表
class Drugout_Database :public SeverDatabase
{
public:
	int AddData(std::map<std::string, std::string> data) {    //map必须有name,count,outputdate,patient,head,number字段 正确返回0 错误-1
		sqlite3_stmt* stmt;
		int res = sqlite3_prepare(this->db, "insert into drugout(name,count,outputdate,patient,head,number) values (?,?,?,?,?,?);", -1, &stmt, nullptr);
		if (res == SQLITE_OK) {
			sqlite3_bind_text(stmt, 1, data["name"].c_str(), data["name"].size(), NULL);
			sqlite3_bind_int(stmt, 2, std::atoi(data["count"].c_str()));
			sqlite3_bind_text(stmt, 3, data["outputdate"].c_str(), data["outputdate"].size(), NULL);
			sqlite3_bind_text(stmt, 4, data["patient"].c_str(), data["patient"].size(), NULL);
			sqlite3_bind_text(stmt, 5, data["head"].c_str(), data["head"].size(), NULL);
			sqlite3_bind_int64(stmt, 6, time_str(data["outputdate"]));
			res = sqlite3_step(stmt);
			if (res != SQLITE_DONE) {
				sqlite3_finalize(stmt);
				return -1;
			}
		}
		sqlite3_finalize(stmt);
		return 0;
	}
	std::string GetData() {
		sqlite3_stmt* stmt; int Count = 0; std::string _data_;  char* buff = new char[204800];
		int res = sqlite3_prepare(this->db, "select* from drugout ORDER BY drugout.number DESC", -1, &stmt, nullptr);
		if (res == SQLITE_OK) {
			while (SQLITE_ROW == sqlite3_step(stmt)) {
				const char* format = DRUGOUT_FORM_HTML;
				int sizestr = sprintf(buff, format,
					sqlite3_column_text(stmt, 1),
					sqlite3_column_text(stmt, 2),
					sqlite3_column_text(stmt, 3),
					sqlite3_column_text(stmt, 4),
					sqlite3_column_text(stmt, 5)
				);
				_data_.append(buff, sizestr);
			}
		}
		delete buff;
		sqlite3_finalize(stmt);
		return _data_;
	}
private:
	long long time_str(std::string strTime) {
		int year, month, day;
		sscanf(strTime.c_str(), "%d-%d-%d", &year, &month, &day);
		return year * 10000 + month * 100 + day;
	}
};

//使用drugnum表
class Drugnum_Database :public SeverDatabase
{
public:
	std::string GetData() {
		sqlite3_stmt* stmt; int Count = 0; std::string _data_; char* buff=new char[204800];
		int res = sqlite3_prepare(this->db, "select* from drugnum", -1, &stmt, nullptr);
		if (res == SQLITE_OK) {
			while (SQLITE_ROW == sqlite3_step(stmt)) {
				const char* format = DRUGNUM_FORM_HTML;
				int sizestr = sprintf(buff, format,
					sqlite3_column_text(stmt, 0),
					sqlite3_column_text(stmt, 1)
				);
				_data_.append(buff, sizestr);
			}
		}
		delete buff;
		sqlite3_finalize(stmt);
		return _data_;
	}
};

//使用Nurseschedule表
class Nurseschedule_Database :public SeverDatabase
{
public:
	Nurseschedule_Database() = default;
	~Nurseschedule_Database() = default;
	long long time_str(std::string strTime) {
		tm _tm;
		int year, month, day, hour, minute;
		sscanf(strTime.c_str(), "%d-%d-%dT%d:%d", &year, &month, &day, &hour, &minute);
		_tm.tm_year = year - 1900;
		_tm.tm_mon = month - 1;
		_tm.tm_mday = day;
		_tm.tm_hour = hour;
		_tm.tm_min = minute;
		_tm.tm_sec = 0;
		_tm.tm_isdst = 0;
		time_t t = mktime(&_tm);
		return t;
	}
	int AddData(std::map<std::string, std::string> data) {    //map必须有name,,addr,timeup,timedown,leadername字段 正确返回0 错误-1
		sqlite3_stmt* stmt;
		int res = sqlite3_prepare(this->db, "insert into nurseschedule(number,name,addr,timeup,timedown,leadername) values (?,?,?,?,?,?);", -1, &stmt, nullptr);
		if (res == SQLITE_OK) {
			sqlite3_bind_int64(stmt, 1, time_str(data["timeup"]));//重复没关系
			//sqlite3_bind_int64(stmt, 1, time(NULL));//方案2不可排序
			sqlite3_bind_text(stmt, 2, data["name"].c_str(), data["name"].size(), NULL);
			sqlite3_bind_text(stmt, 3, data["addr"].c_str(), data["addr"].size(), NULL);
			sqlite3_bind_text(stmt, 4, data["timeup"].c_str(), data["timeup"].size(), NULL);
			sqlite3_bind_text(stmt, 5, data["timedown"].c_str(), data["timedown"].size(), NULL);
			sqlite3_bind_text(stmt, 6, data["leadername"].c_str(), data["leadername"].size(), NULL);
			res = sqlite3_step(stmt);
			if (res != SQLITE_DONE) {
				sqlite3_finalize(stmt);
				return -1;
			}
		}
		sqlite3_finalize(stmt);
		return 0;
	}
	int DeleteData(std::map<std::string, std::string> data) {    //map必须有number
		sqlite3_stmt* stmt; int Count = 0; std::string authority;
		int res = sqlite3_prepare(this->db, "delete from nurseschedule where nurseschedule.number=? AND nurseschedule.name=?;", -1, &stmt, nullptr);
		if (res == SQLITE_OK) {
			sqlite3_bind_int64(stmt, 1, std::atoll(data["number"].c_str()));
			sqlite3_bind_text(stmt, 2, data["name"].c_str(), data["name"].size(), NULL);
			res = sqlite3_step(stmt);
			if (res != SQLITE_DONE) {
				sqlite3_finalize(stmt);
				return -1;
			}
		}
		sqlite3_finalize(stmt);
		return 0;
	}

	std::string GetData() {
		sqlite3_stmt* stmt; int Count = 0; std::string _data_;  char* buff = new char[204800];
		int res = sqlite3_prepare(this->db, "select* from nurseschedule ORDER BY nurseschedule.number DESC", -1, &stmt, nullptr);
		if (res == SQLITE_OK) {
			while (SQLITE_ROW == sqlite3_step(stmt)) {
				const char* format = NURSE_FORM_HTML;
				int sizestr = sprintf(buff, format,
					std::to_string(sqlite3_column_int64(stmt, 0)).c_str(),
					sqlite3_column_text(stmt, 1),
					sqlite3_column_text(stmt, 2),
					sqlite3_column_text(stmt, 3),
					sqlite3_column_text(stmt, 4),
					sqlite3_column_text(stmt, 5)
				);
				_data_.append(buff, sizestr);
			}
		}
		delete buff;
		sqlite3_finalize(stmt);
		return _data_;
	}

};


class Hospital_Database :public SeverDatabase
{
public:
	int AddData(std::map<std::string, std::string> data) {    //正确返回0 错误-1
		sqlite3_stmt* stmt;
		int res = sqlite3_prepare(this->db, "insert into hospital(name,age,addr,sex,phone,reason,floor,department,bed,nursename,doctorname) values (?,?,?,?,?,?,?,?,?,?,?);", -1, &stmt, nullptr);
		if (res == SQLITE_OK) {
			sqlite3_bind_text(stmt, 1, data["name"].c_str(), data["name"].size(), NULL);
			sqlite3_bind_int(stmt, 2, std::atoi(data["age"].c_str()));
			sqlite3_bind_text(stmt, 3, data["addr"].c_str(), data["addr"].size(), NULL);
			sqlite3_bind_text(stmt, 4, data["sex"].c_str(), data["sex"].size(), NULL);
			sqlite3_bind_text(stmt, 5, data["phone"].c_str(), data["phone"].size(), NULL);
			sqlite3_bind_text(stmt, 6, data["reason"].c_str(), data["reason"].size(), NULL);
			sqlite3_bind_text(stmt, 7, data["floor"].c_str(), data["floor"].size(), NULL);
			sqlite3_bind_text(stmt, 8, data["department"].c_str(), data["department"].size(), NULL);
			sqlite3_bind_text(stmt, 9, data["bed"].c_str(), data["bed"].size(), NULL);
			sqlite3_bind_text(stmt, 10, data["nursename"].c_str(), data["nursename"].size(), NULL);
			sqlite3_bind_text(stmt, 11, data["doctorname"].c_str(), data["doctorname"].size(), NULL);
			res = sqlite3_step(stmt);
			if (res != SQLITE_DONE) {
				sqlite3_finalize(stmt);
				return -1;
			}
		}
		sqlite3_finalize(stmt);
		return 0;
	}
	std::string GetDataS() {
		sqlite3_stmt* stmt; int Count = 0; std::string _data_;  char* buff = new char[204800];
		int res = sqlite3_prepare(this->db, "select floor,department,bed,name,nursename,doctorname from hospital", -1, &stmt, nullptr);
		if (res == SQLITE_OK) {
			int Count = 1000;
			while (SQLITE_ROW == sqlite3_step(stmt)&& Count!=0) {
				const char* format = HOSPITALMANAGE_FORM_HTML;
				int sizestr = sprintf(buff, format,
					sqlite3_column_text(stmt, 0),
					sqlite3_column_text(stmt, 1),
					sqlite3_column_text(stmt, 2),
					sqlite3_column_text(stmt, 3),
					sqlite3_column_text(stmt, 4),
					sqlite3_column_text(stmt, 5)
				);
				_data_.append(buff, sizestr);
				Count--;
			}
		}
		delete buff;
		sqlite3_finalize(stmt);
		return _data_;
	}
	std::string GetDataOne(std::map<std::string, std::string> data, std::string format) {
		sqlite3_stmt* stmt; int Count = 0; std::string _data_;  char* buff = new char[204800];
		int res = sqlite3_prepare(this->db, "select * from hospital where name=? and department=?", -1, &stmt, nullptr);
		if (res == SQLITE_OK) {
			sqlite3_bind_text(stmt, 1, data["name"].c_str(), data["name"].size(), NULL);
			sqlite3_bind_text(stmt, 2, data["department"].c_str(), data["department"].size(), NULL);
			while (SQLITE_ROW == sqlite3_step(stmt)) {
				int sizestr = sprintf(buff, format.c_str(),
					sqlite3_column_text(stmt, 0),
					sqlite3_column_text(stmt, 1),
					sqlite3_column_text(stmt, 6),
					sqlite3_column_text(stmt, 8),
					sqlite3_column_text(stmt, 7),
					sqlite3_column_text(stmt, 10),
					sqlite3_column_text(stmt, 9),
					sqlite3_column_text(stmt, 2),
					sqlite3_column_text(stmt, 3),
					sqlite3_column_text(stmt, 4),
					sqlite3_column_text(stmt, 5)
				);
				_data_.append(buff, sizestr);
			}
		}
		delete buff;
		sqlite3_finalize(stmt);
		return _data_;
	}
	int DeleteData(std::map<std::string, std::string> data) {
		sqlite3_stmt* stmt; int Count = 0; std::string authority;
		int res = sqlite3_prepare(this->db, "delete from hospital where name=? and department=?;", -1, &stmt, nullptr);
		if (res == SQLITE_OK) {
			sqlite3_bind_text(stmt, 1, data["name"].c_str(), data["name"].size(), NULL);
			sqlite3_bind_text(stmt, 2, data["department"].c_str(), data["department"].size(), NULL);
			res = sqlite3_step(stmt);
			if (res != SQLITE_DONE) {
				sqlite3_finalize(stmt);
				return -1;
			}
		}
		else
		{
			return -1;
		}
		sqlite3_finalize(stmt);
		return 0;
	}
	int ModifyData(std::map<std::string, std::string> data) {    //正确返回0 错误-1
		sqlite3_stmt* stmt;
		int res = sqlite3_prepare(this->db, "UPDATE hospital SET name=?,age=?,addr=?,sex=?,phone=?,reason=?,floor=?,department=?,bed=?,nursename=?,doctorname=? WHERE name=? AND department=?", -1, &stmt, nullptr);
		if (res == SQLITE_OK) {
			sqlite3_bind_text(stmt, 1, data["name"].c_str(), data["name"].size(), NULL);
			sqlite3_bind_int(stmt, 2, std::atoi(data["age"].c_str()));
			sqlite3_bind_text(stmt, 3, data["addr"].c_str(), data["addr"].size(), NULL);
			sqlite3_bind_text(stmt, 4, data["sex"].c_str(), data["sex"].size(), NULL);
			sqlite3_bind_text(stmt, 5, data["phone"].c_str(), data["phone"].size(), NULL);
			sqlite3_bind_text(stmt, 6, data["reason"].c_str(), data["reason"].size(), NULL);
			sqlite3_bind_text(stmt, 7, data["floor"].c_str(), data["floor"].size(), NULL);
			sqlite3_bind_text(stmt, 8, data["department"].c_str(), data["department"].size(), NULL);
			sqlite3_bind_text(stmt, 9, data["bed"].c_str(), data["bed"].size(), NULL);
			sqlite3_bind_text(stmt, 10, data["nursename"].c_str(), data["nursename"].size(), NULL);
			sqlite3_bind_text(stmt, 11, data["doctorname"].c_str(), data["doctorname"].size(), NULL);

			sqlite3_bind_text(stmt, 12, data["name"].c_str(), data["name"].size(), NULL);
			sqlite3_bind_text(stmt, 13, data["department"].c_str(), data["department"].size(), NULL);
			res = sqlite3_step(stmt);
			if (res != SQLITE_DONE) {
				sqlite3_finalize(stmt);
				return -1;
			}
		}
		else
		{
			return -1;
		}
		sqlite3_finalize(stmt);
		return 0;
	}
};


//Operation表
class Operation_Database :public SeverDatabase
{
public:
	int AddData(std::map<std::string, std::string> data) {    //正确返回0 错误-1
		sqlite3_stmt* stmt;
		int res = sqlite3_prepare(this->db, "insert into operation(name,age,doctor,assistant1,assistant2,assistant3,assistant4,time,process,addr) values (?,?,?,?,?,?,?,?,?,?);", -1, &stmt, nullptr);
		if (res == SQLITE_OK) {
			sqlite3_bind_text(stmt, 1, data["name"].c_str(), data["name"].size(), NULL);
			sqlite3_bind_int(stmt, 2, std::atoi(data["age"].c_str()));
			sqlite3_bind_text(stmt, 3, data["doctor"].c_str(), data["doctor"].size(), NULL);
			sqlite3_bind_text(stmt, 4, data["assistant1"].c_str(), data["assistant1"].size(), NULL);
			sqlite3_bind_text(stmt, 5, data["assistant2"].c_str(), data["assistant2"].size(), NULL);
			sqlite3_bind_text(stmt, 6, data["assistant3"].c_str(), data["assistant3"].size(), NULL);
			sqlite3_bind_text(stmt, 7, data["assistant4"].c_str(), data["assistant4"].size(), NULL);
			sqlite3_bind_text(stmt, 8, data["time"].c_str(), data["time"].size(), NULL);
			sqlite3_bind_text(stmt, 9, data["process"].c_str(), data["process"].size(), NULL);
			sqlite3_bind_text(stmt, 10, data["addr"].c_str(), data["addr"].size(), NULL);
			res = sqlite3_step(stmt);
			if (res != SQLITE_DONE) {
				sqlite3_finalize(stmt);
				return -1;
			}
		}
		sqlite3_finalize(stmt);
		return 0;
	}
};


//prescription表
class Prescription_Database :public SeverDatabase
{
public:
	int AddData(std::map<std::string, std::string> data) {    //正确返回0 错误-1
		sqlite3_stmt* stmt;
		int res = sqlite3_prepare(this->db, "insert into prescription(name,sex,age,time,addr,phone,hospitalname,doctorname,text) values (?,?,?,?,?,?,?,?,?);", -1, &stmt, nullptr);
		if (res == SQLITE_OK) {
			sqlite3_bind_text(stmt, 1, data["name"].c_str(), data["name"].size(), NULL);
			sqlite3_bind_text(stmt, 2, data["sex"].c_str(), data["sex"].size(), NULL);
			sqlite3_bind_int(stmt, 3, std::atoi(data["age"].c_str()));
			sqlite3_bind_text(stmt, 4, data["time"].c_str(), data["time"].size(), NULL);
			sqlite3_bind_text(stmt, 5, data["addr"].c_str(), data["addr"].size(), NULL);
			sqlite3_bind_text(stmt, 6, data["phone"].c_str(), data["phone"].size(), NULL);
			sqlite3_bind_text(stmt, 7, data["hospitalname"].c_str(), data["hospitalname"].size(), NULL);
			sqlite3_bind_text(stmt, 8, data["doctorname"].c_str(), data["doctorname"].size(), NULL);
			sqlite3_bind_text(stmt, 9, data["text"].c_str(), data["text"].size(), NULL);
			res = sqlite3_step(stmt);
			if (res != SQLITE_DONE) {
				sqlite3_finalize(stmt);
				return -1;
			}
		}
		sqlite3_finalize(stmt);
		return 0;
	}
};

#endif // !_SEVERDATABASE_H_

