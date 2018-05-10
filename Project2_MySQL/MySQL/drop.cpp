#include "drop.h"
#include "mysql.h"

Drop::Drop():
	DROPID(1),
	pattern("\\s*DROP\\s+TABLE\\s+(\\S*)\\s*")
{
	table = new Table;
	//cout << "to drop table function" << endl;
}

Drop::~Drop()
{
	;
}

void Drop::Print()
{

}

int Drop::Match(string & input)
{
	if(MatchInput(input, pattern, matched))
	{
		SortMatched(input, DROPID);
		//Print();
		return DROPID;
	}
	else
		return ERROR;
}

void Drop::SortMatched(string & input, int flag)
{
	if(flag == DROPID)
	{
		const char *cinput = input.c_str();
		char temp[MATCHBUF];
		int len = matched[1].rm_eo - matched[1].rm_so;
		memset(temp, '\0', sizeof(temp));
		memcpy(temp, cinput+matched[1].rm_so, len);	
		table->tablename = string(temp);
	}
}

bool Drop::DropIt()
{
	;
	// return mySQL.DropTable(tablename);
}
