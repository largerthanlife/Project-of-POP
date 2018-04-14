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
#define OUTPUTFORMAT setiosflags(ios::left)<< std::setw(FIELDWIDTH) //���������ʽ


//class File;

class Base
{
protected:
	File *file;
	string ID;			
	string password;
	bool isadmin;		//�Ƿ�Ϊ����Ա
	string storefile;	//����ļ���
	string soldfile;	//�����嵥�ļ���
public:
//	virtual void operationlist(int, Goodlist&) = 0; //���ݲ����ţ����ò�ͬ����
	virtual void List(Goodlist&) = 0;				//��ѯ��Ʒ�ӿ� ��ͳһʵ��
	virtual void Add(Goodlist&) = 0;				//�ֱ�ʵ��
//	virtual void Remove() = 0;						//�ֱ�ʵ��
//	virtual void Read() = 0;						//���� file ��ȡ��Ӧ�ļ�
};

class Customer; //ǰ������

class Admin:public Base
{
private:
	vector<Customer> vcustomer;			//������ע���û�
	vector<string> shoppingnames;		//�û����ﳵ��
	map<string, string> customerlist;	//�����û���������
	string customerfile;				//��ʼ��Ϊ �û�.txt
	string soldoutfile;					//�۳��嵥���ļ���
public:
	Admin();
	~Admin();
	bool Login();
	void operationlist(int, Goodlist&);			//�����б�
	void List(Goodlist&);						//�г�������Ʒ
	void Logout();								//ע��
	void Modify(Goodlist&);						//�޸���Ʒ��Ϣ
	void Add(Goodlist&);						//�ڿ���ļ��������Ʒ
	void Remove(Goodlist&);						//�ڿ���ļ���ɾ����Ʒ
	void Modifycustom(string);					//�޸��û����ﳵ����
	bool Addcustom(Customer*&, bool&);			//����û���¼
	void ListSold();							//�г����۳���Ʒ
	void customerupdate();						//���� �û�.txt
	void setID(string newID){ID = newID;}		//�޸��û���
	bool Checkout(Customer*const&,Goodlist&);		//Ϊĳλ�˿ͽ���
};

class Customer:public Base
{
private:
	string boughtfile;				//�����ļ�
	string shoppingfile;			//���ﳵ�ļ���
public:
	Customer();
	~Customer();
	vector<Goods> sgoods;			//���ﳵ������Ʒ
	void setID(string newID){ID = newID;}
	void setpassword(string newpassword){password = newpassword;}
	string getID(){return ID;}
	string getpassword(){return password;}

	void Login();
	void operationlist(int, Goodlist&, Admin&);
	void Search(Goodlist& );		//��ѯĳһ��Ʒ
	void Add(Goodlist&);			//�ڹ��ﳵ�ļ��������Ʒ
	void Remove();					//�ڹ��ﳵ�ļ���ɾ����Ʒ
	void ListGoods();				//�鿴���ﳵ�ļ�
	bool Checkout(Goodlist&);				//����
	void List(Goodlist&);			//�г����У��ɹ�����Ʒ
	void update();					//���¶�Ӧ�Ĺ��ﳵ�ļ�
	void deletegoods(string);				//ɾ�����ﳵ���ĳһָ����Ʒ
	void clear();					//��չ��ﳵ
};


#endif