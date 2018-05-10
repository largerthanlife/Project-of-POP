#include "base.h"
#include "goods.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <map>

#define FIELDWIDTH 10
#define LOGOUT 0
#define LISTALL 1
#define ADD 2
#define DELETE 3
#define MODIFY 4
#define LISTSOLDOUT 5
#define SEARCH 2
#define ADDGOODS 3
#define	DELETEGOODS 4
#define LISTGOODS 5
#define CHECKOUT 6

using std::map;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ios;
using std::pair;


Customer::Customer()
{
	file = new File;
}

Customer::~Customer()
{

}

void Customer::Login()
{
	shoppingfile = ID + ".txt";
	file->filename = shoppingfile;
	file->readfile(sgoods); //��ȡ���
}

void Customer::operationlist(int index, Goodlist& glist, Admin& admin)
{
	switch (index)
	{
	case LOGOUT:
		break;
	case LISTALL:
		{
			List(glist);
		}
		break;
	case SEARCH:
		{
			Search(glist);
		}
		break;
	case ADDGOODS:
		{
			Add(glist);
			ListGoods();
		}
		break;
	case DELETEGOODS:
		{
			Remove();
			ListGoods();
		}
		break;
	case LISTGOODS:
		{
			ListGoods();
		}
		break;
	case CHECKOUT:
		{
			if(Checkout(glist))	//ȷ�ϸ���
				if(admin.Checkout(this, glist))
				{
					cout << "����ɹ�" << endl;
					clear();
				}
				else
					cout << "ʧ��" << endl;
		}
		break;
	}
}

void Customer::List(Goodlist& glist)
{
	;//����
	cout << OUTPUTFORMAT << "ID" << OUTPUTFORMAT <<  "����" << OUTPUTFORMAT << "Ʒ��" << OUTPUTFORMAT << "�۸�" << OUTPUTFORMAT << "����" << endl;
	for(vector<Goods>::iterator it = glist.vgood.begin(); it != glist.vgood.end(); it++)
		if(it->num > 0)
			cout << OUTPUTFORMAT <<it->goodsid() << OUTPUTFORMAT << it->goodsname()<< OUTPUTFORMAT << it->goodsbrand()<< OUTPUTFORMAT << it->goodsprice()<< OUTPUTFORMAT << it->goodsnum() << endl;
	;//����
}

void Customer::Search(Goodlist& glist)
{
	bool exist = false;
	string inputname;
	cout << "������Ʒ���ƣ�";
	cin >> inputname;
	for(vector<Goods>::iterator it = glist.vgood.begin(); it !=glist.vgood.end(); it++)
		if(inputname == it->goodsname()&& it->goodsnum() != -1)
		{
			cout << OUTPUTFORMAT <<it->goodsid()<< OUTPUTFORMAT <<  it->goodsname() << OUTPUTFORMAT << it->goodsbrand()<< OUTPUTFORMAT << it->goodsprice()<< OUTPUTFORMAT << it->goodsnum() << endl;
			exist = true;
		}
	if(exist == false)			//���Լ���������ֱ���˳������Ľ�
		cout << "�����ڸ���Ʒ�����������ԣ�" << endl;
}

void Customer::ListGoods()
{
	;//����
	cout << OUTPUTFORMAT << "ID" << OUTPUTFORMAT <<  "����" << OUTPUTFORMAT << "Ʒ��" << OUTPUTFORMAT << "�۸�" << OUTPUTFORMAT << "����" << endl;
	for(vector<Goods>::iterator it = sgoods.begin(); it != sgoods.end(); it++)
		if(it->goodsnum())	//��������0
			cout << OUTPUTFORMAT <<it->goodsid() << OUTPUTFORMAT << it->goodsname()<< OUTPUTFORMAT << it->goodsbrand()<< OUTPUTFORMAT << it->goodsprice()<< OUTPUTFORMAT << it->goodsnum() << endl;
	;//����
}

void Customer::update()
{
	file->writefile(sgoods);
}

void Customer::Add(Goodlist& glist)
{
	bool exist = false;
	Goods* newgood = new Goods;
	string inputID;
	int inputnum;
	cout << "������ƷID��������" ;
	cin >> inputID >> inputnum;

	for(vector<Goods>::iterator it = glist.vgood.begin(); it != glist.vgood.end(); it++)
		if(it->goodsid() == inputID)
		{
			exist = true;
			if(it->goodsnum() >= inputnum)
			{
				newgood->setnum(inputnum);
				newgood->setID(inputID);
				newgood->setname(it->goodsname());
				newgood->setbrand(it->goodsbrand());
				newgood->setprice(it->goodsprice());
				sgoods.push_back(*newgood);
				cout << "successfully add" << endl;//�Ѿ����빺�ﳵ
			}
			else
				cout << "��������" << endl;
		}
		if(exist == false)
			cout << "�޴���Ʒ" << endl;
		update();
}

void Customer::Remove()
{
	bool exist = false;
	string inputID;
	int inputnum;
	cout << "������ƷID��������" ;
	cin >> inputID >> inputnum;

	for(vector<Goods>::iterator it = sgoods.begin(); it != sgoods.end(); it++)
		if(it->goodsid() == inputID)
		{
			exist = true;
			if(it->goodsnum() >= inputnum)
			{
				it->setnum(it->goodsnum() - inputnum);
				update();
				cout << "ɾ���ɹ�" << endl;
			}
			else
				cout << "��������" << endl;
		}
		if(exist == false)
			cout << "�޴���Ʒ" << endl;
}

void Customer::deletegoods(string ID)
{
	for(vector<Goods>::iterator it = sgoods.begin(); it != sgoods.end(); it++)
		if(it->goodsid() == ID)
		{
			sgoods.erase(it);
			update();
			break;
		}
}

bool Customer::Checkout(Goodlist& glist)
{
	ListGoods();
	float sum = 0;
	int choice;

	for(vector<Goods>::iterator it = sgoods.begin(); it != sgoods.end(); it++)
		for(vector<Goods>::iterator origin = glist.vgood.begin(); origin != glist.vgood.end(); origin++)
			if(it->goodsid() == origin->goodsid())
			{
				if(it->goodsnum() > origin->goodsnum())
				{	
					cout << it->goodsname() << "(" << it->goodsbrand() << ")" << "���������" << endl;
					cout << "���ʣ��" << origin->goodsnum() << endl;
					cout << "���ʵ������������ٴν���" << endl;
					return false;
				}
				sum += it ->goodsprice() * it->goodsnum();
			}
	
	//ִ�е��ⲽ��˵��������Ʒ�����������ڿ�棬���Խ���ȷ�ϸ���
	cout << "һ����Ҫ���� " << sum << "!ȷ�ϸ����밴 1�� ȡ���밴 0��";
	cin >> choice;

	return choice == 1;
}

void Customer::clear()
{
	sgoods.clear();
	file->writefile(sgoods);
}