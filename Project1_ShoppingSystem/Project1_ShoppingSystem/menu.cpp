#include "menu.h"
#include <iostream>

#define WRONG -1
int const WindowLength = 120;

using std::cout;
using std::endl;
using std::cin;

int Menu::showlogin()
{
	showline();
	
	cout << "1.用户登录" << "\t" << "2.用户注册" << "\t" << "3.管理员登录" << endl;

	showline();

	int choice;
	cout << "输入操作:";
	cin >> choice;
	getchar();
	if(choice >= 0 && choice <= 3)
		return choice;
	else
		return WRONG;
}

void Menu::showsucceed(bool isreg)
{
	int length = isreg?16:8;

	for(int i = 0; i < length; i ++)
		cout << "*";
	cout << endl;

	if(isreg)
		cout << "注册成功！登录成功！" << endl;
	else
		cout << "登录成功！" << endl;

	for(int i = 0; i < length; i ++)
		cout << "*";
	cout << endl;
}

int Menu::showadminmenu()
{
	showline();

	cout << "0.注销登录" << " " << "1.查询商品" << " " << "2.增加商品" << " " << "3.删除商品" << " " << "4.修改商品数量" << " " << "5.售货清单" << endl;

	showline();

	int choice;
	cout << "输入操作:";
	cin >> choice;
	getchar();
	if(choice >= 0 && choice <= 5)
		return choice;
	else
		return WRONG;
}

int Menu::showcustommenu()
{
	showline();

	cout << "0.注销登录" << "\t" << "1.查看商品" << "\t" << "2.商品搜索" << "\t" << "3.添加商品至购物车" << "\t" << "4.删除购物车商品" 
			<< "\t" << "5.查看购物车" << "\t" << "6.结账" << endl;

	showline();

	int choice;
	cout << "输入操作:";
	cin >> choice;
	getchar();
	if(choice >= 0 && choice <= 6)
		return choice;
	else
	{
		cout << "输入有误，请重新选择" << endl;
		return WRONG;
	}
}

void Menu::showline()
{
	for(int i = 0; i < WindowLength; i++)
		cout << "=";
	cout << endl;
}