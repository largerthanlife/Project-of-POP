#ifndef _BASE_H_
#define _BASE_H_

#include "file.h"
#include "goods.h"
#include <map>
#include <list>
#include <string>

using std::string;
using std::map;
using std::list;

#define FIELDWIDTH 10
#define OUTPUTFORMAT setiosflags(ios::left)<< std::setw(FIELDWIDTH) //定义输出格式


//class File;

class Base
{
protected:
	File *file;
	string ID;			
	string password;
	bool isadmin;		//是否为管理员
	string storefile;	//库存文件名
	string soldfile;	//已售清单文件名
public:
//	virtual void operationlist(int, Goodlist&) = 0; //根据操作号，调用不同函数
	virtual void List(Goodlist&) = 0;				//查询商品接口 可统一实现
	virtual void Add(Goodlist&) = 0;				//分别实现
//	virtual void Remove() = 0;						//分别实现
//	virtual void Read() = 0;						//调用 file 读取对应文件
};

class Customer; //前向声明

class Admin:public Base
{
private:
	vector<Customer> vcustomer;			//所有已注册用户
	vector<string> shoppingnames;		//用户购物车名
	map<string, string> customerlist;	//保存用户名和密码
	string customerfile;				//初始化为 用户.txt
	string soldoutfile;					//售出清单的文件名
public:
	Admin();
	~Admin();
	bool Login();
	void operationlist(int, Goodlist&);			//操作列表
	void List(Goodlist&);						//列出所有商品
	void Logout();								//注销
	void Modify(Goodlist&);						//修改商品信息
	void Add(Goodlist&);						//在库存文件里添加商品
	void Remove(Goodlist&);						//在库存文件里删除商品
	void Modifycustom(string);					//修改用户购物车数据
	bool Addcustom(Customer*&, bool&);			//添加用户记录
	void ListSold();							//列出已售出商品
	void customerupdate();						//更新 用户.txt
	void setID(string newID){ID = newID;}		//修改用户名
	bool Checkout(Customer*const&,Goodlist&);		//为某位顾客结账
};

class Customer:public Base
{
private:
	string boughtfile;				//已买文件
	string shoppingfile;			//购物车文件名
public:
	Customer();
	~Customer();
	vector<Goods> sgoods;			//购物车现有商品
	void setID(string newID){ID = newID;}
	void setpassword(string newpassword){password = newpassword;}
	string getID(){return ID;}
	string getpassword(){return password;}

	void Login();
	void operationlist(int, Goodlist&, Admin&);
	void Search(Goodlist& );		//查询某一商品
	void Add(Goodlist&);			//在购物车文件里添加商品
	void Remove();					//在购物车文件里删除商品
	void ListGoods();				//查看购物车文件
	bool Checkout(Goodlist&);				//结算
	void List(Goodlist&);			//列出所有（可购买）商品
	void update();					//更新对应的购物车文件
	void deletegoods(string);				//删除购物车里的某一指定商品
	void clear();					//清空购物车
};


#endif