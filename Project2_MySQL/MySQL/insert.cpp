#include "insert.h"

Insert::Insert():
	Insert_ALL(1),
	Insert_PART(2),
	pattern_all("\\s*INSERT\\s+INTO\\s+(\\S*)\\s+VALUES\\s+\\((.*)\\)\\s*"),
	pattern_part("\\s*INSERT\\s+INTO\\s+(\\S*)\\s+\\((.*)\\)\\s+VALUES\\s+\\((.*)\\)\\s*"),
	item_num(0),
	id_num(0),
	defaultstring(DEFAULTSTRING)
{
	// cout << "insert something " << endl;
}	

Insert::~Insert()
{

}

void Insert::Print()
{
	cout << "item_list: " ;
	for(vector<string>::iterator it = to_be_insert.begin(); it != to_be_insert.end(); it++)
		cout << *it << " ";
	cout << endl;
	cout << "^^^^^^^^^^^^^^^^" << endl;

	cout << "id_list: ";
	for(vector<string>::iterator it = to_be_insert_id.begin(); it != to_be_insert_id.end(); it++)
		cout << *it << " ";
	cout << endl;
	cout << "^^^^^^^^^^^^^^^^" << endl;
}

int Insert::Match(string &input)
{
	int num;
	regex_t reg;

	//多一层判断 因为 pattern_part 符合 pattern_all
	//由于程序结构的原因，只能采取下列方式多判断一次,去掉了报错信息
	// 当然也可以把 MatchInput 中的报错信息删除，但保险起见
	if(Match_more(input, pattern_part, matched))
	{
		if(MatchInput(input, pattern_part, matched))
		{
			SortMatched(input, Insert_PART);
			return Insert_PART;
		}		
	}
	
	if(Match_more(input, pattern_all, matched))
	{
		if(MatchInput(input, pattern_all, matched))
		{
			SortMatched(input, Insert_ALL);
			return Insert_ALL;
		}
	}

	return ERROR;
}

//与 MatchInput 相比，只少了报错信息
bool Insert::Match_more(string input, const char* pattern, regmatch_t* matched)
{
	char errorbuf[ERRBUF];
	char match[MATCHBUF];
	int error;
	regex_t reg;
	TrimSpace(input);
	if((error = regcomp(&reg, pattern, REG_EXTENDED)) < 0)
	{	
		return false;
	}
	else
	{
		const char *cinput = input.c_str();
		error = regexec(&reg, cinput, MAXMATCHNUM, matched, 0);
		if(error == REG_NOMATCH)
			return false;
		else if(error)
			return false;
		else
			return true;	
	}

}

//写的一点都不精简
void Insert::SortMatched(string &input, int flag)
{
	if(flag == Insert_ALL)
	{
		const char *cinput = input.c_str();
		char temp[MATCHBUF];
		int len = matched[2].rm_eo - matched[2].rm_so;
		if(len)	//	整理 ID
		{
			memset(temp, '\0', sizeof(temp));
			memcpy(temp, cinput+matched[2].rm_so, len);
			string s_temp(temp);
			TrimALLSpace(s_temp);
			char *id;

			const char *ss = s_temp.c_str();
			char s[MATCHBUF];
			strcpy(s, ss);
			for(id = strtok(s, ","); id != NULL; id = strtok(NULL, ","))
			{
				item_num++;
				string newid(id);
				to_be_insert.push_back(newid);
			}
		}	
		else
			return;
		// 整理 tablename
		len = matched[1].rm_eo - matched[1].rm_so;
		memset(temp, '\0', sizeof(temp));
		memcpy(temp, cinput+matched[1].rm_so, len);	
		table->tablename = string(temp);
	}


	if(flag == Insert_PART)
	{
		const char *cinput = input.c_str();
		char temp[MATCHBUF];
		int len = matched[2].rm_eo - matched[2].rm_so;
		if(len)	//	整理 待插入数据
		{
			memset(temp, '\0', sizeof(temp));
			memcpy(temp, cinput+matched[2].rm_so, len);
			string s_temp(temp);
			TrimALLSpace(s_temp);
			char *id;

			const char *ss = s_temp.c_str();
			char s[MATCHBUF];
			strcpy(s, ss);
			for(id = strtok(s, ","); id != NULL; id = strtok(NULL, ","))
			{
				id_num++;
				string newid(id);
				to_be_insert_id.push_back(newid);
			}
		}	
		len = matched[3].rm_eo - matched[3].rm_so;
		if(len)	//	整理 待插入数据的ID
		{
			memset(temp, '\0', sizeof(temp));
			memcpy(temp, cinput+matched[3].rm_so, len);
			string s_temp(temp);
			TrimALLSpace(s_temp);
			char *id;

			const char *ss = s_temp.c_str();
			char s[MATCHBUF];
			strcpy(s, ss);
			for(id = strtok(s, ","); id != NULL; id = strtok(NULL, ","))
			{
				item_num++;
				string newid(id);
				to_be_insert.push_back(newid);
			}
		}	
		else
			return;

		// 整理 tablename
		len = matched[1].rm_eo - matched[1].rm_so;
		memset(temp, '\0', sizeof(temp));
		memcpy(temp, cinput+matched[1].rm_so, len);	
		table->tablename = string(temp);

		if(item_num != id_num)
		{
			cout << "item_num and id_num dont fit" << endl;
			cout << "item_num = " << item_num << " while id_num = " << id_num << endl;
			cout << "some error may happen " << endl;
			return;
		}
	}
}

bool Insert::Insert_to_all(vector<Table> & table_list)
{
	for(vector<Table>::iterator it = table_list.begin(); it != table_list.end(); it++)
		if(table->tablename == it->tablename)
		{
			if(item_num != it->column)
			{
				cout << "column not fit " << endl;
				cout << it->column << endl;
				return false;
			}
			else
			{
				// if(table->row)
				{
					int index = 0;
					vector<vector<string>>::iterator i = it->data_list.begin();
					vector<string>::iterator j = to_be_insert.begin();	
					for( ; i != it->data_list.end(); i++, j++, index++)
					{
						i->push_back(*j);
						if((*j).size() > (it->width)[index])
							(it->width)[index] = (*j).size();
					}
				}
			}
			it->row++;
			return it->WriteFile();
		}
	cout << "Thers's no table called " << table->tablename << ", use \"TABLE LIST \" to check it out " << endl;
	return false;
}

//理论上 insert_to_part 可与 insert_to_all 合并
//但是好像不美观
//也许部分相同部分提取为函数会好一点
bool Insert::Insert_to_part(vector<Table> & table_list)
{
	int fit_num = 0; // 在原 id 序列中 匹配个数
	int fit = false;
	for(vector<Table>::iterator it = table_list.begin(); it != table_list.end(); it++)
		if(table->tablename == it->tablename)
		{
			if(item_num > it->column)
			{
				cout << "column not fit " << endl;
				cout << "the to_be_insert list should not bigger than original" << endl;
				cout << it->column << endl;
				return false;
			}
			else
			{
				{
					int index = 0;
					vector<string>::iterator id_it = it->id_list.begin();
					
					//遍历 原始 id 列
					for( ; id_it != it->id_list.end(); id_it++, index ++)
					{
						// if(fit_num == to_be_insert_id.size())
						// 	break;

						int pos = 0;
						for(vector<string>::iterator id_to = to_be_insert_id.begin();id_to != to_be_insert_id.end(); id_to++, pos++)
						{
							if(*id_it == *id_to)
							{
								fit = true;
								fit_num++;
								it->data_list[index].push_back(to_be_insert[pos]);
								if((to_be_insert[pos]).size() > (it->width)[index])
								{	
									(it->width)[index] = (to_be_insert[pos]).size();
								}
							}
						}

						if(fit == false)
						{
							it->data_list[index].push_back(defaultstring);
							if(defaultstring.size() > (it->width)[index])
								(it->width)[index] = defaultstring.size();
						}
						else
							fit = false;
		
					}

					if(fit_num != to_be_insert_id.size())
					{
						cout << "Warning:some id cannot be insert, ";
						cout << "total: " << to_be_insert_id.size() - fit_num << endl;
						// return false;
					}
				}
				// return true;
			}
			it->row++;
			return it->WriteFile();
		}
	cout << "Thers's no table called " << table->tablename << ", use \"TABLE LIST \" to check it out " << endl;
	return false;	
}

