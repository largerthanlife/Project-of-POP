/*
* Copyright(c): 2017 - 2018
* File name: calculator.h
* Description: include some statements, macro definitions, functions that used in main.cpp
* Author: Liu Zechao
* Version: 1.3
* Date: 2018/6/15
*/


#ifndef _CALCULATOR
#define _CALCULATOR

#define COLOR_RECORD
#define FLUSH
#define HISTORY
// #define COUNT_SHOW
#define FOR_TEST
// #define MYCOMPLEX


#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<ctype.h>
#include<time.h>
#include<float.h>
#include"my_complex.h"

#include <complex>
#include <stack>
#include <iostream>
#include <fstream>
#include <readline/readline.h>
#include <readline/history.h>
#include <string>

using std::complex;
using std::stack;
using std::endl;
using std::cout;
using std::cin;
using std::string;
using std::ofstream;
using std::ifstream;
using std::ios;

typedef char Element;

typedef complex<double> Element_num;	// complex class from <complex>
// typedef myComplex<double> Element_num;	// complex class from my own

#define OprandType  Element_num
#define OpcharType  Element

/* Length of input string */
#define MAXSTRING   100

/* Length of some strings */
/* Used to store some part of input string */
#define MAXNUM      20

/* The number of times of add */
/* Used in the formula of definite integral */
#define MAX         100000

/* Length of line that show in the screen */
#define LINELENGTG  80

/* Label of functions */
#define CALCULATE   	1
#define VARIANCE    	2
#define INTEGRAL    	3
#define EQUATION    	4
#define INFORMATION 	5
#define SAMPLE      	6

#define VERSION     5.2
#define PI          3.1415926535897932384626433832795028841971
#define E           2.7182818284590452353602874713526624977572 


#define STACK_INIT_SIZE 50   //存储空间初始分配量
#define STACKINCREMENT  10   //存储空间分配增量
#define STATUS int
#define TRUE   1
#define FALSE  0
#define OK     1
#define ERROR  0
#define RAND   6		

#define BUFFER MAXSTRING
#define INSTRUCTION 5
#define LOGNAME ".history"

#ifdef COLOR_RECORD
#define LOGWORD "\033[35mMyCalculator, by Patrick\033[0m"
#else
#define LOGWORD "MyCalculator, by Patrick"

#endif
char *readCmdLine();

#endif
