/*
 * File name: my_complex.h
 * Description: provide a class named myComplex, to fit those interfaces of std::complex class, just for test, and it is terrible.
 * Author: Liu Zechao
 * Version: 1.3
 * Date: 2018/6/15
 */

#ifndef _MYCOMPLEX_H_
#define _MYCOMPLEX_H_

#include "calculator.h"
#include "show.h"
#include <iostream>

using std::ostream;
using std::istream;
using std::endl;
using std::cout;
using std::cin;

#ifdef MYCOMPLEX

template<typename value_type>
class myComplex
{
public:
	typedef myComplex<value_type> _myC;
	typedef value_type _mytype;
private:
	_mytype _real;
	_mytype _imag;
public:
	myComplex() {_real = 0; _imag = 0;}
	myComplex(_mytype realvalue, _mytype imagvalue) {_real = realvalue; _imag = imagvalue;}
	myComplex(_myC& src) {_real = src._real; _imag = src._imag;}
	myComplex(const _myC& src) {_real = src._real; _imag = src._imag;}
	myComplex(const int src) {_real = src; _imag = 0;}
	myComplex(const double src) {_real = src; _imag = 0;}
	~myComplex() {};

	_mytype real() {return _real;}
	_mytype imag() {return _imag;}
	void real(_mytype value) { _real = value;}
	void imag(_mytype value) { _imag = value;}
	
	_myC& operator= (_myC);
	_myC  operator= (const int src){_real = src; _imag = 0;}
	_myC  operator+ (_myC );
	_myC  operator- (_myC );
	_myC  operator- (double);
	_myC  operator* (_myC );
	_myC  operator/ (_myC );

	_myC  operator+= (_myC &);
	_myC  operator+= (const _myC &);
	_myC  operator-= (_myC &);
	_myC  operator-= (const _myC &);
	_myC  operator*= (_myC);
	_myC  operator/= (_myC);
	_myC  operator++ ();
	_myC  operator++ (int);
	bool operator<= (_myC);

	friend ostream& operator<< (ostream& out, _myC& src)
	{
		out << "real = " << src.real() << "," << "imag = " << src.imag() << endl;
		return out;
	}

	friend istream& operator>> (istream& in, myComplex<value_type>& src)
	{
		in >> src._real >> src._imag;
		return in;
	}

	friend _myC operator/ (double left, _myC src)
	{
		// cout << "left " << left << ", " << src << endl;
		_mytype temp = norm(src);
		src.real(left * src.real()/temp);
		src.imag(-left * src.imag()/temp);
		// cout << src << endl;
		return src;
	}

	friend _myC operator* (double left, _myC src)
	{
		src.real(left * src.real());
		src.imag(left * src.imag());
		return src;
	}
};


template<typename value_type>
myComplex<value_type>& myComplex<value_type>::operator=(myComplex<value_type> src)
{
	if(this != &src)
	{
		this->_real = src.real();
		this->_imag = src.imag();
	}
	return *this; 
}

template<typename value_type>
myComplex<value_type> myComplex<value_type>::operator+(myComplex<value_type> src)
{
	myComplex<value_type> temp;
	temp.real(this->_real + src.real());
	temp.imag(this->_imag + src.imag());
	return temp;
}

template<typename value_type>
myComplex<value_type> myComplex<value_type>::operator-(myComplex<value_type>  src)
{
	myComplex<value_type> temp;
	temp.real(this->_real - src.real());
	temp.imag(this->_imag - src.imag());
	return temp;
}

template<typename value_type>
myComplex<value_type> myComplex<value_type>::operator-(double src)
{
	this->_real -= src;
	return *this;
}

template<typename value_type>
myComplex<value_type> myComplex<value_type>::operator*(myComplex<value_type>  src)
{
	myComplex<value_type> temp;
	temp.real(this->real()*src.real() - this->imag()*src.imag());
	temp.imag(this->real()*src.imag() + this->imag()*src.real());
	return temp;
}

template<typename value_type>
myComplex<value_type> myComplex<value_type>::operator/(myComplex<value_type> src)
{
	myComplex<value_type> temp;
	temp.real( (this->real()*src.real() + this->imag()*src.imag()) / norm(src) );
	temp.imag( (this->imag()*src.real() - this->real()*src.imag()) / norm(src) );
	return temp;
}

template<typename value_type>
myComplex<value_type> myComplex<value_type>::operator+= (const myComplex<value_type> &src)
{
	_real += src._real;
	_imag += src._imag;
	return *this;
}

template<typename value_type>
myComplex<value_type> myComplex<value_type>::operator-=(const myComplex<value_type> & src)
{
	_real -= src._real;
	_imag -= src._imag;
	return *this;
}

template<typename value_type>
myComplex<value_type> myComplex<value_type>::operator+= (myComplex<value_type> &src)
{
	_real += src._real;
	_imag += src._imag;
	return *this;
}

template<typename value_type>
myComplex<value_type> myComplex<value_type>::operator-=(myComplex<value_type> & src)
{
	_real -= src._real;
	_imag -= src._imag;
	return *this;
}

template<typename value_type>
myComplex<value_type> myComplex<value_type>::operator*= (myComplex<value_type> src)
{
	myComplex<value_type> temp;
	temp.real(this->real()*src.real() - this->imag()*src.imag());
	temp.imag(this->real()*src.imag() + this->imag()*src.real());
	this->_real = temp._real;
	this->_imag = temp._imag;
	return *this;
}

template<typename value_type>
myComplex<value_type> myComplex<value_type>::operator/= (myComplex<value_type> src)
{
	myComplex<value_type> temp;
	temp.real( (this->real()*src.real() + this->imag()*src.imag()) / fabs(src) );
	temp.imag( (this->imag()*src.real() - this->real()*src.imag()) / fabs(src) );
	this->_real = temp._real;
	this->_imag = temp._imag;
	return *this;
}

template<typename value_type>
bool myComplex<value_type>::operator<= (myComplex<value_type> src)
{
	return norm(*this) <= norm(src) ;
}

template<typename value_type>
myComplex<value_type> myComplex<value_type>::operator++()
{
	_real++;
	return *this;
}

template<typename value_type>
myComplex<value_type> myComplex<value_type>::operator++(int)
{
	myComplex<value_type> temp(*this);
	_real++;
	return temp;
}

template<typename value_type>
myComplex<value_type> fabs(myComplex<value_type> var)
{
	return pow(var.real() * var.real() + var.imag() * var.imag(),0.5);
}

template<typename value_type>
myComplex<value_type> arg(myComplex<value_type> var)
{
	if(var.real() == 0 )
	{
		// ShowMeg("Error: in arg(), the real part of parameter cannot be zero");
		cout << "\033[31mError: in arg(), the real part of parameter cannot be zero\033[0m";
		return 0;
	}
	else
		return atan(fabs(var.imag()) / fabs(var.real()));
}

template<typename value_type>
myComplex<value_type> conj(myComplex<value_type> var)
{
	myComplex<value_type> temp(var.real(), -var.imag());
	return temp;
}

//to be continue
template<typename value_type>
myComplex<value_type> pow(myComplex<value_type> var, myComplex<value_type> _num)
{
	int num = _num.real();
	if(num<0)
	{
		// if(num==INT_MIN)
		// 	return 1.0 / (pow(var,INT_MAX)*var);
	 // 	else
		_num.real( -_num.real());
		return 1.0 / pow(var,_num);
	}
    if(num==0)
        return 1.0;
	myComplex<value_type> ans = 1.0;

	for( ; num>0; var *= var, num>>=1)
	{
		if(num&1>0)
			ans *= var;
	}
	return ans;
}

template<typename value_type>
value_type norm(myComplex<value_type> var)
{
	return var.real() * var.real() + var.imag() * var.imag();
}

template<typename value_type>
myComplex<value_type> sin(myComplex<value_type> var)
{
	;
	return var;
}

template<typename value_type>
myComplex<value_type> cos(myComplex<value_type> var)
{
	return var;
}

template<typename value_type>
myComplex<value_type> tan(myComplex<value_type> var)
{
	return var;
}

template<typename value_type>
myComplex<value_type> asin(myComplex<value_type> var)
{
	;
	return var;
}

template<typename value_type>
myComplex<value_type> acos(myComplex<value_type> var)
{
	return var;
}

template<typename value_type>
myComplex<value_type> atan(myComplex<value_type> var)
{
	return var;
}

template<typename value_type>
myComplex<value_type> log(myComplex<value_type> var)
{
	return var;
}

template<typename value_type>
myComplex<value_type> log10(myComplex<value_type> var)
{
	return var;
}

template<typename value_type>
value_type sqrt(myComplex<value_type> var)
{
	return 1;
}

#endif

#endif
