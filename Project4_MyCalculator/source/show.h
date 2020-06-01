#ifndef _SHOW_H_
#define _SHOW_H_

#include <string>
#include <sstream>
using std::string;
using std::ostringstream;

#define SHOW_BLACK		30
#define SHOW_RED 		31
#define SHOW_GREEN		32
#define	SHOW_YELLOW		33
#define SHOW_BLUE		34
#define SHOW_PURPLE		35
#define	SHOW_DEEPGREEN	36
#define SHOW_WHITE		37


template <typename T>
string operator& (const T &_t, const string &s)
{
	ostringstream oss;
	oss << _t;
	return oss.str() + s;
}

template <typename T>
string operator& (const string &s, const T &_t)
{
	ostringstream oss;
	oss << _t;
	return  s + oss.str() ;
}

#endif
