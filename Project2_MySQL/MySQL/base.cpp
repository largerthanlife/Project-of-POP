#include "base.h"


Table::Table()
{;}

Table::~Table()
{;}

bool Table::WriteFile()
{
	ofstream file(filename, ios::out);
	string temp;
	int line_num = 0;
	if(!file.is_open())
	{
		cout << "error: can not open " << filename << endl;
		return false;
	}
	else
	{
		for(vector<string>::iterator it = id_list.begin(); it != id_list.end(); it++)
		{
			file << *it << "\t";
		}
		file << endl;
		while(line_num < row)	////////////////////////////////////////*************??????????????把等于号去掉了 注意
		{
			for(vector<vector<string>>::iterator it = data_list.begin(); it != data_list.end() && data_list.size(); it++)
			{
				file << (*it)[line_num] << "\t" ;
			}
			file << endl;
			line_num++;
		}
	}
	file.close();
	return true;	
}

bool Table::ShowData()
{
	ifstream file(filename, ios::in);
	string temp_id;
	string temp;
	int line_num = 1;
	if(!file.is_open())
	{
		cout << "cannot open " << filename << ", maybe it has been deleted" << endl;
		return false;
	}
	else
	{
		int i = 0;
		int temp_column;
		string temp_id;
		string temp;
		string temp_item;
		getline(file, temp);

		PrintHead();
		temp_column = id_list.size();

		while( !file.eof())
		{
			getline(file, temp);
			if(temp == "") //末尾是空行
				break;
			stringstream s(temp);
			vector<string> temp_data;
			while(s)	
			{
				s >> temp_id;
				temp_data.push_back(temp_id);
			}
			temp_data.pop_back();
			int index = 0;
			
			cout << "| " << line_num;
			for(int i = 0; i < bit(row)+2 - bit(line_num); i++)
				cout << " ";
			cout << "|  ";
			line_num++;

			for(vector<string>::iterator it = temp_data.begin(); it != temp_data.end(); it++, index++)
			{
				string temp_it(*it);
				if(*it == DEFAULTSTRING)
					temp_it = " ";
				cout << temp_it ;
				for(int temp = 0; temp < width[index] + 2 - temp_it.size(); temp++)
					cout << " ";
				cout <<"|  ";
			}
			cout << endl;
		}		

		cout << "+";
		for(int i = 0; i < bit(row)+2;i++)
			cout << "-";	
		cout << "+";

		for(vector<int>::iterator it = width.begin(); it != width.end(); it++)
		{
			for(int i = 0; i < (*it)+ 4; i++) //前后多占 4 个 空格
				cout << "-";
			cout << "+";
		}

		cout << endl;	
		cout << "row = " << line_num-1 << "," << "column = " << temp_column << endl;
	}
	file.close();
	return true;	
}

void Table::PrintHead()
{
	cout << "+";
	for(int i = 0; i < bit(row)+2;i++)
		cout << "-";
	cout << "+";

	for(vector<int>::iterator it = width.begin(); it != width.end(); it++)
	{
		for(int i = 0; i < (*it)+ 4; i++) //前后多占 4 个 空格
			cout << "-";
		cout << "+";
	}
	cout << endl;

	cout << "| ID";
	for(int i = 0; i < bit(row) - 1; i++)
		cout << " ";
	cout << "|  ";

	int index = 0;
	for(vector<string>::iterator it = id_list.begin(); it != id_list.end(); it++, index++)
	{
		cout << *it;
		for(int temp = 0; temp < width[index] + 2 - (*it).size(); temp++)
			cout << " ";
		cout <<"|  ";
	}

	cout << endl;

	cout << "+";
	for(int i = 0; i < bit(row)+2;i++)
		cout << "-";	
	cout << "+";

	for(vector<int>::iterator it = width.begin(); it != width.end(); it++)
	{
		for(int i = 0; i < (*it)+ 4; i++) //前后多占 4 个 空格
			cout << "-";
		cout << "+";
	}	
	cout << endl;	
}



//匹配函数，不属于任何类
//输入：待匹配字符串 input, 匹配表达式 pattern，存放匹配信息的 matched结构体
//输出：若匹配成功， matched 保存对应信息，返回 true， 否则返回 false
bool MatchInput(string & input, const char *pattern, regmatch_t matched[])
{
	char errorbuf[ERRBUF];
	char match[MATCHBUF];
	int error;
	regex_t reg;
	TrimSpace(input);
	if((error = regcomp(&reg, pattern, REG_EXTENDED)) < 0)
	{
		regerror(error, &reg, errorbuf, sizeof(errorbuf));
		cout << "error :" << errorbuf << endl;
		return false;
	}
	else
	{
		const char *cinput = input.c_str();
		error = regexec(&reg, cinput, MAXMATCHNUM, matched, 0);
		if(error == REG_NOMATCH)
		{
			 cout << "not match expression：" << pattern << endl;
			return false;
		}
		else if(error)
		{
			regerror(error, &reg, errorbuf, sizeof(errorbuf));
			cout << "error : " << errorbuf << endl;
			return false;
		}
		else
		{
			// for(int i = 0; i < MAXMATCHNUM && matched[i].rm_so != -1; i++)
			// {
			// 	int len = matched[i].rm_eo - matched[i].rm_so;
			// 	if(len)
			// 	{
			// 		memset(match, '\0', sizeof(match));
			// 		memcpy(match, cinput + matched[i].rm_so, len);
			// 		// cout << "i = " << i << " " << match << endl;
			// 	}
			// }

			return true;
		}	
	}
}

bool checkinput(string & input)
{
	TrimSpace(input);
	string temp(input);
	transform(input.begin(), input.end(), temp.begin(), ::tolower);
	if(temp == "quit")
	{
		transform(input.begin(), input.end(), input.begin(), ::tolower);
		return false;
	}
	else
		return temp != "";
}

int bit(int num)
{
	int i;
	for(i = 1; num != 0; i++)
	{
		num /= 10;
	}
	return i;
}

void TrimSpace(string &s)
{
    if (s.empty()) 
        return ;
	s.erase(0,s.find_first_not_of(" ")); 
	s.erase(s.find_last_not_of(" ") + 1);
}

bool TrimALLSpace(string & s)
{
	bool hasspace = false;
    int index = 0;
 	s.erase(0,s.find_first_not_of(" ")); 
	s.erase(s.find_last_not_of(" ") + 1);
    if( !s.empty() )
        while( (index = s.find(' ',index)) != string::npos)
        {
        	hasspace = true;
          	s.erase(index,1);
		}
	return hasspace;
}

void Split(string to_be_split, string & temp_id, string & temp_item)
{
	const char * s = to_be_split.c_str();
	char temp[MATCHBUF];
	const char *p = s;
	char *q;

	for(q = temp; *p != '='; p++, q++)
		*q = *p;

	*q = '\0';

	temp_id = temp;

	for(++p, q = temp; *p != '\0'; p++, q++)
		*q = *p;

	*q = '\0';
	temp_item = temp;
}

bool vector_compare(string &i, string& j, bool order)
{
	if(i == "None")
		i = "";
	if(j == "None")
		j = "";

	if(order)
		return i >= j;
	else
		return i <= j;
}

bool vector_exchange(vector<string> & list, int i, int j)
{
	string temp;
	vector<string>::iterator it = list.begin();
	temp = *(it+i);
	*(it+i) = *(it+j);
	*(it+j) = temp;
}


bool Vector_not_Equal(vector<string> & A, vector<string> &B)
{
	for(vector<string>::iterator it = A.begin(); it != A.end(); it++)
		if(*it != B[it- A.begin()])
			return true;
	return false;
}
