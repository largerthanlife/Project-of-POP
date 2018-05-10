#include <iostream>
#include <string>
#include <cstdio>
#include "mysql.h"

using std::cout;
using std::endl;
using std::cin;
using std::string;

bool checkinput(string & input);
MySQL mySQL;

int main()
{
	mySQL.ReadConfigFile();
	string input;
	char str[STRBUF];
	while(input != "quit")
	{
		cout << "~$ ";
		cin.getline(str, STRBUF);
		input = str;
		TrimSpace(input);
		if(input == "mySQL")
			mySQL.mysql();
		else if (input == "help")
			mySQL.ShowHelp();
		else if(checkinput(input))
			cout << "Wrong operation" << endl;
	}
	return 0;
}
