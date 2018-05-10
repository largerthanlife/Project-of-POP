#include "mysql.h"

MySQL::MySQL()
{
	mapfile = "table.config";
	create_new = new Create;
	drop_new   = new Drop;
	insert_new = new Insert;
	update_new = new Update;
	select_new = new Select;
	delete_new = new Delete;
}

MySQL::~MySQL()//好好写
{}

void MySQL::mysql()
{
	string input("init");
	char str[STRBUF];
	bool result = false;
	while(input != "quit")
	{
		cout << "(mysql)==>";
		cin.getline(str,STRBUF);
		input = str;
		TrimSpace(input);
		if(checkinput(input))
		{
			if(input != "READ")
			{
				result = Analyse(input);
				if(result)
					cout << "Congratulation !" << endl;
				else
					cout << "Failed, you silly boy" << endl;	
			}
			else
			{
				if(READ() == false)
					cout << "some error happened" << endl;
				else
					cout << "all operation succeed " << endl;
			}
		}

	}
}


bool MySQL::Analyse(string & input)
{	
	int index;
	switch(Operator_ID(input))
	{
	case CREATETABLE:
	{
		create_new = new Create;

		index = create_new->Match(input);
		if(!check(create_new->Gettablename(), create_new->Getfilename()))
			return false;
		if(index == create_new->CreateToFile)
		{
			if(create_new->Create_to_file())
			{
				if(Register(create_new->table))
				{
					// ShowTable(create_new->table->tablename);
					create_new->table->PrintHead();	
					return true;
				}
			}
		}
		else if(index == create_new->CreateFromFile)
		{
			if(create_new->Create_from_file())
			{
				// create_new->table->PrintHead();
				if( Register(create_new->table) )
				{
					// ShowTable(create_new->table->tablename);
					create_new->table->PrintHead();	
					return true;
				}
			}	
		}		
		return false;
		// break;
	}
	case DROPTABLE:
	{
		drop_new = new Drop;
		index = drop_new->Match(input);
		if(!check(drop_new->Gettablename()))
			return false;

		if(index == drop_new->DROPID)
			if(DropTable(drop_new->Gettablename(), true))
				return true;

		return false;
		// break;
	}
	case INSERTINTO:
	{
		insert_new = new Insert;
		index = insert_new->Match(input);
		if(!check(insert_new->Gettablename()))
			return false;

		if(index == insert_new->Insert_ALL)
		{
			if(insert_new->Insert_to_all(table_list))
			{
				insert_new->table->filename = paired_filename(insert_new->table->tablename);
				ShowTable(insert_new->table->tablename);
				return true;
			}
		}
		else if(index == insert_new->Insert_PART)
		{
			// insert_new->Print(); return true;
			if(insert_new->Insert_to_part(table_list))
			{
				insert_new->table->filename = paired_filename(insert_new->table->tablename);
				ShowTable(insert_new->table->tablename);	
				return true;
			}
		}
		// else
			return false;
		break;
	}
	case DELETE:
	{
		delete_new = new Delete;
		index = delete_new->Match(input);
		if(!check(delete_new->Gettablename()))
			return false;

		if(index == delete_new->All)
		{
			if(delete_new->Delete_All(table_list))
			{
				delete_new->table->filename = paired_filename(delete_new->table->tablename);
				ShowTable(delete_new->table->tablename);
				return true;
			}
		}
		else if(index == delete_new->Part)
		{
			if(delete_new->Delete_Part(table_list))
			{
				delete_new->table->filename = paired_filename(delete_new->table->tablename);
				ShowTable(delete_new->table->tablename);	
				return true;
			}
		}	
		// else
			return false;	
		break;
	}
	case UPDATE:
	{
		update_new = new Update;
		index = update_new->Match(input);
		if(!check(update_new->Gettablename()))
			return false;

		if(index == update_new->All)
		{
			if(update_new->Update_All(table_list))
			{
				// update_new->Print();
				update_new->table->filename = paired_filename(update_new->table->tablename);
				ShowTable(update_new->table->tablename);
				return true;
			}
		}
		else if(index == update_new->Part)
		{
			if(update_new->Update_Part(table_list))
			{
				// update_new->Print();
				update_new->table->filename = paired_filename(update_new->table->tablename);
				ShowTable(update_new->table->tablename);	
				return true;
			}
		}	
		// else
			return false;		
		break;
	}
	case SELECT:
	{
		select_new = new Select;
		index = select_new->Match(input);
		if(!check(select_new->Gettablename()))
			return false;
		
		cout << "index = " << index << endl;
		if(index == select_new->All)
		{
			select_new->table->filename = paired_filename(select_new->table->tablename);
			ShowTable(select_new->table->tablename);
			return true;
		}
		else if(index == select_new->Part)
		{
			if(select_new->Select_Part(table_list))
			{
				select_new->table->tablename += POSTFIX;
				select_new->table->filename += POSTFIX;
				table_list.push_back(*(select_new->table));
				if(select_new->table->WriteFile())
				{
					ShowTable(select_new->table->tablename);
					return DropTable(select_new->table->tablename, true);
				}
			}
		}
		else if(index == select_new->Distinct)
		{
			if(select_new->Select_Distinct(table_list))
			{
				select_new->table->tablename += POSTFIX;
				select_new->table->filename += POSTFIX;
				table_list.push_back(*(select_new->table));
				if(select_new->table->WriteFile())
				{
					ShowTable(select_new->table->tablename);
					return DropTable(select_new->table->tablename, true);
				}
			}
		}
		else if(index == select_new->Order)
		{
			if(select_new->Select_Order(table_list))
			{
				// return true;
				select_new->table->tablename += POSTFIX;
				select_new->table->filename += POSTFIX;
				table_list.push_back(*(select_new->table));
				if(select_new->table->WriteFile())
				{
					ShowTable(select_new->table->tablename);
					return DropTable(select_new->table->tablename, true);
				}
			}
		}
		else if(index == select_new->Part_where)
		{
			if(select_new->Select_Where(table_list))
			{
				select_new->select_table.tablename += POSTFIX;
				select_new->select_table.filename += POSTFIX;
				table_list.push_back(select_new->select_table);
				if(select_new->select_table.WriteFile())
				{
					ShowTable(select_new->select_table.tablename);
					return DropTable(select_new->select_table.tablename, true);
				}
			}
		}
		else if(index == select_new->To)
		{
			if(select_new->Select_Part(table_list))
			{
				select_new->table->tablename += POSTFIX;
				select_new->table->filename = select_new->select_filename;
				table_list.push_back(*(select_new->table));
				if(select_new->table->WriteFile())
				{
					ShowTable(select_new->table->tablename);
					return DropTable(select_new->table->tablename, false);
				}
			}			
		}
		// else
			return false;
		break;
	}
	case ERROR:
	{
		cout << "operation error happened, checkout your input and try again" << endl;
		return false;
		break;
	}
	case LIST:
	{	
		if(TrimALLSpace(input))
			ListTable();
		return true;
		break;
	}
	case SHOW:	//	show imformation of a table, just for easy-use
	{
		stringstream s(input);
		string temp;
		vector<string> temp_vector;
		while(s)
		{
			s >>temp;
			temp_vector.push_back(temp);
		}
		if(check(temp_vector[2]))
			ShowTable(temp_vector[2]);
		else
			return false;
		break;
	}
	}
}

int MySQL::Operator_ID(string &input)
{
	TrimSpace(input);
	if (input.find("CREATE") != input.npos && input.find("TABLE") != input.npos)
		return CREATETABLE;
	else if (input.find("DROP") != input.npos && input.find("TABLE") != input.npos)
		return DROPTABLE;
	else if (input.find("INSERT") != input.npos && input.find("INTO") != input.npos)
		return INSERTINTO;
	else if (input.find("DELETE") != input.npos)
		return DELETE;
	else if (input.find("UPDATE") != input.npos && input.find("SET") != input.npos)
		return UPDATE;
	else if (input.find("SELECT") != input.npos)
		return SELECT;
	else if (input.find("SHOW") != input.npos)	//最好用正则匹配一下
		return SHOW;
	else if (input.find("LIST") != input.npos)		//最好也用正则匹配一下
		return LIST;
	else
		return ERROR;
}

bool MySQL::ReadConfigFile()
{
	ifstream file(mapfile, ios::in);
	string temp_id;
	string temp;
	bool haserror = false;
	if(!file.is_open())
	{
		cout << "cannot open " << mapfile << endl;
		cout << "creating ……" << endl;
		ofstream tempfile(mapfile, ios::out);
		tempfile.close();
		cout << "done" << endl;
		return false;
	}
	else
	{
		cout << "reading " << mapfile << endl;
		while( !file.eof())
		{
			getline(file, temp);
			if(temp == "") //末尾是空行
				break;
			stringstream s(temp);
			Table *temp_table = new Table;
			s >> temp_table->tablename >> temp_table->filename; 
			// >> temp_table->column >> temp_table->row;
			// while(s)
			// {
			// 	s >> temp_id;
			// 	temp_table->id_list.push_back(temp_id);
			// }
			// temp_table->id_list.pop_back();


			//	 读入数据表
			create_new = new Create;
			create_new->table ->filename = temp_table->filename;
			if(!create_new->Create_from_file())//对应文件不存在了
			{
				haserror = true;
				// table_list.pop_back();
				// for(map<string, string>::iterator it = table_map.begin(); it != table_map.end(); it++)
				// 	if(it->second == temp_table->filename)
				// 		table_map.erase(it);
				cout << "no file called " << temp_table->filename << endl;
				cout << "deteled table:" << temp_table->tablename << " paird with" << temp_table->filename << endl;
			}
			else
			{
				temp_table->id_list.swap(create_new->table->id_list);////////////
				temp_table->data_list.swap(create_new->table->data_list);
				temp_table->width.swap(create_new->table->width);	
				temp_table->row = create_new->table->row;
				temp_table->column = create_new->table->column;/////////////
				table_list.push_back(*temp_table);
				table_map.insert(pair<string, string>(temp_table->tablename, temp_table->filename));				
			}
		}

		if(haserror)
			WriteConfigFile();	//更新 config 文件
		cout << "done" << endl;		
	}
	file.close();	
	return true;
}

bool MySQL::WriteConfigFile()
{
	ofstream file(mapfile, ios::out);
	string temp;
	//	Goods *newgood = new Goods;
	if(!file.is_open())
	{
		cout << "error: can not open " << mapfile << endl;
		return false;
	}
	else
	{
		for(vector<Table>::iterator it = table_list.begin(); it != table_list.end(); it++)
		{
			file <<  it->tablename << "   "  << it->filename << "  ";

			//////////////////////////////////////////////////////
			// 	 << it->column << "   " << it->row << "   ";
			// for(vector<string>::iterator i = it->id_list.begin(); i != it->id_list.end(); i++)
			// 	file << *i << " ";
			file << endl;
		}
	}
	file.close();

	return true;
}

bool MySQL::DropTable(string &tablename, bool deletefile)
{
	for(map<string, string>::iterator it = table_map.begin(); it != table_map.end(); it++)
		if(it->first == tablename)
			table_map.erase(it);
	for(vector<Table>::iterator it = table_list.begin(); it != table_list.end(); it++)
		if(it->tablename == tablename)
		{
			const char *temp_delete_name = it->filename.c_str();
			if(deletefile)
			{	
				if(remove(temp_delete_name))
				{				
					cout << "delete file : " << temp_delete_name << " failed" << endl;
					return false;
				}
			}
			table_list.erase(it);
			return WriteConfigFile();
		}

	cout << "No table named " << tablename << endl;
	return false;
}

bool MySQL::Register(Table * reg_table)// pay attention to the '_'
{
	ofstream file(mapfile, ios::app);
	string temp;
	//	Goods *newgood = new Goods;
	if(!file.is_open())
	{
		cout << "error: can not open " << mapfile << endl;
		//exit(0);
		return false;
	}
	else
	{
		file << reg_table->tablename << " " << reg_table->filename << endl;
	}
	file.close();
	table_map.insert(pair<string, string>(reg_table->tablename, reg_table->filename));
	table_list.push_back(*reg_table);
	
	return WriteConfigFile();
}

bool MySQL::check(string &tablename, string &filename)
{
	int haserror = 0;
	; // if both tablename and filename are not in table_map, return true
	for(map<string, string>::iterator it = table_map.begin(); it != table_map.end(); it++)
	{
		if(tablename == it->first)
		{
			cout << "error : table " << tablename << " exists, paired with " << it->second << endl;
			haserror++;
		}
		if(filename == it->second)
		{
			cout << "error : file " << filename << " exists, paired with " << it->first << endl;
			haserror++;
		}
	}
	if(mapfile == filename)
	{
		cout << "same name with init file, try another" << endl;
		haserror++;
	}

	if(!haserror)
		return true;
	else
	{	
		cout << haserror << " error";
		if(haserror>1)
			cout << "s";
		cout << " happened" << endl;
		return false;
	}
}

bool MySQL::check(string tablename)
{
	for(map<string, string>::iterator it = table_map.begin(); it != table_map.end(); it++)
		if(tablename == it->first)
			return true;
	cout << "no table is called " << tablename << endl;
	return false;
}

void MySQL::ListTable()
{
	unsigned int index = 1;
	cout << " total:" << table_list.size() << endl;
	for(vector<Table>::iterator it = table_list.begin(); it != table_list.end(); it++)
	{
		index = 1;
		cout << "  ";
		cout << OUTPUTFORMAT_1(WIDTH_1) <<  it->tablename 
			 << "(" << it->column << "," 
			 // << OUTPUTFORMAT_1(WIDTH_2) 
			 << it->row << ")  "
			 << "[";
		for(vector<string>::iterator i = it->id_list.begin(); i != it->id_list.end(); i++, index++)
		{
			cout << *i;
			if(index < it->column)
				cout << ",";
		}
		cout << "]" << endl;
		cout << RESETOUTPUTFORMAT;
	}
}

string MySQL::paired_filename(string tablename)
{
	for(map<string, string>::iterator it = table_map.begin(); it != table_map.end(); it++)
		if(tablename == it->first)
			return it->second;

	return NULL;
}

bool MySQL::ShowTable(string tablename)
{
	for(vector<Table>::iterator it = table_list.begin(); it != table_list.end();it++)
	{
		if(it->tablename == tablename)
			return it->ShowData();
	}

	cout <<"something wromg" << endl;
	cout << "no table is called " << tablename << endl;
	return false;
}


void MySQL::ShowHelp()
{
	cout << "-im here to help you !!" << endl;
	cout << "-input \"mySQL\" to enter the system" << endl;
	cout << "-input \"quit\" or \"QUIT\" to quit from the system and this console" << endl;
	cout << "-input \"SHOW TABLE TABLENAME\" to see datails of the table" << endl;
	cout << "-do not enter any string named \"None\" " << endl;
	cout << "-no space is admitted" << endl;
	cout << "-do not use keywords below:" << endl;
	cout << "-- \"CREATE\" \"DELETE\" \"DROP\" \"INSERT\" \"UPDATE\" \"SELECT\" \"SHOW\" \"LIST\"" << endl; 
	cout << "-you'd better not use words below:" << endl;
	cout << "--	\"WHERE\" \"TO\" \"FROM\" \"DISTINCT\" \"ORDER\" \"BY\" \"SET\" \"*\"" << endl; 
}


bool MySQL::READ()
{
	string input, temp;
	bool result;
	cout << "input filename: ";
	cin >> input;
	getchar();
	ifstream file(input, ios::in);
	if(!file.is_open())
	{
		cout << "cannot open " << input << endl;
		return false;
	}
	else
	{
		while(!file.eof())
		{
			getline(file, temp);
			result = Analyse(temp);
			cout << "executing :" << temp << endl;
			if(result)
				cout << "Succeed "<< endl;
			else
				cout << "Error happened" << endl;
			cout << "******************" << endl;	
		}
		return true;
	}
}