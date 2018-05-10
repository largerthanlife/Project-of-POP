#ifndef _INSERT_H_
#define _INSERT_H_

#include "base.h"

class Insert:public Base
{
public:
	Insert();
	~Insert();
	void Print();
	void SortMatched(string &, int);
	int Match(string &);
	bool Match_more(string , const char*, regmatch_t*);
	bool Insert_to_all(vector<Table> &);
	bool Insert_to_part(vector<Table> &);
	const int Insert_ALL;
	const int Insert_PART;
 	vector<string> to_be_insert;
 	vector<string> to_be_insert_id;
private:
	const char *pattern_all;
	const char *pattern_part;
	int item_num; // number of VALUES (……)num; = to_be_insert.size()
	int id_num;	// = to_be_insert_id.size()
	string defaultstring;
};

#endif