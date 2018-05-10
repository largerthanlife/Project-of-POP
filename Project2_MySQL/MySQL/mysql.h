#ifndef _MYSQL_H_
#define _MYSQL_H_

#include <iostream>
#include <cstdio>
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <sstream>

#include "create.h"
#include "drop.h"
#include "insert.h"
#include "delete.h"
#include "update.h"
#include "select.h"

using std::cout;
using std::endl;
using std::cin;
using std::string;
using std::map;
using std::vector;
using std::pair;
using std::stringstream;
using std::ifstream;
using std::ofstream;
using std::ios;

#define STRBUF 1000	//	输入的字符串长度

class MySQL
{
public:
	MySQL();
	~MySQL();

	void mysql();
	int  Operator_ID(string &);
	bool Analyse(string &);
	bool Register(Table *);
	bool ReadConfigFile();
	bool WriteConfigFile();
	bool DropTable(string &, bool);
	void ListTable();
	bool check(string &, string &);
	bool check(string);
	bool ShowTable(string);
	void ShowHelp();
	bool READ();
	string paired_filename(string );
	
	Create *create_new;
	Drop *drop_new;
	Insert *insert_new;
	Delete *delete_new;
	Update *update_new;
	Select *select_new;

private:
	string mapfile;	//	"table-file" map filename
	map<string, string> table_map;
	vector<Table> table_list;
};

#endif
