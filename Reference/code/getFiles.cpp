#include "getFiles.h"


getFiles::getFiles()
{
}


getFiles::~getFiles()
{
}

void getFiles::getAllFiles(string path, vector<string>& files)
{
	// �ļ����
	long hFile = 0;
	// �ļ���Ϣ
	struct _finddata_t fileinfo;

	string p;

	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do {
			// �����ļ���ȫ·��
			files.push_back(p.assign(path).append("\\").append(fileinfo.name));

		} while (_findnext(hFile, &fileinfo) == 0); //Ѱ����һ�����ɹ�����0������-1

		_findclose(hFile);
	}
}

void getFiles::getAllFilesAndOthers(string path, vector<string>& files)
{
	//�ļ����
	long hFile = 0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
		do {
			if ((fileinfo.attrib & _A_SUBDIR)) { //�Ƚ��ļ������Ƿ����ļ���
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
					files.push_back(p.assign(path).append("\\").append(fileinfo.name));
					//�ݹ�����
					getAllFiles(p.assign(path).append("\\").append(fileinfo.name), files);
				}
			}
			else {
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			}
		} while (_findnext(hFile, &fileinfo) == 0); //Ѱ����һ�����ɹ�����0������-1
		_findclose(hFile);
	}
}

void getFiles::getSpecialTypeFiles(string path, vector<string>& files, string fileType)
{
	m_vecStrCurrentModel.clear();
	// �ļ����
	long hFile = 0;
	// �ļ���Ϣ
	struct _finddata_t fileinfo;

	string p;

	if ((hFile = _findfirst(p.assign(path).append("\\*" + fileType).c_str(), &fileinfo)) != -1) {
		do {
			// �����ļ���ȫ·��
			files.push_back(p.assign(path).append("\\").append(fileinfo.name));
			m_vecStrCurrentModel.push_back(fileinfo.name);

		} while (_findnext(hFile, &fileinfo) == 0); //Ѱ����һ�����ɹ�����0������-1

		_findclose(hFile);
	}
}


std::vector<std::string> getFiles::currentModel()
{
	return m_vecStrCurrentModel;
}


