#ifndef _DELETE_H_
#define _DELETE_H_

#include "base.h"


class Delete:public Base
{
public:
	Delete();
	~Delete();
	void Print();
	void SortMatched(string &, int);
	int Match(string &);
	bool Delete_All(vector<Table> &);
	bool Delete_Part(vector<Table> &);
	const int Part;
	const int All;
private:
	const char * delete_part;
	const char * delete_all;
	vector<int> delete_num;
	string delete_id;
	string delete_item;
};



#endif