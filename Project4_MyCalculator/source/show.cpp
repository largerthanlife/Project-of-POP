/*
 * File name: show.cpp
 * Description: Define some function .
				See the name of these functions so that to know what function they have
 * Author: Liu Zechao
 * Version: 2.2
 * Date: 2018/6/11
 * Function List:	unsigned int Menu();
 *					void ShowLine();
 *					void ShowInformation();
 *					void ShowHelp();
 *					void ShowSample();
 *					void Myshow();
 *					void ShowRectangle();
 */

#include "calculator.h"
#include "show.h"
#include <sys/types.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>

void StringHandle(OpcharType * const s, int *error);
void ConversionOfBODH(OpcharType *s);
OprandType EvaluateExpression(OpcharType * const s, OprandType ans, OprandType myrand, int *flag);
bool file_calc(char* filename, int *error);

// extern int INSTRUCTION;


/*
 * Description:	int Menu();
 * Calls:		void ShowLine();
 *				void ShowInformation();
 *				void StringHandle(OpcharType * const s);
 *				void ConversionOfBODH(OpcharType *s);
 *				OprandType EvaluateExpression(OpcharType * const s, OprandType ans, OprandType myrand, bool *flag);
 * Called By:	int main();
 * Input:		None.
 * Output:		Show the basic imformation and functions that can be used in the screen.
 * Return:		The label of functions.
 * Others:		It is permissible to input a mathematical expression in the version, as long as the result is included.
 *				If no label fits, error reporting mechanism will be called in the main() function.
 */
int Menu()
{
	void ShowLine();
	void ShowInformation();

	int error = 0;
    OpcharType *s;
    OprandType label = 0;
	int flag = 0;

	//ShowRectangle();
	ShowInformation();
	ShowLine();
	int index = 0;
	printf("\t\033[7;2mM E N U\033[7;0m\n\n");
	printf("Choose a label of these follow functions:\n\n");
	printf("%d.Calculate\n\n",++index);
	printf("%d.Variance and Standard deviation\n\n",++index);
	printf("%d.Definite  Integral\n\n",++index);
	printf("%d.Solve  Equation\n\n",++index);
	printf("%d.Help  and  Information\n\n",++index);
	printf("%d.Show  some  samples\n\n",++index);
	printf("Your  choice is:\n");
	
    s = readCmdLine();	
    StringHandle(s, &error);  //Even though there are error, it cannot find.
	ConversionOfBODH(s);
	if (strcmp(s, "q") && strcmp(s, "quit"))
	{
		label = EvaluateExpression(s, (int)0, (int)0, &flag); //The second and third parameter is useless, make them zero
		ShowLine();
		system("clear");
		free(s);
        return label.real();
	}
	else
	{
		system("clear");
		ShowInformation();
		ShowLine();
		printf("\nThanks for using.\n\n");
		ShowLine();
		exit(0);
	}
}

/*
 * Description:	void ShowLine();
 * Calls:		None.
 * Called By:	int Menu();
 *				int main();
 * Input:		None.
 * Output:		Show a line in the screen and start a new line.
 * Return:		None.
 * Others:		None.
 */
void ShowLine()
{
    int len = LINELENGTG;
    struct winsize size;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    len = size.ws_col;
	for (int i = 0; i < len; i++)
		putchar('-');
	printf("\n");
}

void replace_space(string& s)
{
	const char *t = s.c_str();
	char * temp = (char*)malloc(strlen(t) + 1);
	strcpy(temp, t);
	for(char * p = temp; *p != '\0'; p++)
		if(*p == ' ')
			*p = '_';

	string handled(temp);
	s = handled;
}

/*
 * Description:	void ShowInformation();
 * Calls:		None.
 * Called By:	int Menu();
 *				int main();
 * Input:		None.
 * Output:		Show the real time and the version number.
 * Return:		None.
 * Others:		None.
 */
void ShowInformation()
{
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);
	printf("%s", asctime(timeinfo));
	//printf("date:")
	//system("date /T TIME /T");
	//system("TIME /T");
	//printf("Liu ZeChao\tNanJing University\n");
	printf("version:%.1f\n",VERSION);
}


/*
 * Description:	void ShowHelp();
 * Calls:		None.
 * Called By:	int main();
 * Input:		None.
 * Output:		Show the functions that can be used, the way of inputting, some basic information about the program and the author.
 *				Show some notices.
 * Return:		None.
 * Others:		None.
 */
void ShowHelp()
{
	printf("\tThis is a simple calculator\n");
	printf("\tif you dont want to record your input history,\n\t\tyou can comment the \"#define HISTORY\" in calculator.h ");
	printf("\tif you dont want to refresh the interface everytime you input,\n\t\tyou can comment the \"#define FLUSH\" in calculator.h ");
	printf("\tif you dont want to make your history file to be colorful,\n\t\tyou can comment the \"#define COLOR_RECORD\" in calculator.h ");
	printf("\tif you dont want the \"show\" to be added to the history,\n\t\tyou can comment the \"#define COUNT_SHOW\" in calculator.h ");


	printf("\tUsing DBC case as input\n");
	printf("\tUsing @ to represent the square symbol\n");
	printf("\tUsing |x| or [x] to seek the absolute value of x\n");
	printf("\tUsing \"e + 1\"instead of\"y = e + 1\"to seek the value of y\n");

	printf("\tYou can only use real number as input\n");
	printf("\tYou can only use \"x\" as the independent variable while using function2\n");
	printf("\tYou can use upper or lower letter while input\n");
	printf("\tYou can use \"ans\" to represent the result while input\n");
	printf("\tYou can use \"pi\" and \"e\"while input\n");
	printf("\tYou can use \"h\" ,\"o\",\"b\"to represent the Hex, Octal and Binary\n");
	printf("\tYou can use \"rand\" to get a interger between 1 and 6\n");
	printf("\tYou can use \"show\" see last 5 inputs\n");
	printf("\tYou can use \"dis\" to get the distance between two complexes\n");
	printf("\tYou can use \"arg\" to get the angle of a complex\n");
	printf("\tYou can use \"cjg\" to get the conjugate of a complex\n");

	printf("\tYou can press arrow up and down to see more historis\n");
	printf("\tWhen your inputs is invalid, \"ans\" will be reset to zero\n");	
	printf("\tBut these oprators could only be uesd in interger\n");
	printf("\tThe lower and upper limit should both be numbers(not \"ans\")\n");
	printf("\tThe 1ength of input-string should be less than 50\n");
	printf("\tThe range of input limit is between %g and %g\n", DBL_MIN, DBL_MAX);
	printf("\tThe rang of result is between %g and %g\n", DBL_MIN, DBL_MAX);
	printf("\tThe result of definite integral isn't exact enough\n");
	printf("\tThe solving equation function is not uesful at all\n");
	printf("\tWhile using function2, remember to ends with an '@'\n");
	printf("\tIt's useful to check the sample before using this calculator\n");
	printf("\tIf you want to quit, input \"q\" or \"quit\" in the menu\n");
	printf("\tIf you find any error, send email to lzc@smail.nju.edu.cn\n");
	printf("\tThanks for using!\n");
	printf("\tLiu Zechao from NJU\n\n");
	printf("\tPress any key to return to the menu\n");
}

/*
 * Description:	void ShowSample();
 * Calls:		None.
 * Called By:	int main();
 * Input:		None.
 * Output:		Show some possible input, including a majority of error-input, and show the corresponding results.
 *				Show some characteristic.
 * Return:		None.
 * Others:		None.
 */
void ShowSample()
{
	unsigned int order = 1;
	printf("\tHere are some samples to show what operator and function can be used:\n\n");
	printf("\t%d.\tlg10                =                1\n",	order++);
	printf("\t%d.\t   ln(e)            =                1\n", 	order++);
	printf("\t%d.\t[1-[-2]]            =                1\n", 	order++);
	printf("\t%d.\t|1-|-2||            =                1\n", 	order++);
	printf("\t%d.\tasin(sin(1))        =                1\n", 	order++);
	printf("\t%d.\t2!  -  0!           =                1\n", 	order++);
	printf("\t%d.\t10h/1000b - 1o      =                1\n", 	order++);
	printf("\t%d.\t-2/-2               =                1\n", 	order++);
	printf("\t%d.\tpi\\e                =                1\n", 	order++);
	printf("\t%d.\t5  %%  2             =                1\n", 	order++);
	printf("\t%d.\tdis(0,0.6+0.8i)     =                1\n", 	order++);
	printf("\t%d.\t(cjg(3+4i))/5       =                1\n", 	order++);
	printf("\t%d.\t(-1)@2              =                i\n", 	order++);
	printf("\t%d.\targ(1+i)            =             PI/4\n", 	order++);
	printf("\t%d.\t2sinx              <=>         2*sin(x)\n", 	order++);
	printf("\t%d.\tcosx^2             <=>         cos(x^2)\n", 	order++);
	printf("\t%d.\t3^3*2              <=>          2*(3^3)\n", 	order++);
	printf("\t%d.\t4lnx               <=>          4*ln(x)\n", 	order++);
	printf("\t%d.\ttan(pi/2)          <=>        TAN(PI/2)\n", 	order++);
	printf("\t%d.\t2 ans              <=>             2ANS\n", 	order++);
	printf("\t%d.\trand               <=>    {1,2,3,4,5,6}\n", 	order++);
	printf("\t%d.\ty = 2x             <=>           error!\n", 	order++);
	printf("\t%d.\tlg(-1)             <=>           error!\n", 	order++);
	printf("\t%d.\t(cos0)/0           <=>           error!\n", 	order++);
	printf("\t%d.\t6%%0                <=>           error!\n", order++);
	printf("\t%d.\ttox(1)             <=>           error!\n", 	order++);
	printf("\t%d.\tacos(-1.0001)      <=>           error!\n", 	order++);
	printf("\t%d.\tlg[10]             <=>           error!\n", 	order++);
	printf("\t%d.\tlg([10])           <=>         correct!\n", 	order++);
	printf("\t%d.\t10000!             <=>        overflow!\n", 	order++);
	printf("\t%d.\t(No Input)         <=>              0\n\n", 	order++);
	printf("\tPress any key to return to the menu\n");
}

/*
 *
 */
void Myshow()
{
	ShowLine();
	printf("Hope you could see it someday\n\n");
	printf("Hope you have a wonderful life\n\n");
	printf("And I would be a better man%c\n\n", 1);
	printf(" to Princess Sweet cake\n");
	ShowLine();
	getchar();
	exit(0);
}

/*
 * Description:	int ShowSomething_or_not(char * s);
 * Calls:		None.
 * Called By:	int main();
 * Input:		the input string s which has been handled by string_handle().
 * Output:		None.
 * Return:		index of particular strings.
 * Others:		None.
 */
int ShowSomething_or_not(char * s)
{
	if(!strcmp(s, "show"))
		return 1;
	else if(!strcmp(s, "lin"))
		return 2;
	else if(!strcmp(s, "jyuan"))
		return 3;
	else if(!strcmp(s, "patrick"))
		return 4;
	else if(!strcmp(s, "q") || !strcmp(s, "qiut")|| !strcmp(s, "quit"))
		return 5;
	else if(!strcmp(s, "largerthanlife"))
		return 6;
	else
		return 0;
}

/*
 * Description:	void ShowMeg(string meg, int color);
 * Calls:		None.
 * Called By:	int main(), void ShowSomething, OprandType EvaluateExpression(), void StringHandle() and so on;
 * Input:		meg is the message you want to print and the font color is optional, default red.
 * Output:		color message
 * Return:		None.
 * Others:		None.
 */
void ShowMeg(string meg, int color = SHOW_RED)
{
	printf("\033[%dm%s\033[0m\n", color, meg.c_str());
}


/*
 * Description:	void ShowSomething(char*s ,int index, int use_time, char* instructions[MAXSTRING], int instruction_num);
 * Calls:		None.
 * Called By:	int main();
 * Input:		s is the input, index and use_time are used to determined what to print, instructions are histories to be shown.
 * Output:		Particular imformation of corresponding index.
 * Return:		None.
 * Others:		None.
 */
void ShowSomething(char *s, int index, int use_time, char* instructions[MAXSTRING], int instructino_num)
{
	switch(index)
	{
		case 1:
		{
			if(use_time == 0)
            {
            	ShowMeg("You havn't input! yet!");
			}
			else
        	{
        		string meg("");
        		cout << endl <<  "Here are your " << instructino_num << " latest inputs :" << endl << endl;
        		for(int i = 0; i < instructino_num; i++)
				{
					meg = meg & (i+1) & "->" & instructions[i];
					ShowMeg(meg, SHOW_DEEPGREEN);
					meg = "";
				}	
				cout << endl;
			}
		}
		break;

		case 2:
		{
			string meg("\n");
			meg = meg + "\"The hope\"" + "\n";
			ShowMeg(meg, SHOW_BLUE);
		}
		break;
		case 3:
		{
			string meg("\n");
			meg = meg + "This guy is handsome and kind!" + "\n";
			ShowMeg(meg, SHOW_DEEPGREEN);
		}
		break;
		case 4:
		{
			string meg("\n");
			meg = meg + "Less is more." + "\n";
			ShowMeg(meg, SHOW_BLUE);
		}
		break;

		case 5:
		{
			string meg("\n");
			meg = meg + "Do you want to quit ?" + "\n" 
					  + "If so, press \"Enter\" to return to the Menu" + "\n" 
					  + "then input \'q\' or \"quit\" to exit" + "\n";
			ShowMeg(meg, SHOW_YELLOW);
		}
		break;
		case 6:
		{
			string meg("\n");
			meg = meg + "Dont let me go" + "\n";
			ShowMeg(meg, SHOW_BLUE);
		}
		break;

		default:
		{
			string meg("\n");
			meg = meg + "To be continued..." + "\n";
			ShowMeg(meg);
		}
	}
}

/*
 * Description:	void Record(char *input, OprandType ans, bool error, int record_time);
 * Calls:		None.
 * Called By:	int main().
 * Input:		"input" is the expression that user inputs, if no error, "ans" will be recorded, otherwise records "invalid", every input has a correspond input-time. 		
 * Output:		record the input-exprssion and the correspond answer int history-log file, if the expression is invalid, then records "invalid", with correspond input-time.
 * Return:		None.
 * Others:		None.
 */
void Record(char *input, OprandType ans, bool error, int record_time)
{
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	char str[MAXSTRING];
	string filename = LOGNAME;
	ofstream file_history(filename, ios::app);

	file_history << asctime(timeinfo);
	file_history << record_time << ".";
	file_history << "Input: " << input << endl;
	if(!error)
	{
#ifdef COLOR_RECORD
		file_history << "\033[32mAns: \033[0m";
#else
		file_history << "Ans: ";
#endif
		if( ans.real() != 0)
			file_history << ans.real() ;
		if (ans.imag() > 0)
		{
			file_history << "+";
		}
		if(fabs(ans.imag()) != 0)
			file_history << ans.imag() << "i";
		if(ans.imag() == 0 && ans.real() == 0)
			file_history << "0";
		file_history << endl << endl;
	}
	else
#ifdef COLOR_RECORD
		file_history << "\033[31mAns: invalid\033[0m" << endl << endl;
#else
		file_history << "Ans: invalid" << endl << endl;
#endif
}

/*
 * Description:	void ShowAnswer(OprandType & ans);
 * Calls:		ShowMeg().
 * Called By:	int main();
 * Input:		ans which will be printed.
 * Output:		a format-answer according to the ans itself.
 * Return:		None.
 * Others:		None.
 */
void ShowAnswer(OprandType & ans)
{
	if(fabs(ans.real()) <= 1e-8)
        ans.real(0);
    if(fabs(ans.imag()) <= 1e-8)
        ans.imag(0);

	string meg("");
	if(ans.real() != 0)
		meg = meg & ans.real();
	if(ans.imag() > 0)
		meg = meg & "+";
	if(fabs(ans.imag()) != 0)
		meg = meg & ans.imag() & "i";
	if(ans.imag() == 0 && ans.real() == 0)
		meg = meg & "0";
	ShowMeg(meg, SHOW_GREEN);
}

/*
 * Description:	void ShowRectangle();
 * Calls:		None.
 * Called By:	None;
 * Input:		None.
 * Output:		Show rectangles in the screen.
 * Return:		None.
 * Others:		This function isn't used at present version.
 */
// void ShowRectangle()
// {
// 	HWND hwnd;
// 	HDC hdc;
// 	system("color F1");
// 	hwnd = GetConsoleWindow();
// 	hdc = GetDC(hwnd);
// 	//LineTo(hdc, 200, 300);
// 	//for(int i = 1; i < 10; i++)
// 	//	Rectangle(hdc, i, i, 640-i, 400-i);
// 	//for(int i = 50; i < 60; i++)
// 	//	Rectangle(hdc, i, i, 640-i, 400-i);
// 	//for(int i = 100; i < 110; i++)
// 	//	Rectangle(hdc, i, i, 640-i, 400-i);
// 	//for(int i = 150; i < 160; i++)
// 	//	Rectangle(hdc, i, i, 640-i, 400-i);
// 	//for(int i = 300; i < 310; i++)
// 	//Rectangle(hdc, i, i, 640-i, 400-i);

// 	//Rectangle(hdc, 2, 1, 640, 400);
// 	//printf("\n\n\n\n\n\n\n\n\n\n\n\n\tHere are some rectangles\n");
// 	//printf("\n\n\n\tYes they are\n");

// 	//for(int i = 50; i < 52; i++)//menuµÄ·½¿ò
// 	//	Rectangle(hdc, i+120, i+8, 320-i, 135-i);

// 	ReleaseDC(hwnd, hdc);
// 	//while(1);
// 	// return 0;
// }
