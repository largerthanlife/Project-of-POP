#ifndef _Create_H_
#define _Create_H_
#include "base.h"

class Create: public Base
{
public:
	Create();
	~Create();
	int Match(string &);			// 匹配输入表达式
	bool Create_from_file();	// 第一种 CREATE 操作	
	bool Create_to_file();				// 第二种 CREATE 操作
	void SortMatched(string &, int);		// 整理所匹配的变量
	const int CreateToFile;			// 操作号 1
	const int CreateFromFile;		// 操作号 2
private:
	void Print();					// 打印一些关键变量
	const char *pattern_to;			// 正则表达式 1
	const char *pattern_from;		// 正则表达式 2	
};


#endif
