#include "select.h"

Select::Select():
	All(1), Part(2), Distinct(3), Order(4), Part_where(5), To(6),
	select_all("\\s*SELECT\\s+\\*\\s+FROM\\s+(\\S*)\\s*"),
	select_part("\\s*SELECT\\s+(.*)\\s+FROM\\s+(\\S*)\\s*"),
	select_distinct("\\s*SELECT\\s+DISTINCT\\s+(.*)\\s+FROM\\s+(\\S*)\\s*"),
	select_order("\\s*SELECT\\s+(.*)\\s+FROM\\s+(\\S*)\\s+ORDER\\s+BY\\s+(.*)\\s+(\\S*)\\s*"),
	select_part_where("\\s*SELECT\\s+(.*)\\s+FROM\\s+(\\S*)\\s+WHERE\\s+(\\S*)\\s*=\\s*(\\S*)\\s*"),
	select_to("\\s*SELECT\\s+(.*)\\s+FROM\\s+(\\S*)\\s+TO\\s+(\\S*)\\s*")
{}

Select::~Select()
{}

void Select::Print()
{
	for(vector<string>::iterator i = part_id_list.begin(); i != part_id_list.end(); i++)
		cout << *i << " ";
	cout << endl;

}

void Select::SortMatched(string &input, int flag)
{
	if(flag == All)
	{
		const char *cinput = input.c_str();
		char temp[MATCHBUF];
		// 整理 tablename	
		int len = matched[1].rm_eo - matched[1].rm_so;
		memset(temp, '\0', sizeof(temp));
		memcpy(temp, cinput+matched[1].rm_so, len);	
		table->tablename = string(temp);
	}
	if(flag == Part || flag == To || flag == Part_where || flag == Order)
	{
		const char *cinput = input.c_str();
		char temp[MATCHBUF];
		int len = matched[1].rm_eo - matched[1].rm_so;
		if(len)	//	整理 ID
		{
			memset(temp, '\0', sizeof(temp));
			memcpy(temp, cinput+matched[1].rm_so, len);
			string s_temp(temp);
			TrimALLSpace(s_temp);
			char *id;

			const char *ss = s_temp.c_str();
			char s[MATCHBUF];
			strcpy(s, ss);
			for(id = strtok(s, ","); id != NULL; id = strtok(NULL, ","))
			{
				string newid(id);
				part_id_list.push_back(newid);
			}
		}	
		else
			return;
		// 整理 tablename
		len = matched[2].rm_eo - matched[2].rm_so;
		memset(temp, '\0', sizeof(temp));
		memcpy(temp, cinput+matched[2].rm_so, len);	
		table->tablename = string(temp);
		
		if(flag == To)
		{
			// 整理 filename
			len = matched[3].rm_eo - matched[3].rm_so;
			memset(temp, '\0', sizeof(temp));
			memcpy(temp, cinput+matched[3].rm_so, len);	
			select_filename = string(temp);
		}

		if(flag == Part_where)
		{			
			len = matched[3].rm_eo - matched[3].rm_so;
			memset(temp, '\0', sizeof(temp));
			memcpy(temp, cinput+matched[3].rm_so, len);	
			select_id = string(temp);

			len = matched[4].rm_eo - matched[4].rm_so;
			memset(temp, '\0', sizeof(temp));
			memcpy(temp, cinput+matched[4].rm_so, len);	
			select_item = string(temp);			

		}

		if(flag == Order)
		{
			len = matched[3].rm_eo - matched[3].rm_so;
			if(len)	//	整理 ID
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
					string newid(id);
					order_id_list.push_back(newid);
				}
			}

			len = matched[4].rm_eo - matched[4].rm_so;
			memset(temp, '\0', sizeof(temp));
			memcpy(temp, cinput+matched[4].rm_so, len);	
			ordermethod = string(temp);
			if(ordermethod != "ASC" && ordermethod != "DESC")
			{
				cout << "wrong oder, default as ASC" << endl;
				ASCorNot = true;
			}
			else
				ASCorNot = ordermethod == "ASC";
		}
	}


	if(flag == Distinct)
	{
		const char *cinput = input.c_str();
		char temp[MATCHBUF];
		int len = matched[1].rm_eo - matched[1].rm_so;
		if(len)	//	整理 ID
		{
			memset(temp, '\0', sizeof(temp));
			memcpy(temp, cinput+matched[1].rm_so, len);
			string s_temp(temp);
			TrimALLSpace(s_temp);
			char *id;

			const char *ss = s_temp.c_str();
			char s[MATCHBUF];
			strcpy(s, ss);
			for(id = strtok(s, ","); id != NULL; id = strtok(NULL, ","))
			{
				string newid(id);
				part_id_list.push_back(newid);
			}
		}

		len = matched[2].rm_eo - matched[2].rm_so;
		memset(temp, '\0', sizeof(temp));
		memcpy(temp, cinput+matched[2].rm_so, len);	
		table->tablename = string(temp);

	}
}

int Select::Match(string& input)
{
	if(input.find("TO") == input.npos && input.find("WHERE") == input.npos && input.find("DISTINCT") == input.npos && input.find("ORDER") == input.npos)
	{
		if(Match_more(input, select_all, matched))
		{
			if(MatchInput(input, select_all, matched))
			{
				SortMatched(input, All);
				return All;
			}		
		}
	}

	if(input.find("TO") == input.npos && input.find("WHERE") == input.npos && input.find("DISTINCT") == input.npos && input.find("ORDER") == input.npos)
	{
		if(Match_more(input, select_part, matched))
		{
			if(MatchInput(input, select_part, matched))
			{
				SortMatched(input, Part);
				return Part;
			}		
		}	
	}

	if(Match_more(input, select_distinct, matched))
	{
		if(MatchInput(input, select_distinct, matched))
		{
			SortMatched(input, Distinct);
			return Distinct;
		}		
	}
	
	if(Match_more(input, select_order, matched))
	{
		if(MatchInput(input, select_order, matched))
		{
			SortMatched(input, Order);
			return Order;
		}	
	}
	
	if(input.find("WHERE") != input.npos)	
	{
		if(Match_more(input, select_part_where, matched))
		{
			if(MatchInput(input, select_part_where, matched))
			{
				SortMatched(input, Part_where);
				return Part_where;
			}		
		}
	}

	if(input.find("TO") != input.npos)
	{	
		if(Match_more(input, select_to, matched))
		{
			if(MatchInput(input, select_to, matched))
			{
				SortMatched(input, To);
				return To;
			}		
		}
	}


	return ERROR;
}

bool Select::Match_more(string input, const char* pattern, regmatch_t* matched)
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
		{
			return false;
		}else if(error)
		{
			return false;
		}else
		{
			return true;	
		}
	}	
}

bool Select::Select_Part(vector<Table> &table_list)
{
	int delete_column;
	bool isfound = false;
	int fit_num = 0;
	for(vector<Table>::iterator it = table_list.begin(); it != table_list.end(); it++)
	{
		if(it->tablename == table->tablename)
		{
			isfound = true;
			table->id_list = it->id_list;
			table->data_list = it->data_list;
			table->filename = it->filename;
			table->width = it->width;
			table->row = it->row;
			fit_num = part_id_list.size();
			delete_column = 0;

			if(part_id_list.size()== 1 && part_id_list.at(0) == "*")
				return true;

			for(vector<string>::iterator it_id = it->id_list.begin(); it_id != it->id_list.end(); it_id++, delete_column++)
			{
				vector<string>::iterator temp = find(part_id_list.begin(), part_id_list.end(), *it_id);
				if( temp == part_id_list.end() )
				{
					table->data_list.erase(table->data_list.begin() + delete_column);
					table->id_list.erase(table->id_list.begin() + delete_column);
					table->width.erase(table->width.begin() + delete_column);
					delete_column--;
				}
				else
					fit_num--;
			}
			break;
		}
	}

	if(!isfound)
	{
		cout << "no table is called " << table->tablename << endl;
		return false;
	}
	else if(fit_num == part_id_list.size())
	{
		cout << "Error: All id miss-match, total:  "<< fit_num << endl; 
		return false;
	}
	else 	
	{
		if(fit_num)
			cout << "Warning: Some id miss-match, total:  "<< fit_num << endl; 
		return true;
	}
}

//////报错机制还没写
bool Select::Select_Where(vector<Table>& table_list)
{
	if(	!Select_Part(table_list))
		return false;

	int temp_column;
	bool isfound = false;
	// if(part_id_list.size() == 1 && part_id_list.at(0) == "*")
	// {	
	// 	for(vector<Table>::iterator it = table_list.begin(); it != table_list.end(); it++)
	// 	{
	// 		if(it->tablename == table->tablename)
	// 		{
	// 			table->id_list = it->id_list;
	// 			table->data_list = it->data_list;
	// 			table->filename = it->filename;
	// 			table->width = it->width;
	// 			table->row = it->row;
	// 			break;
	// 		}
	// 	}
	// }
	// else 
	// {	
	// 	if(!Select_Part(table_list))
	// 	{
	// 		cout << "soomthing wrong" << endl;
	// 		return false;
	// 	}
	// }	

	for(vector<Table>::iterator it = table_list.begin(); it != table_list.end(); it++)
	{
		if(it->tablename == table->tablename)
		{
			temp_column = 0;
			for(vector<string>::iterator it_id = it->id_list.begin(); it_id != it->id_list.end(); it_id++, temp_column++)
			{
				if(*it_id == select_id)
				{
					int temp_row = 0;
					for(vector<string>::iterator it_item = (it->data_list)[temp_column].begin(); it_item != (it->data_list)[temp_column].end(); it_item++, temp_row++)
					{
						if(*it_item == select_item)
						{
							isfound = true;
							select_row.push_back(temp_row);
						}
					}
					if(!isfound)
					{
						cout << "in table " << table->tablename << ", no item is called " << select_item << " in id column:" << select_id << endl;
						return false;
					}
				}
			}
			if(!isfound)
			{
				cout << "in table " << table->tablename << ", no id is called " << select_id << endl;
				return false;
			}
		}
	}

	select_table.row = select_row.size();
	select_table.column = table->id_list.size();
	select_table.width = table->width;
	select_table.id_list = table->id_list;
	select_table.data_list.resize(table->data_list.size());
	temp_column = 0;
	for(vector<vector<string>>::iterator it = table->data_list.begin(); it != table->data_list.end(); it++, temp_column++)
	{
		for(vector<int>::iterator i = select_row.begin(); i != select_row.end(); i++)
		{
			(select_table.data_list)[temp_column].push_back((*it)[*i]);
		}
	}

	return true;
}

bool Select::Select_Distinct(vector<Table> & table_list)
{
	if(	!Select_Part(table_list))
		return false;


	vector<vector<string>> vector_row;
	vector_row.resize(table->row);
	vector<bool> vector_not_ok;
	vector_not_ok.resize(table->row);
	for(vector<bool>::iterator i = vector_not_ok.begin(); i != vector_not_ok.end(); i++)
		*i = true;


	for(vector<vector<string>>::iterator it = table->data_list.begin(); it != table->data_list.end(); it++)
	{
		for(vector<string>::iterator i = it->begin(); i != it->end(); i++)
			vector_row[i - it->begin()].push_back(*i);
	}

	for(int i = 0; i < table->row; i++)
		for(int j = i+1; j < table->row; j++)
			if(!Vector_not_Equal(vector_row[i],vector_row[j]))
				vector_not_ok[j] = false;

	// for(vector<bool>::iterator i = vector_not_ok.begin(); i != vector_not_ok.end(); i++)
	// 	cout << *i << " " ;
	// cout << endl;

	for(vector<vector<string>>::iterator it = table->data_list.begin(); it != table->data_list.end(); it++)
	{
		int deletenum = 0;
		for(vector<bool>::iterator i = vector_not_ok.begin(); i != vector_not_ok.end(); i++)
			if( *i == false)
				it->erase(it->begin() + (i - vector_not_ok.begin()) - deletenum++ );
	}

	int falsenum = 0;
	for(vector<bool>::iterator i = vector_not_ok.begin(); i != vector_not_ok.end(); i++)
		if(*i == false)
			falsenum++;
	table->row -= falsenum;

	return true;
}

bool Select::Select_Order(vector<Table>& table_list)
{
	if(	!Select_Part(table_list))
		return false;

	if(!Sort_id_list(table->id_list, order_id_list))
		return false;

	vector<int> id_column;
	for(vector<string>::iterator it = order_id_list.begin(); it!= order_id_list.end(); it++)
	{
		for(vector<string>::iterator it_id = table->id_list.begin(); it_id != table->id_list.end(); it_id++)
		{
			if(*it == *it_id)
			{
				int index = it_id - table->id_list.begin();
				id_column.push_back(index);
				vector<vector<string>>::iterator it_index = (table->data_list).begin() + (it_id - table->id_list.begin());	
				for(vector<string>::iterator i = it_index->begin(); i != it_index->end();i++)
					for(vector<string>::iterator j = i; j != it_index->end(); j++)
					{
						if(vector_compare(*i, *j, ASCorNot))
						{	
							bool youcanexchange = true;

							if(id_column.size() > 1)
							{
								for(int number = 0; number < id_column.size()-1; number++)
								{
									if((table->data_list[id_column[number]][i - it_index->begin()]) != (table->data_list[id_column[number]][j - it_index->begin()]))
									{
										youcanexchange = false;
									}
								}
							}

							if(youcanexchange)
							    for(vector<vector<string>>::iterator temp_it = table->data_list.begin(); temp_it != table->data_list.end(); temp_it++)
									vector_exchange(*temp_it, i - it_index->begin(), j - it_index->begin());
						}
						if(*i == "")		//	为达目的不择手段 可耻
							*i = DEFAULTSTRING;
						if(*j == "")
							*j = DEFAULTSTRING;
					}
			}
		}
	}

	// Print();
	return true;
}

bool Select::Sort_id_list(vector<string>& id_list, vector<string> &part_id_list)
{
	bool fit = false;
	int error = 0;
	vector<string>temp;
	for(vector<string>::iterator i = part_id_list.begin(); i != part_id_list.end(); i++)
	{
		fit = false;
		for(vector<string>::iterator it = id_list.begin(); it != id_list.end(); it++)
			if(*it == *i)
			{
				fit = true;
				vector<string>::iterator t = find(temp.begin(), temp.end(), *i);
				if(t == temp.end())
					temp.push_back(*i);
				else
					cout << "Warning : "<< *i << " is repeat " << endl;
			}
		if(!fit)
		{
			error++;
			cout << *i << " is not a qualified id" << endl;
		}

	}

	if(error)
		cout << "total error: " << error << endl;
	else
		return true;
}