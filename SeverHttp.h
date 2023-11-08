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


//http����ͷ����
class SeverRequest
{
public:
	std::string method;//����
	std::string url;//url��ַ
	std::string protocol;//Э��
	std::map<std::string, std::string> header_;//ͷ�����Զ�
	std::string content;//�������204800�ֽڣ�δ����
	std::map<std::string, std::string> urlcontent; //url��ʽ����
	SeverRequest(NetStreamSSL handle);            //����NetStreamSSL������
	SeverRequest()=default;//��������ʹ��
	NetStreamSSL httpIterator;//NetStreamSSL��������
	~SeverRequest() = default;
	int analysis();              //����httpͷ
	void print();                //��ӡhttpͷ
	void printurlcontent();      //��ӡurl��ʽ����
private:
	char input_char = 0;
	int res = 0;
	//�ݹ��½���
	int in_methon();          
	int in_url();
	int in_protocol();
	int in_Header();
	int in_Header_s();
	int in_content();
	int geturlcontent();//��url��ʽ����ת����map
	std::string in_Fieldname();
	std::string in_id();
	std::string urlEncode(std::string str);//url����
	std::string urlDecode(std::string str);//url��ʽ����
	void popspace();      //����ȥ������ǰ��ĵ�һ���ո�             
};

//http����ͷ��Ӧ
class SeverRespond
{
public:
	SeverRespond(NetStreamSSL handle);
	virtual ~SeverRespond()=default;
	void AddHeader(std::pair<std::string, std::string> module);//����Զ�����Ӧͷ���Ḳ��map
	void AddContent(std::string content);      //�����Ӧ����
	int Send();                                //������Ӧ
	std::string protocol;   //Э��
	std::string statecode;  //״̬��
	std::string state;       //״̬
	std::map<std::string, std::string> StateMap
	{
		{"200" ,  "OK"},   //�ͻ�������ɹ�
		{"400" ,  "Bad Request"},   //���ڿͻ����������﷨���󣬲��ܱ�����������⡣
		{"401",   "Unauthonzed"},  //����δ����Ȩ�����״̬��������WWW - Authenticate��ͷ��һ��ʹ��
		{"403",   "Forbidden"},  //�������յ����󣬵��Ǿܾ��ṩ���񡣷�����ͨ��������Ӧ�����и������ṩ�����ԭ��
		{"404",   "Not Found"},  //�������Դ�����ڣ����磬�����˴����URL��
		{"500",   "Internal Server Error"}, //��������������Ԥ�ڵĴ��󣬵����޷���ɿͻ��˵�����
		{"503",   "Service Unavailable"},  //��������ǰ���ܹ�����ͻ��˵�������һ��ʱ��֮�󣬷��������ܻ�ָ�������
		{"206",   "partical content"},
		{"301", "Moved Permanently"},//�����ض���
		{"307", "Temporary Redirect"}//��ʱ�ض���
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
			if (this->method == "POST") {//�г�ʱ�ɲ����ж�(�бȽϺ�)
				if (in_content() > 0) {
					geturlcontent();//���url��content
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