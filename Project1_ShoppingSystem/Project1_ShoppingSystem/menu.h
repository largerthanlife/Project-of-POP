#ifndef _MENU_H_
#define _MENU_H_

class Menu
{
public:
	Menu(){;}
	int showlogin();
	int showcustommenu();
	int showadminmenu();
	void showsucceed(bool);

private:
	void showline();
};


#endif
