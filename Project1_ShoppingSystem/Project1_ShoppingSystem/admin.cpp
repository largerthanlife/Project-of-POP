#include "base.h"
#include "goods.h"
#include <string>
#include <iostream>
#include <iomanip>
#include <ctype.h>
#include <map>

#define LOGOUT 0
#define LISTALL 1
#define ADD 2
#define DELETE 3
#define MODIFY 4
#define LISTSOLDOUT 5

using std::map;
using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::ios;
using std::pair;


string append(string origin)//ID ����
{
	string temp(origin);
	int i = origin.length() - 1;
	bool cf = true;	//carry flag
	int flag = 1;	//���н�λ��־��Ϊ1�� ��һ��Ĭ��Ϊ1
	while(i>=0)	//ppt �Ϲ涨�û�������20������û�п��� F + 1 = G �����
	{
		temp[i] = (((origin[i]+flag)>'9')?(flag = 1,cf = true, '0'):(flag = cf?1:0,cf = false,origin[i]+(flag--)));
		i--;
	}
	return temp;
}

Admin::Admin()
{
	file = new File; 
	ID = "admin";	
	password = "admin";

	customerfile = "�û�.txt";	//��ȡ�����û����ϣ����浽 customerlist ��
	file->filename = customerfile;
	file->readfile(customerlist, shoppingnames);
}

bool Admin::Login()
{
	string inputID;
	cout << "�������ԱID:";
	cin >> inputID;
	getchar();
	string inputpwd;
	cout << "�������Ա����:";
	cin >> inputpwd;
	getchar();
	if(ID == inputID)
		if(password == inputpwd)
			return true;
		else
			cout << "�������,������" << endl;
	else
		cout << "ID����, ������" << endl;
	return false;
}

void Admin::operationlist(int index, Goodlist& glist)
{
	switch (index)
	{
	case LOGOUT:
		{
			Logout();
		}
		break;
	case LISTALL:
		{
			List(glist);
		}
		break;
	case ADD:
		{
			Add(glist);
			List(glist);
		}
		break;
	case DELETE:
		{
			Remove(glist);
			List(glist);
		}
		break;
	case MODIFY:
		{
			Modify(glist);
			List(glist);
		}
		break;
	case LISTSOLDOUT:
		{
			ListSold();
		}
		break;
	}
}

void Admin::Logout()
{

}

void Admin::Add(Goodlist &glist)
{
	//string newID, newname, newbrand, newprice, newnum;
	Goods *newgood = new Goods;
	string newID;
	bool exist = false; //�Ƿ���ڸ���Ʒ

	cout << "��������Ʒ�����ơ�Ʒ�ơ��۸��Լ�����:" << endl;
	cin >> newgood->name >> newgood->brand >> newgood->price >> newgood->num;

	//����������Ʒ�б�������ơ�Ʒ�ơ��۸񶼷��ţ����޸Ķ�Ӧ���������� ID ��1 
	for(vector<Goods>::iterator it = glist.vgood.begin(); it != glist.vgood.end(); newID = it->ID, it++)
		if(it->name == newgood->name && it->brand == newgood->brand && it->price == newgood->price)
		{
			if(it->num != -1)
				it->num += newgood->num;
			else
				it->num = newgood->num;
			exist = true;
		}
	if(exist == false)
	{
		newgood->ID = append(newID);
		glist.vgood.push_back(*newgood);
	}
	glist.update();
}

void Admin::Remove(Goodlist &glist)
{
	cout << "������Ҫɾ������Ʒ�����ʡ�Ʒ�ơ��۸�" << endl;
	Goods *newgood = new Goods;
	cin >> newgood->name >> newgood->brand >> newgood->price;
	for(vector<Goods>::iterator it = glist.vgood.begin(); it != glist.vgood.end();it++)
		if(it->name == newgood->name && it->brand == newgood->brand && it->price == newgood->price)
		{
			it->num = -1;
			glist.update();//
			Modifycustom(it->ID);		//	�޸��û����ﳵ����
		}
}

void Admin::Modify(Goodlist& glist)
{
	bool exist= false;
	Goods *newgood = new Goods;
	cout << "������Ҫ�޸ĵ���Ʒ�����ƺ�Ʒ��:" << endl;
	cin >> newgood->name >> newgood->brand;
	for(vector<Goods>::iterator it = glist.vgood.begin(); it != glist.vgood.end();it++)
		if(it->name == newgood->name && it->brand == newgood->brand)
		{
			exist = true;
			cout << "��Ʒ���ڣ���ԭ�������ͼ۸��ǣ�" << it->num << "&" << it->price << endl; 
			cout << "������Ʒ�������ͼ۸�(�ո�ָ�):";
			cin >> newgood->num >> newgood->price;
			if(newgood->price > 0) //����Ҫ��һ���������
			{
				it->num = newgood->num;
				it->price = newgood->price;
			}
			glist.update();
			break;
		}
		if(!exist)
			cout << "û�з�����������Ʒ" << endl;
}

void Admin::Modifycustom(string ID)//����ɾ�����й��ﳵ��Ӧ��Ʒ
{
	for(map<string, string>::iterator it = customerlist.begin(); it != customerlist.end(); it++)
	{
		Customer *newcustomer = new Customer;
		newcustomer->setID(it->first);
		newcustomer->Login();
		newcustomer->deletegoods(ID);
	}
}

void Admin::List(Goodlist& glist)
{
	;//����
	cout << OUTPUTFORMAT << "ID" << OUTPUTFORMAT <<  "����" << OUTPUTFORMAT << "Ʒ��" << OUTPUTFORMAT << "�۸�" << OUTPUTFORMAT << "����" << endl;
	for(vector<Goods>::iterator it = glist.vgood.begin(); it != glist.vgood.end(); it++)
		cout << OUTPUTFORMAT <<it->goodsid() << OUTPUTFORMAT << it->goodsname()<< OUTPUTFORMAT << it->goodsbrand()<< OUTPUTFORMAT << it->goodsprice()<< OUTPUTFORMAT << it->goodsnum() << endl;
	;//����
}

bool Admin::Addcustom(Customer*& newcutomer, bool& isreg)
{
	string inputID;
	cout << "�����û���:";
	cin >> inputID;
	newcutomer->setID(inputID);
	getchar();

	string inputpwd;
	cout << "��������:";
	cin >> inputpwd;
	newcutomer->setpassword(inputpwd);
	getchar();

	//����������ע����û���Ѱ��ƥ����
	for(map<string, string>::iterator it = customerlist.begin(); it != customerlist.end(); it++)
		if(it->first == newcutomer->getID())
		{
			if(it->second != newcutomer->getpassword())
				//	while(it->second != newcutomer->getpassword()) //���һֱ���ǵ����� ��ô��
			{
				if(isreg == false)
					cout << "�����������������" << endl;
				else
					cout << "ID�ѱ�ע�ᣬ��һ������" << endl;
				return false;
				//cin >> inputpwd;
				//newcutomer->setpassword(inputpwd);
			}
			else
			{
				//	shoppingnames.push_back(it->first+".txt");
				//	newcutomer->Login();
				isreg = false;
				newcutomer->Login();
				return true;
			}
		}

		//�������û�ע��Ĳ���
		if(isreg)
		{
			isreg = true;
			customerlist.insert(pair<string, string>(newcutomer->getID(), newcutomer->getpassword()));
			shoppingnames.push_back(newcutomer->getID() + ".txt");
			file->creatfile(newcutomer->getID()+".txt");
			customerupdate();
			newcutomer->Login();
			return true;
		}
		else
		{
			cout << "��������������������˻�" << endl;
			return false;
		}
}

void Admin::customerupdate()
{
	file->filename = "�û�.txt";
	file->writefile(customerlist);
}

bool Admin::Checkout(Customer* const& topay, Goodlist& glist)
{
	//���տ��͹��ﳵ��������Ʒ�� �Դ˼��ٿ��
	for(vector<Goods>::iterator topayit = topay->sgoods.begin(); topayit != topay->sgoods.end(); topayit++)
		for(vector<Goods>::iterator originit = glist.vgood.begin(); originit != glist.vgood.end(); originit++)
			if(topayit->goodsid() == originit->goodsid())
			{
				originit-> setnum(originit->goodsnum() - topayit->goodsnum());	//	����ʣ����
			}
	
	//�޸������嵥
	file->filename = "�����嵥.txt";
	file->writesoldoutfile(topay->sgoods, topay->getID());
	
	//���¿���б�
	glist.update();
	return true;
}

void Admin::ListSold()
{
	file->filename = "�����嵥.txt";
	file->readsoldoutfile();
}
