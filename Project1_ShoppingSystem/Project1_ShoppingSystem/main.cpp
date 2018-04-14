//Author:	largerthanlife
//Time:		2018/04/04 11:55:18
//Filename:	main.cpp
//����ļ�����Ӧ���
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
	int choice = 1;	// �Ǹ�ֵ��ʼ�� ʹ֮������ѭ��
	bool isreg = false;
	Menu *mainmenu = new Menu;
	Admin *newadmin = new Admin;
	Goodlist *glist = new Goodlist;		//����嵥

	while(choice != EXIT)  //������ѭ��
	{
		switch(choice = mainmenu->showlogin()) 
		{
		case CUSTOMER_REG:
			{
				isreg = true;
			}
			///////////////////////////ע������û�� break�� ��Ϊ�û�ע�����¼ʵ��ֻ�п�ͷ�ǲ�ͬ��
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
							;//�������봦��
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
					; //��ʾ��¼�ɹ� ��ת����
					while(choice)
					{
						choice = mainmenu->showadminmenu();
						if(choice == WRONG)
							;
						else if(choice == EXIT)
						{
							choice  = 1;	//�� choice ��ֵ����Ϊ�㣬 ��Ϊֻ�˳�������ѭ�������˳�����
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
