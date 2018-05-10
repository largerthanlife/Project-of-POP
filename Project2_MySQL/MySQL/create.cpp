#include "create.h"
#include "mysql.h"

Create::Create():
	CreateToFile(1), 
	CreateFromFile(2),
	pattern_to("\\s*CREATE\\s+TABLE\\s+(\\S*)\\s+\\((.*)\\)\\s+TO\\s+(\\S*)\\s*"),
	pattern_from("\\s*CREATE\\s+TABLE\\s+(\\S*)\\s+FROM\\s+(\\S*)\\s*")
{
	;
	// cout << "to Create table function" << endl;
}

Create::~Create()
{

}


int Create::Match(string & input)
{
	if(input.find("TO") != input.npos)
		if(MatchInput(input, pattern_to, matched))
		{
			SortMatched(input, CreateToFile);
			//Print();		
			return CreateToFile;
		}
		else
			return ERROR;
	else if(input.find("FROM") != input.npos)
		if(MatchInput(input, pattern_from, matched))
		{
			SortMatched(input, CreateFromFile);			
			return CreateFromFile;
		}
		else
			return ERROR;	
	else
		return ERROR;
}

bool Create::Create_to_file()
{
	int id_num = 0;
	const char *name = table->filename.c_str();
	ofstream file(table->filename, ios::out);
	string temp;
	if(!file.is_open())
	{
		cout << "can not create " << table->filename << endl;
		return false;
	}
	else
		for(vector<string>::iterator it = table->id_list.begin(); it != table->id_list.end(); it++, id_num++)
		{
			file << *it << " ";
			table->width.push_back((*it).size());
		}
	file.close();
	table->data_list.resize(id_num);
	return true;
}

bool Create::Create_from_file()
{
	int column = -1;
	int row = 0;
	ifstream file(Getfilename(), ios::in);
	if(!file.is_open())
	{
		cout << "There is no file named " << Getfilename() << " or has been deleted" << endl;
		return false;
	}
	else
	{
		int i = 0;
		string temp_id;
		string temp;
		string temp_item;
		getline(file, temp);
		if(temp == "") //空行
			return true;
		stringstream s(temp);	
		while(s)	//	这里总是会多复制一次最后一项
		{
			s >> temp_id;
			table->id_list.push_back(temp_id);
			column++;
			table->width.push_back(temp_id.size());
		}
		table->id_list.pop_back();
		table->width.pop_back();
		table->column = column;
		table->row = 0;
		table->data_list.resize(column);

		while(!file.eof())
		{
			i = 0;
			getline(file, temp);		
			if(temp == "")
				break;
			row++;
			stringstream s(temp);
			while(i < column)
			{
				s >> temp_item;
				if(temp_item.size() > (table->width)[i])
					(table->width)[i] = temp_item.size();
				table->data_list[i++].push_back(temp_item);
			}	
			// cout << endl;
		}

		if(row != 0)	//说明该文件不只是ID行， 还有数据行
			table->row = row;
		else	//只有 ID 行
		{
			table->row = 0;
			i= 0;
		}
		column += 1; // 实际column， 注意初始化时以 -1 开始， 是因为要和数组下标比较，较方便
		return true;
	}
}

void Create::SortMatched(string & input, int flag)
{
	if(flag == CreateToFile)
	{// return;
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
				string newid(id);
				table->id_list.push_back(newid);
			}
		}	
		else
			return;
		// 整理 tablename
		len = matched[1].rm_eo - matched[1].rm_so;
		memset(temp, '\0', sizeof(temp));
		memcpy(temp, cinput+matched[1].rm_so, len);	
		table->tablename = string(temp);

		// 整理 filename
		len = matched[3].rm_eo - matched[3].rm_so;
		memset(temp, '\0', sizeof(temp));
		memcpy(temp, cinput+matched[3].rm_so, len);	
		table->filename = string(temp);

		table->column = table->id_list.size();
		table->row = 0;
	}
	else if(flag == CreateFromFile)
	{
		const char *cinput = input.c_str();
		char temp[MATCHBUF];
		int len = matched[1].rm_eo - matched[1].rm_so;
		memset(temp, '\0', sizeof(temp));
		memcpy(temp, cinput+matched[1].rm_so, len);	
		table->tablename = string(temp);

		// 整理 filename
		len = matched[2].rm_eo - matched[2].rm_so;
		memset(temp, '\0', sizeof(temp));
		memcpy(temp, cinput+matched[2].rm_so, len);	
		table->filename = string(temp);

		table->column = table->id_list.size();
		table->row = 0;	
	}
}

void Create::Print()
{
	cout << "id: ";
	for(vector<string>::iterator it = table->id_list.begin(); it != table->id_list.end(); it++)
		cout << *it << " ";
	cout << endl;
	cout << "tablename: " << table->tablename << endl;
	cout << "filename: " << table->filename << endl;
}
