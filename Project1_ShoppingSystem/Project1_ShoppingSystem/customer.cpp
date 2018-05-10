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
	file->readfile(sgoods); //读取库存
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
			if(Checkout(glist))	//确认付款
				if(admin.Checkout(this, glist))
				{
					cout << "付款成功" << endl;
					clear();
				}
				else
					cout << "失败" << endl;
		}
		break;
	}
}

void Customer::List(Goodlist& glist)
{
	;//画线
	cout << OUTPUTFORMAT << "ID" << OUTPUTFORMAT <<  "名称" << OUTPUTFORMAT << "品牌" << OUTPUTFORMAT << "价格" << OUTPUTFORMAT << "数量" << endl;
	for(vector<Goods>::iterator it = glist.vgood.begin(); it != glist.vgood.end(); it++)
		if(it->num > 0)
			cout << OUTPUTFORMAT <<it->goodsid() << OUTPUTFORMAT << it->goodsname()<< OUTPUTFORMAT << it->goodsbrand()<< OUTPUTFORMAT << it->goodsprice()<< OUTPUTFORMAT << it->goodsnum() << endl;
	;//画线
}

void Customer::Search(Goodlist& glist)
{
	bool exist = false;
	string inputname;
	cout << "输入商品名称：";
	cin >> inputname;
	for(vector<Goods>::iterator it = glist.vgood.begin(); it !=glist.vgood.end(); it++)
		if(inputname == it->goodsname()&& it->goodsnum() != -1)
		{
			cout << OUTPUTFORMAT <<it->goodsid()<< OUTPUTFORMAT <<  it->goodsname() << OUTPUTFORMAT << it->goodsbrand()<< OUTPUTFORMAT << it->goodsprice()<< OUTPUTFORMAT << it->goodsnum() << endl;
			exist = true;
		}
	if(exist == false)			//可以继续搜索或直接退出，待改进
		cout << "不存在该商品，换个词试试？" << endl;
}

void Customer::ListGoods()
{
	;//画线
	cout << OUTPUTFORMAT << "ID" << OUTPUTFORMAT <<  "名称" << OUTPUTFORMAT << "品牌" << OUTPUTFORMAT << "价格" << OUTPUTFORMAT << "数量" << endl;
	for(vector<Goods>::iterator it = sgoods.begin(); it != sgoods.end(); it++)
		if(it->goodsnum())	//数量大于0
			cout << OUTPUTFORMAT <<it->goodsid() << OUTPUTFORMAT << it->goodsname()<< OUTPUTFORMAT << it->goodsbrand()<< OUTPUTFORMAT << it->goodsprice()<< OUTPUTFORMAT << it->goodsnum() << endl;
	;//画线
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
	cout << "输入商品ID和数量：" ;
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
				cout << "successfully add" << endl;//已经加入购物车
			}
			else
				cout << "数量不足" << endl;
		}
		if(exist == false)
			cout << "无此商品" << endl;
		update();
}

void Customer::Remove()
{
	bool exist = false;
	string inputID;
	int inputnum;
	cout << "输入商品ID和数量：" ;
	cin >> inputID >> inputnum;

	for(vector<Goods>::iterator it = sgoods.begin(); it != sgoods.end(); it++)
		if(it->goodsid() == inputID)
		{
			exist = true;
			if(it->goodsnum() >= inputnum)
			{
				it->setnum(it->goodsnum() - inputnum);
				update();
				cout << "删除成功" << endl;
			}
			else
				cout << "数量不足" << endl;
		}
		if(exist == false)
			cout << "无此商品" << endl;
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
					cout << it->goodsname() << "(" << it->goodsbrand() << ")" << "超过库存量" << endl;
					cout << "库存剩余" << origin->goodsnum() << endl;
					cout << "请适当减少数量后再次结账" << endl;
					return false;
				}
				sum += it ->goodsprice() * it->goodsnum();
			}
	
	//执行到这步，说明所有商品数量都不大于库存，可以进行确认付款
	cout << "一共需要付款 " << sum << "!确认付款请按 1， 取消请按 0：";
	cin >> choice;

	return choice == 1;
}

void Customer::clear()
{
	sgoods.clear();
	file->writefile(sgoods);
}