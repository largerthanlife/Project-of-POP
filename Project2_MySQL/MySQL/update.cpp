#include "update.h"

Update::Update():
	All(1),
	Part(2),
	update_all("\\s*UPDATE\\s+(\\S*)\\s+SET\\s+(.*)\\s*"),
	update_part("\\s*UPDATE\\s+(\\S*)\\s+SET\\s+(.*)\\s+WHERE\\s+(\\S*)\\s*=\\s*(\\S*)\\s*")
{

}

Update::~Update()
{

}

void Update::Print()
{
	for(vector<string>::iterator it = id_update.begin(); it != id_update.end(); it++)
		cout << *it << " ";
	cout << endl;

	for(vector<string>::iterator it = item_update.begin(); it != item_update.end(); it++)
		cout << *it << " ";
	cout << endl;	
}

void Update::SortMatched(string& input, int flag)
{
	const char *cinput = input.c_str();
	char temp[MATCHBUF];
	// match tablename
	int len = matched[1].rm_eo - matched[1].rm_so;
	memset(temp, '\0', sizeof(temp));
	memcpy(temp, cinput+matched[1].rm_so, len);	
	table->tablename = string(temp);

	len = matched[2].rm_eo - matched[2].rm_so;
	if(len)	//	整理 ID
	{
		memset(temp, '\0', sizeof(temp));
		memcpy(temp, cinput+matched[2].rm_so, len);
		string s_temp(temp);
		TrimALLSpace(s_temp);
		char *temp_pair;

		const char *ss = s_temp.c_str();
		char s[MATCHBUF];
		strcpy(s, ss);
		for(temp_pair = strtok(s, ","); temp_pair != NULL; temp_pair = strtok(NULL, ","))
		{
			string temp_newid;
			string temp_newitem;
			string temp_newpair(temp_pair);

			Split(temp_newpair, temp_newid, temp_newitem);//????这个函数居然对temp_pair 有影响，能让它变成NULL？
			id_update.push_back(temp_newid);
			item_update.push_back(temp_newitem);
		}
	}
	else	//	maybe a problem
		return;

	if(flag == Part)//力求能用，待改进
	{
		len = matched[3].rm_eo - matched[3].rm_so;
		memset(temp, '\0', sizeof(temp));
		memcpy(temp, cinput+matched[3].rm_so, len);	
		optional_id = temp;

		len = matched[4].rm_eo - matched[4].rm_so;
		memset(temp, '\0', sizeof(temp));
		memcpy(temp, cinput+matched[4].rm_so, len);		
		optional_item = temp;
	}
}

int Update::Match(string& input)
{
	if(input.find("WHERE") != input.npos)
	{
		if(MatchInput(input, update_part, matched))
		{
			SortMatched(input, Part);
			return Part;
		}
	}
	else
	{
		if(MatchInput(input, update_all, matched))
		{
			SortMatched(input, All);
			return All;
		}
	}
	cout << "Cannot match for update-function" << endl;
	return ERROR;
}	

bool Update::Update_All(vector<Table>& table_list)
{
	bool isfound = false;
	int fit_num = 0;
	int temp_column, temp_column_update;
	for(vector<Table>::iterator it_table = table_list.begin(); it_table != table_list.end(); it_table++)
		if(table->tablename == it_table->tablename)
		{
			if(id_update.size() > it_table->column)
			{
				cout << "column too larger" << endl;
				return false;
			}
			temp_column_update = 0;
			for(vector<string>::iterator it_id = id_update.begin() ; it_id != id_update.end(); it_id++, temp_column_update++)
			{
				temp_column = 0;
				for(vector<string>::iterator it_idlist = it_table->id_list.begin(); it_idlist != it_table->id_list.end(); it_idlist++, temp_column++)
				{
					if(*it_id == *it_idlist)
					{
						fit_num++;
						isfound = true;
						for(vector<string>::iterator it_item = (it_table->data_list)[temp_column].begin(); it_item != (it_table->data_list)[temp_column].end(); it_item++)
							*it_item = item_update[temp_column_update];
						// if(it_id->size() > (it_table->width)[temp_column])
						(it_table->width)[temp_column] = it_id->size()>item_update[temp_column_update].size()? it_id->size() : item_update[temp_column_update].size();
					}
				}
				if(!isfound)
				{
					cout << "no id in " << table->tablename << " is called " << *it_id << endl;
				}
				else
					isfound = false;
			}
			if(fit_num < id_update.size())
			{
				cout << "Warning: some id cannot be updated, ";
				cout << "total: " << id_update.size() - fit_num << endl;
			}

			return it_table->WriteFile();

		}

	cout << "Thers's no table called " << table->tablename << ", use \"TABLE LIST \" to check it out " << endl;
	return false;
}
	
bool Update::Update_Part(vector<Table>& table_list)
{
	bool isfound = false, fit = false;
	int fit_num = 0;
	int temp_column, temp_column_update, temp_row, temp_row_update;
	for(vector<Table>::iterator it_table = table_list.begin(); it_table != table_list.end(); it_table++)
		if(table->tablename == it_table->tablename)
		{
			if(id_update.size() > it_table->column)
			{
				cout << "column too larger" << endl;
				return false;
			}

			int temp_temp_column = 0;
			for(vector<string>::iterator it_idlist_temp = it_table->id_list.begin(); it_idlist_temp != it_table->id_list.end(); it_idlist_temp++, temp_temp_column++)
			{
				temp_row = -1;
				if(*it_idlist_temp == optional_id)
				{
					temp_row = 0;
					fit = false;
					for(vector<string>::iterator i = (it_table->data_list)[temp_temp_column].begin(); i != (it_table->data_list)[temp_temp_column].end(); i++, temp_row++)
					{
						if(*i == optional_item)
						{
							fit = true;
							fit_num = 0;
							isfound = false;
							temp_column_update = 0;
							for(vector<string>::iterator it_id = id_update.begin() ; it_id != id_update.end(); it_id++, temp_column_update++)
							{
								temp_column = 0;
								for(vector<string>::iterator it_idlist = it_table->id_list.begin(); it_idlist != it_table->id_list.end(); it_idlist++, temp_column++)
								{
									if(*it_id == *it_idlist)
									{
										fit_num++;
										isfound = true;
										(it_table->data_list)[temp_column][temp_row] = item_update[temp_column_update];
										if(it_id->size() > (it_table->width)[temp_column])
											(it_table->width)[temp_column] = it_id->size();
									}
								}
								if(!isfound)
								{
									cout << "no id in " << table->tablename << " is called " << *it_id << endl;
									return false;
								}
								// else
								// 	isfound = false;
							}
							if(fit_num != id_update.size())
							{
								cout << "Warning: some id cannot be updated, ";
								cout << "total: " << id_update.size() - fit_num << endl;
								fit_num = 0;
							}
						}
					}
					if(!fit)
					{
						cout << "in table : " << it_table->tablename << "," << optional_id << " has no value :" << optional_item << endl;
						return false;
					}
				}
				if(!isfound && temp_row == 0)
				{
					cout << "in table :" << it_table->tablename << ", there is no item called " << optional_item << " in id column: " << *it_idlist_temp << endl;
					return false;
				}
				else
				{
					isfound = false;
				}
			}
			return it_table->WriteFile();

		}

	cout << "Thers's no table called " << table->tablename << ", use \"TABLE LIST \" to check it out " << endl;
	return false;
}
