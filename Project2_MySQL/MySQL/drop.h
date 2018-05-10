#ifndef _DROP_H_
#define _DROP_H_
#include "base.h"

class Drop:public Base
{
public:
	Drop();
	~Drop();
	void Print();
	int Match(string &);
	void SortMatched(string &, int);
	bool DropIt();
	const int DROPID;
private:
	const char *pattern;
};

#endif