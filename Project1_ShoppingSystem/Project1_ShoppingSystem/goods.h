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
	string goodsid(){return ID;}						//获取商品id
	string goodsname(){return name;}					//获取商品名
	string goodsbrand(){return brand;}					//获取商品品牌
	float goodsprice(){return price;}					//获取商品价格
	int goodsnum(){return num;}							//获取商品数量
	
	void setID(string newID){ID = newID;}
	void setname(string newname){name = newname;}
	void setbrand(string newbrand){brand = newbrand;}
	void setprice(float newprice){price = newprice;}	//设置商品价格
	void setnum(int newnum){num = newnum;}				//设置商品数量
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