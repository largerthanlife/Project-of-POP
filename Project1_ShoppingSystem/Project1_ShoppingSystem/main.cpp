//Author:	largerthanlife
//Time:		2018/04/04 11:55:18
//Filename:	main.cpp
//检查文件名对应情况
#include <iostream>
#include <cstdlib>
#include <vector>
#include "base.h"
#include "goods.h"
#include "menu.h"

#define CUSTOMER_LOGIN 1
#define CUSTOMER_REG 2
#define ADMIN_LOGIN 3
#define WRONG -1
#define EXIT 0

using std::cout;
using std::endl;
using std::vector;

int main()
{
	system("mode con cols=120 lines=30");
	system("color F0");
	int choice = 1;	// 非负值初始化 使之进入主循环
	bool isreg = false;
	Menu *mainmenu = new Menu;
	Admin *newadmin = new Admin;
	Goodlist *glist = new Goodlist;		//库存清单

	while(choice != EXIT)  //程序主循环
	{
		switch(choice = mainmenu->showlogin()) 
		{
		case CUSTOMER_REG:
			{
				isreg = true;
			}
			///////////////////////////注意这里没有 break， 因为用户注册与登录实际只有开头是不同的
		case CUSTOMER_LOGIN:
			{
				Customer *newcustomer = new Customer; 
				if(newadmin->Addcustom(newcustomer, isreg))
				{
					mainmenu->showsucceed(isreg);
					;
					while(choice)
					{
						choice = mainmenu->showcustommenu();
						if(choice == WRONG)
							;//错误输入处理
						else if(choice == EXIT)
						{
							choice = 1;
							break;
						}
						else
						{
							newcustomer->operationlist(choice, *glist, *newadmin);
						}
					}
				}
			}
			break;

		case ADMIN_LOGIN:
			{
				if(newadmin->Login())
				{
					mainmenu->showsucceed(false);
					; //显示登录成功 跳转操作
					while(choice)
					{
						choice = mainmenu->showadminmenu();
						if(choice == WRONG)
							;
						else if(choice == EXIT)
						{
							choice  = 1;	//给 choice 赋值不能为零， 因为只退出本次主循环，不退出程序
							break;
						}
						else
						{
							newadmin->operationlist(choice, *glist);
						}
					}

				}
				else
				{;}
			}
			break;

		case WRONG:
			{
				cout << "Wrong index, input again" << endl;
			}
			break;

		case EXIT: 
			break;
		}
	}
	return 0;
}
