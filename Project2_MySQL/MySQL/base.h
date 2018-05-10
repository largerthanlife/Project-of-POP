#ifndef _BASE_H_
#define _BASE_H_

#include <iostream>
#include <string>
#include <vector>
#include <memory.h>
#include <regex.h>
#include <cstring>
#include <cstdio>
#include <fstream>
#include <sstream>
#include <stddef.h>
#include <iomanip>
#include <algorithm>
#include <sstream>

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;
using std::ifstream;
using std::ofstream;
using std::stringstream;
using std::ios;

#define ERRBUF 100		//regerror的错误信息缓冲区长度
#define MATCHBUF 100	// 匹配串的长度
#define MAXMATCHNUM 50	//最多需要匹配的表达式个数

#define ERROR 		0
#define CREATETABLE 1
#define DROPTABLE  	2
#define INSERTINTO 	3
#define DELETE		4
#define UPDATE		5
#define SELECT		6
#define LIST		7
#define SHOW 		8
#define POSTFIX		".tmp"
#define DEFAULTSTRING "None"

#define WIDTH_1 8
#define WIDTH_2 4
#define OUTPUTFORMAT_1(FIELDWIDTH) setiosflags(ios::left) << std::setw(FIELDWIDTH) //定义输出格式
#define RESETOUTPUTFORMAT std::resetiosflags(ios::showbase)

class Table
{
public:
	Table();
	~Table();
	bool WriteFile();
	bool ShowData();
	void PrintHead();
	string tablename;
	string filename;
	vector<string> id_list;
	vector<vector<string>> data_list;
	vector<int> width;	//	每列的最大宽度， "id" 列的长度可以根据 row 来调
	int column;
	int row;	

};

class Base
{
public:
	Base(){table = new Table;}
	virtual ~Base(){;}
	virtual void Print()=0;	//	print information after this operation
	virtual void SortMatched(string&, int)=0;
	virtual int Match(string &) = 0;
	string& Getfilename(){return table->filename;}
	string& Gettablename(){return table->tablename;}
	Table *table;
	regmatch_t matched[MAXMATCHNUM];// 匹配信息的结构体		
};

bool MatchInput(string &, const char *,regmatch_t*);
bool checkinput(string &);
int bit(int num);
void TrimSpace(string &);
bool TrimALLSpace(string &);
void Split(string , string & , string & );
bool vector_compare(string&, string&, bool);
bool vector_exchange(vector<string> &, int, int);
bool Vector_not_Equal(vector<string> &, vector<string> &);
//////////////////////////////////////////////////////////////////////////////////////

#endif
