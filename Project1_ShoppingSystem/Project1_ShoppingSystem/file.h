#ifndef _FILE_H_
#define _FILE_H_

#include "goods.h"
#include <vector>
#include <string>
#include <map>

using std::vector;
using std::string;
using std::map;

class Goods;

class File
{
private:
public:
	string filename;
	void readfile(vector<Goods> &);	//��ȡ����ļ�
	void readfile(map<string, string> &, vector<string> &);	//��ȡ�û������ļ�������
	void readsoldoutfile();							//��ȡ�ۻ��嵥��ֱ����ʾ
	void writefile(vector<Goods> &);				//���¹��ﳵ�ļ�
	void writefile(map<string, string> &);			//�����û������ļ�
	void writesoldoutfile(vector<Goods> &, string);	//�����ۻ��嵥�ļ�
	void creatfile(string);		//�����û�������Ӧ���ﳵ�ļ�
};

#endif