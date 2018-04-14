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


string append(string origin)//ID 递增
{
	string temp(origin);
	int i = origin.length() - 1;
	bool cf = true;	//carry flag
	int flag = 1;	//若有进位标志则为1， 第一次默认为1
	while(i>=0)	//ppt 上规定用户不超过20个，故没有考虑 F + 1 = G 的情况
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

	customerfile = "用户.txt";	//读取已有用户资料，保存到 customerlist 中
	file->filename = customerfile;
	file->readfile(customerlist, shoppingnames);
}

bool Admin::Login()
{
	string inputID;
	cout << "输入管理员ID:";
	cin >> inputID;
	getchar();
	string inputpwd;
	cout << "输入管理员密码:";
	cin >> inputpwd;
	getchar();
	if(ID == inputID)
		if(password == inputpwd)
			return true;
		else
			cout << "密码错误,请重试" << endl;
	else
		cout << "ID错误, 请重试" << endl;
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
	bool exist = false; //是否存在该商品

	cout << "输入新商品的名称、品牌、价格以及数量:" << endl;
	cin >> newgood->name >> newgood->brand >> newgood->price >> newgood->num;

	//遍历已有商品列表，如果名称、品牌、价格都符号，则修改对应数量，否则 ID 加1 
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
	cout << "输入需要删除的商品的名词、品牌、价格：" << endl;
	Goods *newgood = new Goods;
	cin >> newgood->name >> newgood->brand >> newgood->price;
	for(vector<Goods>::iterator it = glist.vgood.begin(); it != glist.vgood.end();it++)
		if(it->name == newgood->name && it->brand == newgood->brand && it->price == newgood->price)
		{
			it->num = -1;
			glist.update();//
			Modifycustom(it->ID);		//	修改用户购物车数据
		}
}

void Admin::Modify(Goodlist& glist)
{
	bool exist= false;
	Goods *newgood = new Goods;
	cout << "输入需要修改的商品的名称和品牌:" << endl;
	cin >> newgood->name >> newgood->brand;
	for(vector<Goods>::iterator it = glist.vgood.begin(); it != glist.vgood.end();it++)
		if(it->name == newgood->name && it->brand == newgood->brand)
		{
			exist = true;
			cout << "商品存在，其原有数量和价格是：" << it->num << "&" << it->price << endl; 
			cout << "输入商品的数量和价格(空格分割):";
			cin >> newgood->num >> newgood->price;
			if(newgood->price > 0) //还需要进一步检查输入
			{
				it->num = newgood->num;
				it->price = newgood->price;
			}
			glist.update();
			break;
		}
		if(!exist)
			cout << "没有符合条件的商品" << endl;
}

void Admin::Modifycustom(string ID)//遍历删除所有购物车对应商品
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
	;//画线
	cout << OUTPUTFORMAT << "ID" << OUTPUTFORMAT <<  "名称" << OUTPUTFORMAT << "品牌" << OUTPUTFORMAT << "价格" << OUTPUTFORMAT << "数量" << endl;
	for(vector<Goods>::iterator it = glist.vgood.begin(); it != glist.vgood.end(); it++)
		cout << OUTPUTFORMAT <<it->goodsid() << OUTPUTFORMAT << it->goodsname()<< OUTPUTFORMAT << it->goodsbrand()<< OUTPUTFORMAT << it->goodsprice()<< OUTPUTFORMAT << it->goodsnum() << endl;
	;//画线
}

bool Admin::Addcustom(Customer*& newcutomer, bool& isreg)
{
	string inputID;
	cout << "输入用户名:";
	cin >> inputID;
	newcutomer->setID(inputID);
	getchar();

	string inputpwd;
	cout << "输入密码:";
	cin >> inputpwd;
	newcutomer->setpassword(inputpwd);
	getchar();

	//遍历所有已注册的用户，寻找匹配项
	for(map<string, string>::iterator it = customerlist.begin(); it != customerlist.end(); it++)
		if(it->first == newcutomer->getID())
		{
			if(it->second != newcutomer->getpassword())
				//	while(it->second != newcutomer->getpassword()) //如果一直不记得密码 怎么办
			{
				if(isreg == false)
					cout << "密码输入错误，请重试" << endl;
				else
					cout << "ID已被注册，换一个试试" << endl;
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

		//这是新用户注册的操作
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
			cout << "不存在这个符号条件的账户" << endl;
			return false;
		}
}

void Admin::customerupdate()
{
	file->filename = "用户.txt";
	file->writefile(customerlist);
}

bool Admin::Checkout(Customer* const& topay, Goodlist& glist)
{
	//比照库存和购物车的所有商品， 以此减少库存
	for(vector<Goods>::iterator topayit = topay->sgoods.begin(); topayit != topay->sgoods.end(); topayit++)
		for(vector<Goods>::iterator originit = glist.vgood.begin(); originit != glist.vgood.end(); originit++)
			if(topayit->goodsid() == originit->goodsid())
			{
				originit-> setnum(originit->goodsnum() - topayit->goodsnum());	//	计算剩余量
			}
	
	//修改已售清单
	file->filename = "已售清单.txt";
	file->writesoldoutfile(topay->sgoods, topay->getID());
	
	//更新库存列表
	glist.update();
	return true;
}

void Admin::ListSold()
{
	file->filename = "已售清单.txt";
	file->readsoldoutfile();
}
