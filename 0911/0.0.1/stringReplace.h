#pragma once
#include <vs_all.h>
class stringReplace
{
public:
	stringReplace();
	~stringReplace();
	//�ֲ��滻
	string&   replace_all(string&   str, const   string&   old_value, const   string&   new_value);
	//ȫ���滻
	string&   replace_all_distinct(string&   str, const   string&   old_value, const   string&   new_value);

};

