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
	//���ָ��Ŀ¼�µ������ļ������������ļ��У�
	void getAllFiles(string path, vector<string>& files);
	//��ȡָ��Ŀ¼�µ������ļ����������ļ��У�
	void getAllFilesAndOthers(string path, vector<string>& files);
	//��ȡָ����ʽ���ļ�(���������ļ��У�
	void getSpecialTypeFiles(string path, vector<string>& files, string fileType);
};

