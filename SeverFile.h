#ifndef _SEVERFILE_H_
#define _SEVERFILE_H_
#include<iostream>
#include<vector>
#include<fstream>

class SeverFile
{
public:
	bool SeverFile_open(std::string path) {
		std::string localpath = rootPath + path;
		std::cout << localpath << "\n";
		file.open(localpath, std::ios_base::in | std::ios_base::binary);
		if (file.is_open())
		{
			file.seekg(0, std::ios_base::end);
			std::streampos fileSize = file.tellg();
			file.seekg(0, std::ios_base::beg);
			this->length = fileSize;
			unsigned int index1 = localpath.find_last_of("/");
			unsigned int index2 = localpath.find_last_of("\\");
			this->name = localpath.substr(index1 < index2 ? index1 : index2);
			return true;
		}
		return false;
	}

	std::string GetContent() {
		int num = 0;std::string content;
		if (file.is_open()) {
			file.seekg(0, std::ios_base::end);
			std::streampos fileSize = file.tellg();
			file.seekg(0, std::ios_base::beg);
			char* buff = new char[this->length];
			file.read(buff, length);
			if (length > 0)
				content.append(buff, length);
			delete buff;
		}
		return content;
	}
	bool SeverFile_close() {
		this->file.close();
		if (!file.is_open()) {
			return true;
		}
		return false;
	}
	int length;                        //文件长度
	std::string name;					//文件名字
private:
	static std::string rootPath;
	std::fstream file;
};
std::string SeverFile::rootPath;
#endif