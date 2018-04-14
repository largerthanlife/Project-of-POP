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
	
	cout << "1.�û���¼" << "\t" << "2.�û�ע��" << "\t" << "3.����Ա��¼" << endl;

	showline();

	int choice;
	cout << "�������:";
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
		cout << "ע��ɹ�����¼�ɹ���" << endl;
	else
		cout << "��¼�ɹ���" << endl;

	for(int i = 0; i < length; i ++)
		cout << "*";
	cout << endl;
}

int Menu::showadminmenu()
{
	showline();

	cout << "0.ע����¼" << " " << "1.��ѯ��Ʒ" << " " << "2.������Ʒ" << " " << "3.ɾ����Ʒ" << " " << "4.�޸���Ʒ����" << " " << "5.�ۻ��嵥" << endl;

	showline();

	int choice;
	cout << "�������:";
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

	cout << "0.ע����¼" << "\t" << "1.�鿴��Ʒ" << "\t" << "2.��Ʒ����" << "\t" << "3.�����Ʒ�����ﳵ" << "\t" << "4.ɾ�����ﳵ��Ʒ" 
			<< "\t" << "5.�鿴���ﳵ" << "\t" << "6.����" << endl;

	showline();

	int choice;
	cout << "�������:";
	cin >> choice;
	getchar();
	if(choice >= 0 && choice <= 6)
		return choice;
	else
	{
		cout << "��������������ѡ��" << endl;
		return WRONG;
	}
}

void Menu::showline()
{
	for(int i = 0; i < WindowLength; i++)
		cout << "=";
	cout << endl;
}