#ifndef _SEVERROUTERS_H_
#define _SEVERROUTERS_H_

#include"SeverHttp.h"
#include"SeverFile.h"
#include"SeverDatabase.h"
#include"SeverConst.h"
#include<list>

class SeverHospital
{
public:
	int next(SeverRequest& http, std::string cookie) {
		if (http.url == "/hospitalin" && http.method == "GET")//权限进入
		{
			return Router_hospitalin(http);
		}
		if (http.url == "/hospitalin_add" && http.method == "POST")//表单接受
		{
			return Router_hospitalin_add(http);
		}
		if (http.url == "/hospitalmanger" && http.method == "GET")//表单接受
		{
			return Router_hospitalmanage(http);
		}
		if (http.url == "/hospital_msg" && http.method == "POST")//表单接受
		{
			return Router_hospital_msg(http);
		}
		if (http.url == "/hospitalmod" && http.method == "POST")//表单接受
		{
			return Router_hospitalmod(http);
		}
		if (http.url == "/hospitaldel" && http.method == "POST")//表单接受
		{
			return Router_hospitaldel(http);
		}
		//下一次开发从这里开始,开始具体业务处理   2023/11/8
		//承接判断开始，会传入http, cookie，以无路径404结束
	}
private:
	int Router_hospitalin(SeverRequest& http) {
		SeverRespond respond(http.httpIterator);
		respond.protocol = http.protocol;
		respond.state = "200";
		respond.statecode = respond.StateMap["200"];
		SeverFile file;
		file.SeverFile_open("form/hospitalin.html");
		respond.AddContent(file.GetContent());
		file.SeverFile_close();
		int res = respond.Send();
		return res;
	}
	int Router_hospitalin_add(SeverRequest& http) {
		SeverRespond respond(http.httpIterator);
		respond.protocol = http.protocol;
		respond.state = "200";
		respond.statecode = respond.StateMap["200"];
		Hospital_Database db;
		if (db.AddData(http.urlcontent) == 0) {
			respond.AddContent("inputin");
		}
		else
		{
			respond.AddContent("error!");
		}
		int res = respond.Send();
		return res;
	}
	int Router_hospitalmanage(SeverRequest& http) {
		SeverRespond respond(http.httpIterator);
		respond.protocol = http.protocol;
		respond.state = "200";
		respond.statecode = respond.StateMap["200"];
		SeverFile file;
		file.SeverFile_open("form/hospitalmanger.html");
		std::string content = file.GetContent();
		std::string mark = "<!--insert_mark-->";
		int pos = content.find(mark);
		pos += mark.size();
		Hospital_Database db;
		std::string addurl = db.GetDataS();
		content.insert(pos, addurl);
		respond.AddContent(content);
		file.SeverFile_close();
		int res = respond.Send();
		return res;
	}
	int Router_hospital_msg(SeverRequest& http) {
		SeverRespond respond(http.httpIterator);
		respond.protocol = http.protocol;
		respond.state = "200";
		respond.statecode = respond.StateMap["200"];
		SeverFile file;
		file.SeverFile_open("form/hospitalmsg.html");
		std::string content = file.GetContent();
		Hospital_Database db;
		std::string addurl = db.GetDataOne(http.urlcontent,content);
		respond.AddContent(addurl);
		file.SeverFile_close();
		int res = respond.Send();
		return res;
	}
	int Router_hospitalmod(SeverRequest& http) {
		SeverRespond respond(http.httpIterator);
		respond.protocol = http.protocol;
		respond.state = "200";
		respond.statecode = respond.StateMap["200"];
		Hospital_Database db;
		if (db.ModifyData(http.urlcontent) == 0) {
			respond.AddContent("mod ok");
		}
		else
		{
			respond.AddContent("error!");
		}
		int res = respond.Send();
		return res;
	}
	int Router_hospitaldel(SeverRequest& http) {
		SeverRespond respond(http.httpIterator);
		respond.protocol = http.protocol;
		respond.state = "200";
		respond.statecode = respond.StateMap["200"];
		Hospital_Database db;
		if (db.DeleteData(http.urlcontent) == 0) {
			respond.AddContent("deleted ok");
		}
		else
		{
			respond.AddContent("error!");
		}
		int res = respond.Send();
		return res;
	}
};


class SeverManagerRouter
{
public:
	int next(SeverRequest& http, std::string cookie) {
		if (http.url == "/drugin" && http.method == "GET")//权限进入
		{
			return Router_drugin(http);
		}
		if (http.url == "/drugin_add" && http.method == "POST")
		{
			return Router_drugin_add(http);
		}
		if (http.url == "/drugout" && http.method == "GET")
		{
			return Router_drugout(http);
		}
		if (http.url == "/drugout_add" && http.method == "POST")
		{
			return Router_drugout_add(http);
		}
		if (http.url == "/drugreservation" && http.method == "GET")
		{
			return Router_drugreservation(http);
		}
		if (http.url == "/drugreservation/drugreservation_in" && http.method == "GET")
		{
			return Router_drugreservation_in(http);
		}
		if (http.url == "/drugreservation/drugreservation_out" && http.method == "GET")
		{
			return Router_drugreservation_out(http);
		}
		if (http.url == "/drugreservation/drugreservation_num" && http.method == "GET")
		{
			return Router_drugreservation_num(http);
		}
		//下一次开发从这里开始,开始具体业务处理   2023/10/26
		//承接判断开始，会传入http, cookie，以无路径404结束
		SeverHospital drug;
		drug.next(http, cookie);
	}
private:
	int Router_drugin(SeverRequest& http) {
		SeverRespond respond(http.httpIterator);
		respond.protocol = http.protocol;
		respond.state = "200";
		respond.statecode = respond.StateMap["200"];
		SeverFile file;
		file.SeverFile_open("form/drugin.html");
		respond.AddContent(file.GetContent());
		file.SeverFile_close();
		int res = respond.Send();
		return res;
	}
	int Router_drugin_add(SeverRequest& http) {
		SeverRespond respond(http.httpIterator);
		respond.protocol = http.protocol;
		respond.state = "200";
		respond.statecode = respond.StateMap["200"];
		Drugin_Database db;
		if (db.AddData(http.urlcontent) == 0) {
			respond.AddContent("inputin");
		}
		else
		{
			respond.AddContent("error!");
		}
		int res = respond.Send();
		return res;
	}
	int Router_drugout(SeverRequest& http) {
		SeverRespond respond(http.httpIterator);
		respond.protocol = http.protocol;
		respond.state = "200";
		respond.statecode = respond.StateMap["200"];
		SeverFile file;
		file.SeverFile_open("form/drugout.html");
		respond.AddContent(file.GetContent());
		file.SeverFile_close();
		int res = respond.Send();
		return res;
	}
	int Router_drugout_add(SeverRequest& http) {
		SeverRespond respond(http.httpIterator);
		respond.protocol = http.protocol;
		respond.state = "200";
		respond.statecode = respond.StateMap["200"];
		Drugout_Database db;
		if (db.AddData(http.urlcontent) == 0) {
			respond.AddContent("inputin");
		}
		else
		{
			respond.AddContent("error!");
		}
		int res = respond.Send();
		return res;
	}
	int Router_drugreservation(SeverRequest& http) {
		SeverRespond respond(http.httpIterator);
		respond.protocol = http.protocol;
		respond.state = "200";
		respond.statecode = respond.StateMap["200"];
		SeverFile file;
		file.SeverFile_open("form/drugreservation.html");
		respond.AddContent(file.GetContent());
		file.SeverFile_close();
		int res = respond.Send();
		return res;
	}
	int Router_drugreservation_in(SeverRequest& http) {
		SeverRespond respond(http.httpIterator);
		respond.protocol = http.protocol;
		respond.state = "200";
		respond.statecode = respond.StateMap["200"];
		SeverFile file;
		file.SeverFile_open("form/drugreservation_in.html");
		std::string content = file.GetContent();
		std::string mark = "<!--insert_mark-->";
		int pos = content.find(mark);
		pos += mark.size();
		Drugin_Database db;
		std::string addurl = db.GetData();
		content.insert(pos, addurl);
		respond.AddContent(content);
		file.SeverFile_close();
		int res = respond.Send();
		return res;
	}
	int Router_drugreservation_out(SeverRequest& http) {
		SeverRespond respond(http.httpIterator);
		respond.protocol = http.protocol;
		respond.state = "200";
		respond.statecode = respond.StateMap["200"];
		SeverFile file;
		file.SeverFile_open("form/drugreservation_out.html");
		std::string content = file.GetContent();
		std::string mark = "<!--insert_mark-->";
		int pos = content.find(mark);
		pos += mark.size();
		Drugout_Database db;
		std::string addurl = db.GetData();
		content.insert(pos, addurl);
		respond.AddContent(content);
		file.SeverFile_close();
		int res = respond.Send();
		return res;
	}

	int Router_drugreservation_num(SeverRequest& http) {
		SeverRespond respond(http.httpIterator);
		respond.protocol = http.protocol;
		respond.state = "200";
		respond.statecode = respond.StateMap["200"];
		SeverFile file;
		file.SeverFile_open("form/drugreservation_num.html");
		std::string content = file.GetContent();
		std::string mark = "<!--insert_mark-->";
		int pos = content.find(mark);
		pos += mark.size();
		Drugnum_Database db;
		std::string addurl = db.GetData();
		content.insert(pos, addurl);
		respond.AddContent(content);
		file.SeverFile_close();
		int res = respond.Send();
		return res;
	}
};



class SeverNurseRouter
{
public:
	int next(SeverRequest& http, std::string cookie) {
		if (http.url == "/nurseschedule" && http.method == "GET")//权限进入
		{
			return Router_nurseschedule(http);
		}
		else if (http.url == "/nurseschedule_add" && http.method == "POST")
		{
			return Router_nurseschedule_add(http);
		}
		else if (http.url == "/nurseschedule_delete" && http.method == "POST")
		{
			return Router_nurseschedule_delete(http);
		}
		//下一次开发从这里开始,开始具体业务处理   2023/10/26
		//承接判断开始，会传入http, cookie，以无路径404结束
		SeverManagerRouter drug;
		drug.next(http, cookie);
	}
private:
	int Router_nurseschedule(SeverRequest& http) {
		SeverRespond respond(http.httpIterator);
		respond.protocol = http.protocol;
		respond.state = "200";
		respond.statecode = respond.StateMap["200"];
		SeverFile file;
		file.SeverFile_open("form/nurseschedule.html");
		std::string content = file.GetContent();
		std::string mark = "<!--insert_mark-->";
		int pos = content.find(mark);
		pos += mark.size();
		Nurseschedule_Database db;
		std::string addurl = db.GetData();
		content.insert(pos, addurl);
		respond.AddContent(content);
		file.SeverFile_close();
		int res = respond.Send();
		return res;
	}
	int Router_nurseschedule_add(SeverRequest& http) {
		SeverRespond respond(http.httpIterator); int res;
		respond.protocol = http.protocol;
		Nurseschedule_Database db;
		if (db.AddData(http.urlcontent) == 0) {
			respond.state = "301";
			respond.statecode = respond.StateMap["301"];
			respond.AddHeader({ "Location","/nurseschedule" });
			res = respond.Send();
		}
		else
		{
			respond.state = "200";
			respond.statecode = respond.StateMap["200"];
			respond.AddContent("not correct!");
			res = respond.Send();
		}
		return res;
	}
	int Router_nurseschedule_delete(SeverRequest& http) {
		SeverRespond respond(http.httpIterator); int res;
		respond.protocol = http.protocol;
		Nurseschedule_Database db;
		if (db.DeleteData(http.urlcontent) == 0) {
			respond.state = "301";
			respond.statecode = respond.StateMap["301"];
			respond.AddHeader({ "Location","/nurseschedule" });
			res = respond.Send();
		}
		else
		{
			respond.state = "200";
			respond.statecode = respond.StateMap["200"];
			respond.AddContent("not correct!");
			res = respond.Send();
		}
		return res;
	}
};










class SeverDocterRouter
{
public:
	int next(SeverRequest& http, std::string cookie) {
		if (http.url == "/doctorschedule" && http.method == "GET"&& (cookie=="doctor"|| cookie == "root"))//权限进入
		{
			return Router_doctorschedule(http);
		}
		else if (http.url == "/doctorschedule_add" && http.method == "POST")
		{
			return Router_doctorschedule_add(http);
		}
		else if (http.url == "/doctorschedule_delete" && http.method == "POST")
		{
			return Router_doctorschedule_delete(http);
		}
		//下一次开发从这里开始,开始具体业务处理   2023/10/26
		//承接判断开始，会传入http, cookie，以无路径404结束
		SeverNurseRouter nurseschedule;
		nurseschedule.next(http, cookie);
	}
private:
	int Router_doctorschedule(SeverRequest& http) {
		SeverRespond respond(http.httpIterator);
		respond.protocol = http.protocol;
		respond.state = "200";
		respond.statecode = respond.StateMap["200"];
		SeverFile file;
		file.SeverFile_open("form/doctorschedule.html");
		std::string content = file.GetContent();
		std::string mark = "<!--insert_mark-->";
		int pos = content.find(mark);
		pos += mark.size();
		Doctorschedule_Database db;
		std::string addurl = db.GetData();
		content.insert(pos, addurl);
		respond.AddContent(content);
		file.SeverFile_close();
		int res = respond.Send();
		return res;
	}
	int Router_doctorschedule_add(SeverRequest& http) {
		SeverRespond respond(http.httpIterator); int res;
		respond.protocol = http.protocol;
		Doctorschedule_Database db;
		if (db.AddData(http.urlcontent) == 0) {
			respond.state = "301";
			respond.statecode = respond.StateMap["301"];
			respond.AddHeader({ "Location","/doctorschedule" });
			res = respond.Send();
		}
		else
		{
			respond.state = "200";
			respond.statecode = respond.StateMap["200"];
			respond.AddContent("not correct!");
			res = respond.Send();
		}
		return res;
	}
	int Router_doctorschedule_delete(SeverRequest& http) {
		SeverRespond respond(http.httpIterator); int res;
		respond.protocol = http.protocol;
		Doctorschedule_Database db;
		if (db.DeleteData(http.urlcontent) == 0) {
			respond.state = "301";
			respond.statecode = respond.StateMap["301"];
			respond.AddHeader({ "Location","/doctorschedule" });
			res = respond.Send();
		}
		else
		{
			respond.state = "200";
			respond.statecode = respond.StateMap["200"];
			respond.AddContent("not correct!");
			res = respond.Send();
		}
		return res;
	}
};










//进行登录，权限管理
class SeverLoginRouter
{
public:
	const int Cookietime = 50000;//
	static std::list<std::tuple<std::string, std::string,int>>  Cookies; //存储登录进来的Cookie,{ authority ,Session,Cookietime}
    static std::string rand_str_(const int len=8)  /*参数为字符串的长度 ,32为不重复*/
	{
		srand(time(NULL));
		std::string str; char c; static unsigned int id = 0;
		for (int idx = 0; idx < len; idx++)
		{
			c = '0' + rand() % 61;
			str.push_back(c);
		}
		str.append(std::to_string(id));
		id++;
		return str;
	}
	std::string in_Cookies(SeverRequest& http) { //判断表里是否含有cookie ,返回权限
		std::string cookie = http.header_["Cookie"];
		if (cookie.size() == 0) {
			 return "NULL";
		}
		else
		{
			for (auto i = Cookies.begin(); i!=Cookies.end();i++)
			{
				std::string temp = std::get<1>(*i);
				if (temp == cookie) {
					return std::get<0>(*i);
				}
			}
			return "NULL";
		}
	}

	bool  remove_Cookies(SeverRequest& http) {            //清除Cookies
		std::string cookie = http.header_["Cookie"];
		if (cookie.size() == 0) {
			return 0;
		}
		else
		{
			for (auto i = Cookies.begin(); i != Cookies.end(); i++)
			{
				std::string temp = std::get<1>(*i);
				if (temp == cookie) {
					Cookies.erase(i);
					return 1;
				}
			}
			return 0;
		}
	}

	int respond(SeverRequest& http) {                                //路由选择响应
		std::cout << http.url << " " << http.method << "\n";
		http.printurlcontent();
		std::string cookie = in_Cookies(http);                            //权限信息获取
		if(http.url == "/" && http.method == "GET") {
			return Router_redirect(http);
		}
		else if (http.url == "/login" && http.method == "GET") {
			return Router_login(http);
		}
		else if(http.url == "/login_pwd" && http.method == "GET")
		{
			return Router_login_pwd(http);
		}
		else if (http.url == "/login" && http.method == "POST") {
			//if (cookie != "NULL") {
			//	remove_Cookies(http);
			//}
			return Router_login_form(http);
		}
		else if (http.url == "/login_pwd" && http.method == "POST")
		{
			return Router_login_pwd_form(http, cookie);
		}
		else if(http.url == "/index" && http.method == "GET" && cookie!="NULL")
		{
			return Router_index(http, cookie);
		}
		//下一次开发从这里开始,开始具体业务处理   2023/10/24
		//承接判断开始，会传入http, cookie，以无路径404结束

		SeverDocterRouter docterRouter;
		docterRouter.next(http, cookie);
	}
private:
	int Router_redirect(SeverRequest& http) {         //重定向首页路由
		SeverRespond respond(http.httpIterator);
		respond.protocol = http.protocol;
		respond.state = "301";
		respond.statecode = respond.StateMap["301"];
		respond.AddHeader({ "Location","/login" });
		int res = respond.Send();
		return res;
	}

	int Router_index(SeverRequest& http,std::string cookie) {                    //发送主页 GET
		SeverRespond respond(http.httpIterator);
		respond.protocol = "HTTP/1.1";
		respond.state = "200";
		respond.statecode = respond.StateMap["200"];
		SeverFile file;
		file.SeverFile_open("form/index.html");
		std::string content = file.GetContent();
		std::string mark = "<!--insert_mark-->";
		int pos = content.find(mark);
		pos += mark.size();
		std::string addurl = u8"<h2>权限:"+ cookie + u8"</h2>" + INDEX_ROOT_HTML;
		content.insert(pos, addurl);
		respond.AddContent(content);
		file.SeverFile_close();
		int res = respond.Send();
		return res;
	}

	int Router_login(SeverRequest& http) {                  //发送登录页面 GET
		SeverRespond respond(http.httpIterator);
		respond.protocol = http.protocol;
		respond.state = "200";
		respond.statecode = respond.StateMap["200"];
		SeverFile file;
		file.SeverFile_open("form/login.html");
		respond.AddContent(file.GetContent());
		file.SeverFile_close();
		int res = respond.Send();
		return res;
	}

	int Router_login_form(SeverRequest& http) {                  //登录表单判断 POST,正确重定向进入主界面，
        SeverRespond respond(http.httpIterator);
		//http.print();
		Login_Database db;
		std::string authority=db.FindData(http.urlcontent);
		if (authority !="NULL") {
			respond.protocol = http.protocol;
			respond.state = "301";
			respond.statecode = respond.StateMap["301"];
			respond.AddHeader({ "Location","/index" });
			std::string Session; 
			Session += "name=";
			Session += rand_str_(8);
			Session += " Path=/";
			respond.AddHeader({ "Set-Cookie",Session });
			int res = respond.Send();
			Cookies.push_back({ authority ,Session,Cookietime});        //记录cookies
			return res;
		}
		else
		{
			respond.protocol = http.protocol;
			respond.state = "200";
			respond.statecode = respond.StateMap["200"];
			respond.AddContent("user not exist");
			int res = respond.Send();
			return res;
		}

	}

	int Router_login_pwd(SeverRequest& http) {                 //发送注册页面 GET
		SeverRespond respond(http.httpIterator);
		respond.protocol = http.protocol;
		respond.state = "200";
		respond.statecode = respond.StateMap["200"];
		SeverFile file;
		file.SeverFile_open("form/login_pwd.html");
		respond.AddContent(file.GetContent());
		file.SeverFile_close();
		int res = respond.Send();
		return res;
	}

	int Router_login_pwd_form(SeverRequest& http,std::string cookie) {   //处理注册 POST
		SeverRespond respond(http.httpIterator);
		respond.protocol = http.protocol;
		Login_Database db;
		if (cookie == "root") {                                 //root用户可注册所有
			if (db.AddData(http.urlcontent) == -1)
			{
				respond.state = "200";
				respond.statecode = respond.StateMap["200"];
				respond.AddContent("add user error,please rewrite");
				int res = respond.Send();
				return res;
			}
			respond.state = "301";
			respond.statecode = respond.StateMap["301"];
			respond.AddHeader({ "Location","/login" });
			int res = respond.Send();
			return res;
		}
		else{                                               //其他用户包括NULL 只能注册patient
			if (http.urlcontent["authority"]=="patient")
			{
				if (db.AddData(http.urlcontent) == -1)
				{
					respond.state = "200";
					respond.statecode = respond.StateMap["200"];
					respond.AddContent("add user error,please rewrite");
					int res = respond.Send();
					return res;
				}
				respond.state = "301";
				respond.statecode = respond.StateMap["301"];
				respond.AddHeader({ "Location","/login" });
				int res = respond.Send();
				return res;
			}
			else
			{
				respond.state = "200";
				respond.statecode = respond.StateMap["200"];
				respond.AddContent("no permission!");
				int res = respond.Send();
				return res;
			}
		}

	}

	int Router_else(SeverRequest& http) {
		SeverRespond respond(http.httpIterator);
		respond.protocol = "HTTP/1.1";
		respond.state = "404";
		respond.statecode = respond.StateMap["404"];
		//respond.AddHeader({ "type","image/x-icon" });
		//respond.AddContent("hello world");
		int res = respond.Send();
		return res;
	}

};

std::list<std::tuple<std::string, std::string, int>> SeverLoginRouter::Cookies;




#endif // !_SEVERROUTERS_H_
