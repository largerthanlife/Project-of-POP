#include "file.h"
#include "goods.h"
#include "base.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <map>

using std::cout;
using std::cin;
using std::endl;
using std::vector;
using std::string;
using std::map;
using std::stringstream;
using std::ifstream;
using std::ofstream;
using std::ios;
using std::pair;

void File::readfile(vector<Goods> & vgood)
{
	ifstream file(filename, ios::in);
	string temp;
	Goods *newgood = new Goods;

	if(file == nullptr)
	{
		cout << "can not open " << filename << endl;
		exit(0);
	}
	else
	{
		getline(file, temp); //������һ��
		while( !file.eof())
		{
			getline(file, temp);
			if(temp == "") //ĩβ�ǿ���
				break;
			stringstream s(temp);
			s >> newgood->ID >> newgood->name >> newgood->brand >> newgood->price >> newgood->num;
			vgood.push_back(*newgood);
			//cout << newgood->ID << newgood->name << newgood->brand << newgood->price << newgood->num << endl;

		}
	}
	file.close();
}

void File::readfile(map<string, string> &custommap, vector<string> &shoppingnames)//���� �û�.txt
{
	ifstream file(filename, ios::in);
	string temp;
	string tempname;
	string temppwd;

	if(file == nullptr)
	{
		cout << "can not open " << filename << endl;
		exit(0);
	}
	else
	{
		getline(file, temp); //������һ��
		while( !file.eof())
		{
			getline(file, temp);
			if(temp == "") //ĩβ�ǿ���
				break;
			stringstream s(temp);
			s >> tempname >> temppwd;
			custommap.insert(pair<string, string>(tempname, temppwd));
			shoppingnames.push_back(tempname+".txt");
		//	cout << tempname << "\t" << temppwd << endl;
		}
	}
	file.close();
}

void File::readsoldoutfile()
{
	ifstream file(filename, ios::in);
	string temp;
	Goods *newgood = new Goods;
	string username;

	if(file == nullptr)
	{
		cout << "can not open " << filename << endl;
		exit(0);
	}
	else
	{
		cout << OUTPUTFORMAT << "ID" << OUTPUTFORMAT <<  "����" << OUTPUTFORMAT << "Ʒ��" << OUTPUTFORMAT << "�۸�" << OUTPUTFORMAT << "����" << OUTPUTFORMAT << "�û���"<<endl;
		getline(file, temp); //������һ��
		while( !file.eof())
		{
			getline(file, temp);
			if(temp == "") //ĩβ�ǿ���
				break;
			stringstream s(temp);
			s >> newgood->ID >> newgood->name >> newgood->brand >> newgood->price >> newgood->num >> username;
			cout << OUTPUTFORMAT << newgood->goodsid() << OUTPUTFORMAT << newgood->goodsname() << OUTPUTFORMAT << newgood->goodsbrand() << OUTPUTFORMAT << newgood->goodsprice() << OUTPUTFORMAT << newgood->goodsnum() << username << endl;
		}
	}
	file.close();
}

void File::writefile(vector<Goods> &vgood)
{
	ofstream file(filename, ios::out);
	string temp;
//	Goods *newgood = new Goods;

	if(file == nullptr)
	{
		cout << "can not open " << filename << endl;
		exit(0);
	}
	else
	{
		file << "ID" << "\t" << "����" << "\t" << "Ʒ��" << "\t" << "�۸�" << "\t" << "����" << endl;
		for(vector<Goods>::iterator it = vgood.begin(); it != vgood.end(); it++)
			if(it->goodsnum())//������0��д��
				file << it->ID << "\t" << it->name << "\t" << it->brand << "\t" << it->price << "\t" <<  it->num << endl;
	}
	file.close();
}

void File::writefile(map<string, string> &mlist)
{
	ofstream file(filename, ios::out);
	string temp;
	//	Goods *newgood = new Goods;

	if(file == nullptr)
	{
		cout << "can not open " << filename << endl;
		exit(0);
	}
	else
	{
		file << "�û���" << "\t" << "����" << endl;
		for(map<string, string>::iterator it = mlist.begin(); it != mlist.end(); it++)
			file << it->first << "\t" << it->second << endl;
	}
	file.close();
}

void File::writesoldoutfile(vector<Goods> &vgood, string username)	//Ӧ��Ҫ�ϲ�ͬ��������ǵ���׷��
{
	ofstream file(filename, ios::app);
	string temp;
	//	Goods *newgood = new Goods;

	if(file == nullptr)
	{
		cout << "can not open " << filename << endl;
		exit(0);
	}
	else
	{
		for(vector<Goods>::iterator it = vgood.begin(); it != vgood.end(); it++)
			if(it->goodsnum())//������0��д��
				file << it->ID << "\t" << it->name << "\t" << it->brand << "\t" << it->price << "\t" <<  it->num << "\t" << username << endl;
	}
	file.close();
}

void File::creatfile(string newfilename)
{
	ofstream file(newfilename, ios::out);
	file << "ID" << "\t" << "����" << "\t" << "Ʒ��" << "\t" << "�۸�" << "\t" << "����" << endl;
	file.close();
}