#pragma once
#include <io.h>
#include <string>
#include <vector>
#include <fstream>
using namespace std;
class getFiles
{
public:
	getFiles();
	~getFiles();
	//获得指定目录下的所有文件（不搜索子文件夹）
	void getAllFiles(string path, vector<string>& files);
	//获取指定目录下的所有文件（搜索子文件夹）
	void getAllFilesAndOthers(string path, vector<string>& files);
	//获取指定格式的文件(不搜索子文件夹）
	void getSpecialTypeFiles(string path, vector<string>& files, string fileType);
};

