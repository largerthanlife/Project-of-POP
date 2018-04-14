#ifndef _GOODS_H_
#define _GOODS_H_

#include "file.h"
#include <map>
#include <vector>
#include <string>
using std::string;
using std::vector;

class File;

class Goods
{
//private:
public:
	string ID;
	string name;
	string brand;
	float price;
	int num;
public:
	string goodsid(){return ID;}						//��ȡ��Ʒid
	string goodsname(){return name;}					//��ȡ��Ʒ��
	string goodsbrand(){return brand;}					//��ȡ��ƷƷ��
	float goodsprice(){return price;}					//��ȡ��Ʒ�۸�
	int goodsnum(){return num;}							//��ȡ��Ʒ����
	
	void setID(string newID){ID = newID;}
	void setname(string newname){name = newname;}
	void setbrand(string newbrand){brand = newbrand;}
	void setprice(float newprice){price = newprice;}	//������Ʒ�۸�
	void setnum(int newnum){num = newnum;}				//������Ʒ����
};


class Goodlist
{
private:
	File *file;
	//map<Goods> mgood;
public:
	Goodlist();
	void update();
	vector<Goods> vgood;
	//void readfile();
	//void deletegoods();
	//void addgoods();
};

#endif