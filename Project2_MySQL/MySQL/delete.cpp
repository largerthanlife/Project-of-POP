#include "delete.h"

Delete::Delete():
	delete_part("\\s*DELETE\\s+FROM\\s+(\\S*)\\s+WHERE\\s+(\\S*)\\s*=\\s*(\\S*)\\s*"),
	delete_all("\\s*DELETE\\s+\\*\\s+FROM\\s+(\\S*)\\s*"),
	Part(1),
	All(2)
{
	// cout << "you got to delet sth" << endl;
}

Delete::~Delete()
{

}

void Delete::Print()
{

}

void Delete::SortMatched(string &input, int flag)
{
	if(flag == Part)
	{
		const char *cinput = input.c_str();
		char temp[MATCHBUF];
		// 整理 tablename
		int len = matched[1].rm_eo - matched[1].rm_so;
		memset(temp, '\0', sizeof(temp));
		memcpy(temp, cinput+matched[1].rm_so, len);	
		table->tablename = string(temp);

		// 整理待删除项目对应的 id 列
		len = matched[2].rm_eo - matched[2].rm_so;
		memset(temp, '\0', sizeof(temp));
		memcpy(temp, cinput+matched[2].rm_so, len);	
		delete_id = string(temp);

		// 整理待删除项目的名字
		len = matched[3].rm_eo - matched[3].rm_so;
		memset(temp, '\0', sizeof(temp));
		memcpy(temp, cinput+matched[3].rm_so, len);	
		delete_item = string(temp);
	}

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
}
	
int Delete::Match(string &input)
{
	if(input.find("WHERE") != input.npos)
		if(MatchInput(input, delete_part, matched))
		{
			SortMatched(input, Part);
			//Print();		
			return Part;
		}
		else
			return ERROR;
	else if(input.find("*") != input.npos) //非常简易，后续要改进，此处先追求能用
		if(MatchInput(input, delete_all, matched))
		{
			SortMatched(input, All);			
			return All;
		}
		else
			return ERROR;	
	else
		return ERROR;	
}

// clear swap resize 简直是折磨
bool Delete::Delete_All(vector<Table> &table_list)
{
	cout << "deleting……" << endl;
	for(vector<Table>::iterator it = table_list.begin(); it != table_list.end(); it++)
		if(it->tablename == table->tablename)
		{
			for(vector<vector<string>>::iterator i = it->data_list.begin(); i != it->data_list.end(); i++)
			{
				i->clear();
			}

			it->row = 0;
			it->width.clear();
			for(vector<string>::iterator it_id = it->id_list.begin(); it_id != it->id_list.end(); it_id++)
				it->width.push_back((*it_id).size());
			
			return it->WriteFile();
		}
	cout << "There is no table called " << table->tablename << endl;
	return false;
}

bool Delete::Delete_Part(vector<Table> &table_list)
{
	bool found = false;
	for(vector<Table>::iterator it = table_list.begin(); it != table_list.end() && !found; it++)
		if(it->tablename == table->tablename)
		{
			int temp_column = 0;
			// int temp_row = 0;
			for(vector<string>::iterator i = it->id_list.begin(); i != it->id_list.end() && !found; i++, temp_column++)
			{
				if( *i == delete_id)
				{
					for(vector<string>::iterator j = (it->data_list)[temp_column].begin(); j != (it->data_list)[temp_column].end(); j++/*, temp_row++*/)
					{
						if( *j == delete_item)
						{
							found = true;
							delete_num.push_back(j-(it->data_list)[temp_column].begin());
						}
					}
					if(!found)
					{
						cout << "There is no item called " << delete_item << " in the " << delete_id << " column" << " of table "<< table->tablename <<endl;
						return false; 
					}
				}
			}
			if(!found)
			{
				cout << "There is no id called " << delete_id << " in table " << table->tablename << endl;
				return false;
			}
			else
			{	
				cout << "deleting……" << endl;
				// temp_row--;
				for(vector<vector<string>>::iterator k = it->data_list.begin(); k != it->data_list.end(); k++)
				{
					int num = 0;
					for(vector<int>::iterator i = delete_num.begin(); i != delete_num.end(); i++, num++)
						k->erase(k->begin() + *i - num);
				}
				it->row -= delete_num.size();
				return it->WriteFile();	
			}	
		}
	if(!found)
	{
		cout << "There is no table called " << table->tablename << endl;
		return false;
	}

	return false;
}
