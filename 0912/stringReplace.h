#pragma once
#include <vs_all.h>
class stringReplace
{
public:
	stringReplace();
	~stringReplace();
	//局部替换
	string&   replace_all(string&   str, const   string&   old_value, const   string&   new_value);
	//全部替换
	string&   replace_all_distinct(string&   str, const   string&   old_value, const   string&   new_value);

};

