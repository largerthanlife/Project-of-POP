#ifndef _UPDATE_H_
#define _UPDATE_H_

#include "base.h"


class Update:public Base
{
public:
	Update();
	~Update();
	void Print();
	void SortMatched(string&, int);
	int Match(string&);	
	bool Update_All(vector<Table>&);
	bool Update_Part(vector<Table>&);
	const int All;
	const int Part;
	vector<string> id_update;
	vector<string> item_update;
	string optional_id;
	string optional_item;

private:
	const char* update_all;
	const char* update_part;
};


#endif