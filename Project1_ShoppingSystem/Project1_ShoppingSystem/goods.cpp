#include "goods.h"
#include "file.h"



Goodlist::Goodlist()
{
	file = new File;
	file->filename = "���.txt";
	file->readfile(vgood);
}

void Goodlist::update()
{
	file->writefile(vgood);
}