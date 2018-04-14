#ifndef _FILE_H_
#define _FILE_H_

#include "goods.h"
#include <vector>
#include <string>
#include <map>

using std::vector;
using std::string;
using std::map;

class Goods;

class File
{
private:
public:
	string filename;
	void readfile(vector<Goods> &);	//读取库存文件
	void readfile(map<string, string> &, vector<string> &);	//读取用户数据文件并保存
	void readsoldoutfile();							//读取售货清单并直接显示
	void writefile(vector<Goods> &);				//更新购物车文件
	void writefile(map<string, string> &);			//更新用户数据文件
	void writesoldoutfile(vector<Goods> &, string);	//更新售货清单文件
	void creatfile(string);		//给新用户创建对应购物车文件
};

#endif