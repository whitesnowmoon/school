#ifndef _SEVERHTTP_H_
#define _SEVERHTTP_H_

#include<iostream>
#include<map>
#include<string>
#include<vector>
#include<stdio.h>
#include"SeverSSL.h"


#define SEVER_REQUEST_OK_ 1
#define SEVER_REQUEST_ERROR_ -1
#define SEVER_NETWORK_ERROR_ -2

#define SEVER_REQUEST_ON_ 2
#define SEVER_REQUEST_READLY_ 3
#define SEVER_REQUEST_END_ 4


//http请求头解析
class SeverRequest
{
public:
	std::string method;//方法
	std::string url;//url地址
	std::string protocol;//协议
	std::map<std::string, std::string> header_;//头部属性对
	std::string content;//内容最多204800字节，未改良
	std::map<std::string, std::string> urlcontent; //url格式内容
	SeverRequest(NetStreamSSL handle);            //输入NetStreamSSL流解析
	SeverRequest()=default;//用来复制使用
	NetStreamSSL httpIterator;//NetStreamSSL流上下文
	~SeverRequest() = default;
	int analysis();              //解析http头
	void print();                //打印http头
	void printurlcontent();      //打印url格式内容
private:
	char input_char = 0;
	int res = 0;
	//递归下降法
	int in_methon();          
	int in_url();
	int in_protocol();
	int in_Header();
	int in_Header_s();
	int in_content();
	int geturlcontent();//将url格式内容转化成map
	std::string in_Fieldname();
	std::string in_id();
	std::string urlEncode(std::string str);//url编码
	std::string urlDecode(std::string str);//url格式解析
	void popspace();      //补丁去除属性前面的第一个空格             
};

//http请求头响应
class SeverRespond
{
public:
	SeverRespond(NetStreamSSL handle);
	virtual ~SeverRespond()=default;
	void AddHeader(std::pair<std::string, std::string> module);//添加自定义响应头，会覆盖map
	void AddContent(std::string content);      //添加响应内容
	int Send();                                //发送响应
	std::string protocol;   //协议
	std::string statecode;  //状态码
	std::string state;       //状态
	std::map<std::string, std::string> StateMap
	{
		{"200" ,  "OK"},   //客户端请求成功
		{"400" ,  "Bad Request"},   //由于客户端请求有语法错误，不能被服务器所理解。
		{"401",   "Unauthonzed"},  //请求未经授权。这个状态代码必须和WWW - Authenticate报头域一起使用
		{"403",   "Forbidden"},  //服务器收到请求，但是拒绝提供服务。服务器通常会在响应正文中给出不提供服务的原因
		{"404",   "Not Found"},  //请求的资源不存在，例如，输入了错误的URL。
		{"500",   "Internal Server Error"}, //服务器发生不可预期的错误，导致无法完成客户端的请求。
		{"503",   "Service Unavailable"},  //服务器当前不能够处理客户端的请求，在一段时间之后，服务器可能会恢复正常。
		{"206",   "partical content"},
		{"301", "Moved Permanently"},//永久重定向
		{"307", "Temporary Redirect"}//临时重定向
	};
	std::map<std::string, std::string> Response_Type
	{
		{".html","text/html"},
		{".jpg","image/jpeg"},
		{".png","image/png"},
		{".css","text/css"},
		{".js","application/x-javascript"}
	};
private:
	std::string ResponseHeader;
	std::string content;
	std::map<std::string, std::string> headers;
	NetStreamSSL httpIterator;
};

SeverRespond::SeverRespond(NetStreamSSL handle):httpIterator(handle) {

}
void SeverRespond::AddContent(std::string content) {
	this->content = content;
}

void SeverRespond::AddHeader(std::pair<std::string, std::string> module) {
	headers.insert(module);
}
int SeverRespond::Send() {
	int sum = 0;
	std::string main;
	main += this->protocol;
	main += " ";
	main += this->state;
	main += " ";
	main += this->statecode;
	main += "\r\n";
	for (auto i = headers.begin(); i != headers.end(); i++)
	{
		main += i->first;
		main += ": ";
		main += i->second;
		main += "\r\n";
	}
	main += "\r\n";
	sum +=httpIterator._write_((char*)main.c_str(), main.size());
	sum += httpIterator._write_((char*)content.c_str(), content.size());
	return sum;
}


/***********************************************************************/
SeverRequest::SeverRequest(NetStreamSSL handle) :httpIterator(handle){
}
int SeverRequest::analysis() {
	res = httpIterator._getchar_(input_char);
	if (res < 0)return SEVER_NETWORK_ERROR_;
	if (in_methon() <0)return SEVER_REQUEST_ERROR_;
	if (in_url() < 0)return SEVER_REQUEST_ERROR_;
	if (in_protocol() < 0)return SEVER_REQUEST_ERROR_;
	if (in_Header() < 0)return SEVER_REQUEST_ERROR_;
	if (input_char == 0x0D)
	{
		res = httpIterator._getchar_(input_char);
		if (res < 0)return SEVER_NETWORK_ERROR_;
		if (input_char == 0x0A) {
			this->url=urlDecode(this->url);
			popspace();
			if (this->method == "POST") {//有超时可不用判断(有比较好)
				if (in_content() > 0) {
					geturlcontent();//获得url的content
				}
			}
			return SEVER_REQUEST_OK_;
		}
		else
		{
			return SEVER_REQUEST_ERROR_;
		}
	}
	return SEVER_REQUEST_ERROR_;
}



void SeverRequest::print() {
	std::cout << "method:" << method << "\n";
	std::cout << "url:" << url << "\n";
	std::cout << "protocol:" << protocol << "\n";
	for (auto iter = header_.rbegin(); iter != header_.rend(); iter++) {
		std::cout << iter->first << "   " << iter->second << std::endl;
	}
	std::cout << "content:" << "\n"<<content<<"\n";
}

int SeverRequest::in_methon() {
	int state = SEVER_REQUEST_ON_, times = 0;
	while (state != SEVER_REQUEST_END_)
	{
		if (input_char == 0x20) {
			state = SEVER_REQUEST_END_;
			res = httpIterator._getchar_(input_char);
			if (res < 0)return SEVER_NETWORK_ERROR_;
		}
		else
		{
			method.push_back(input_char);
			res = httpIterator._getchar_(input_char);
			if (res < 0)return SEVER_NETWORK_ERROR_;
			times++; if (times > 100)return SEVER_REQUEST_ERROR_;
		}
	}
	return SEVER_REQUEST_OK_;
}

int SeverRequest::in_url() {
	int state = SEVER_REQUEST_ON_, times = 0;
	while (state != SEVER_REQUEST_END_)
	{
		if (input_char == 0x20) {
			state = SEVER_REQUEST_END_;
			res = httpIterator._getchar_(input_char);
			if (res < 0)return SEVER_NETWORK_ERROR_;
		}
		else
		{
			url.push_back(input_char);
			res = httpIterator._getchar_(input_char);
			if (res < 0)return SEVER_NETWORK_ERROR_;
			times++; if (times > 1000)return SEVER_REQUEST_ERROR_;
		}
	}
	return SEVER_REQUEST_OK_;
}

int SeverRequest::in_protocol() {
	int state = SEVER_REQUEST_ON_, times = 0;
	while (state != SEVER_REQUEST_END_)
	{
		if (input_char == 0x0D) {
			state = SEVER_REQUEST_READLY_;
			res = httpIterator._getchar_(input_char);
			if (res < 0)return SEVER_NETWORK_ERROR_;
		}
		else if (input_char == 0x0A && state == SEVER_REQUEST_READLY_) {
			state = SEVER_REQUEST_END_;
			res = httpIterator._getchar_(input_char);
			if (res < 0)return SEVER_NETWORK_ERROR_;
		}
		else
		{
			state = SEVER_REQUEST_ON_;
			protocol.push_back(input_char);
			res = httpIterator._getchar_(input_char);
			if (res < 0)return SEVER_NETWORK_ERROR_;
			times++; if (times > 50)return SEVER_REQUEST_ERROR_;
		}
	}
	return SEVER_REQUEST_OK_;
}

std::string SeverRequest::in_Fieldname() {
	int state = SEVER_REQUEST_ON_, times = 0; std::string buff;
	while (state != SEVER_REQUEST_END_)
	{
		if (input_char == 0x3A) {
			state = SEVER_REQUEST_END_;
			res = httpIterator._getchar_(input_char);
			if (res < 0)return buff;
		}
		else
		{
			state = SEVER_REQUEST_ON_;
			buff.push_back(input_char);
			res = httpIterator._getchar_(input_char);
			if (res < 0)return buff;
			times++; if (times > 10240)return buff;
		}
	}
	return buff;
}

std::string SeverRequest::in_id() {
	int state = SEVER_REQUEST_ON_, times = 0; std::string buff;
	while (state != SEVER_REQUEST_END_)
	{
		if (input_char == 0x0D) {
			state = SEVER_REQUEST_READLY_;
			res = httpIterator._getchar_(input_char);
			if (res < 0)return buff;
		}
		else if (input_char == 0x0A && state == SEVER_REQUEST_READLY_) {
			state = SEVER_REQUEST_END_;
			res = httpIterator._getchar_(input_char);
			if (res < 0)return buff;
		}
		else
		{
			state = SEVER_REQUEST_ON_;
			buff.push_back(input_char);
			res = httpIterator._getchar_(input_char);
			if (res < 0)return buff;
			times++; if (times > 10240)return buff;
		}
	}
	return buff;
}

std::string SeverRequest::urlEncode(std::string str) {
	std::string new_str = "";
	char c;
	int ic;
	const char* chars = str.c_str();
	char bufHex[10];
	int len = strlen(chars);

	for (int i = 0; i < len; i++) {
		c = chars[i];
		ic = c;
		if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') new_str += c;
		else {
			sprintf(bufHex, "%X", c);
			if (ic < 16)
				new_str += "%0";
			else
				new_str += "%";
			new_str += bufHex;
		}
	}
	return new_str;
}

std::string SeverRequest::urlDecode(std::string str) {
	std::string ret;
	char ch;
	int i, ii, len = str.length();

	for (i = 0; i < len; i++) {
		if (str[i] != '%') {
			if (str[i] == '+')
				ret += ' ';
			else
				ret += str[i];
		}
		else {
			sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
			ch = static_cast<char>(ii);
			ret += ch;
			i = i + 2;
		}
	}
	return ret;
}

void SeverRequest::popspace() {
	for (auto pos = header_.begin(); pos != header_.end(); pos++) {
		pos->second.erase(0, 1);
	}
}

int SeverRequest::in_Header() {
	if (in_Header_s() == SEVER_REQUEST_ERROR_)
		return SEVER_REQUEST_ERROR_;
	return SEVER_REQUEST_OK_;
}

int SeverRequest::in_Header_s() {
	int state = SEVER_REQUEST_ON_;
	if (input_char == 0x0D)
	{
		state = SEVER_REQUEST_READLY_;
		char tempc = 0;
		tempc = httpIterator.oldone;
		if (tempc == 0x0A && state == SEVER_REQUEST_READLY_) {
			state = SEVER_REQUEST_END_;
			return SEVER_REQUEST_OK_;
		}
		else
		{
			return SEVER_REQUEST_ERROR_;
		}
	}
	else
	{
		if (input_char == 0x20) {
			res = httpIterator._getchar_(input_char);
			if (res < 0)return SEVER_NETWORK_ERROR_;
		}
		std::string key_word = in_Fieldname();
		if (key_word.size() > 10240)return SEVER_REQUEST_ERROR_;
		std::string key_id = in_id();
		if (key_id.size() > 10240)return SEVER_REQUEST_ERROR_;
		header_.emplace(key_word, key_id);
		if (in_Header() == SEVER_REQUEST_ERROR_)return SEVER_REQUEST_ERROR_;
		return SEVER_REQUEST_OK_;
	}
}

int SeverRequest::in_content() {
	char buff[2048];
	int num = httpIterator._read_(buff, 2028);
	while (num>0) {
		this->content.append(buff, num);
		num = httpIterator._read_(buff, 2048);
	}
	return this->content.size();
}

int SeverRequest::geturlcontent() {
	int fristindex = 0,lastindex=0;
	while (lastindex != std::string::npos)
	{
		lastindex = this->content.find('=', fristindex);
		std::string key = this->content.substr(fristindex, lastindex - fristindex);
		fristindex = lastindex+1;
		lastindex = this->content.find('&', fristindex);
		std::string value = this->content.substr(fristindex, lastindex - fristindex);
		fristindex = lastindex+1;
		value=urlDecode(value);
		key = urlDecode(key);
		urlcontent.emplace(key, value);
	}

	return 0;
}

void SeverRequest::printurlcontent() {
	std::cout << "urlcontent:\n";
	for (auto iter = urlcontent.rbegin(); iter != urlcontent.rend(); iter++) {
		std::cout << iter->first << "   " << iter->second << std::endl;
	}
	std::cout << std::endl;
}


#endif // !_SEVERHTTP_H_