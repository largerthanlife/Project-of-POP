#ifndef _SELECT_H_
#define _SELECT_H_
#include "base.h"

class Select:public Base
{
public:
	Select();
	~Select();
	void Print();
	void SortMatched(string&, int);
	int Match(string&);
	bool Match_more(string , const char*, regmatch_t*);
	bool Select_Part(vector<Table>&);
	bool Select_Where(vector<Table>&);
	bool Select_Distinct(vector<Table>&);
	bool Select_Order(vector<Table>&);
	bool Sort_id_list(vector<string>&, vector<string>&);
	const int All;
	const int Part;
	const int Distinct;
	const int Order;
	const int Part_where;
	const int To;
	string select_filename;
	Table select_table;

private:
	const char* select_all;
	const char* select_part;
	const char* select_distinct;
	const char* select_order;
	const char* select_part_where;
	const char* select_to;
	vector<string> part_id_list;
	vector<string> order_id_list;
	vector<int> select_row;
	string select_id;
	string select_item;

	string ordermethod; 
	bool ASCorNot;
};


#endif