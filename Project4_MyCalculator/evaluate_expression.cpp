/*
 * File name: evaluate_expression.cpp
 * Description: Seek the final value of the input stirng
 * Author: Liu Zechao
 * Version: 2.3
 * Date: 2018/6/15
 * Function List:	static bool IsOperator(OpcharType c);
 *					static OprandType Factorial(unsigned int n);
 *					static bool Check(const OpcharType * const s);
 *					static int Count_Point(char *s);
 *					OprandType EvaluateExpression(OpcharType * const s, OprandType ans, OprandType myrand, int *flag);
 */

#include"calculator.h"
#include"show.h"

OpcharType Precede(OpcharType top, OpcharType get, int *flag);
OprandType Operate(OprandType a, OpcharType theta, OprandType b, int *flag);
OprandType StringToOprand(OpcharType * const s);
void ShowMeg(string meg, int color = SHOW_RED);

/*
 * Description:	bool IsOperator(OpcharType c);
 * Calls:		None.
 * Called By:	OprandType EvaluateExpression(OpcharType * const s, OprandType ans, OprandType myrand, bool *flag)
 * Input:		a possible operator 'c'.
 * Output:		None.
 * Return:		If 'c' is an allowable oprator, return TRUE, else return FALSE.
 * Others:		None.
 */
bool IsOperator(OpcharType c)
{
	if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')' || c == '^' || c == '\0' || c == '@' || c == '|' || c == '!' || c == '[' || c == ']' || c == '%' || c == '\\' || c == ',' /*|| c == 'i'*/)
		return TRUE;
	else
		return FALSE;
}

/*
 * Description:	OprandType Factorial(unsigned int n);
 * Calls:		None.
 * Called By:	OprandType EvaluateExpression(OpcharType * const s, OprandType ans, OprandType myrand, bool *flag)
 * Input:		Nonnegative integer 'n'.
 * Output:		None.
 * Return:		The factorial value of 'n'.
 * Others:		None.
 */
static OprandType Factorial(unsigned int n)
{
	OprandType sum = 1;

	for (int i = n; i >= 1; i--)
		sum *= i;
	return sum;
}

/*
 * Description:	int Count_Point(char *s);
 * Calls:		None.
 * Called By:	OprandType EvaluateExpression(OpcharType * const s, OprandType ans, OprandType myrand, bool *flag)
 * Input:		a string which will be transfered into OprandType type.
 * Output:		None.
 * Return:		The number of decimal point in this string.
 * Others:		if number of decimal point is larger than 1, a warning will be print.
 */
static int Count_Point(char *s)
{
	int num = 0;
	for(char * p = s ; *p != '\0'; p++)
		if(*p == '.')
			num++;
	if(num > 1)
		cout << "too many decimal points in this number: " << s << endl;
	return num;
}

/*
 * Description:	bool Check(const OpcharType * const s);
 * Calls:		None.
 * Called By:	OprandType EvaluateExpression(OpcharType * const s, OprandType ans, OprandType myrand, bool *flag)
 * Input:		A possible mathematical function 's'.
 * Output:		None.
 * Return:		If 's' is a mathematical function, returm TURE, else return FALSE.
 * Others:		None.
 */
static bool Check(const OpcharType * const s)
{
	if (!strcmp(s, "sin") || !strcmp(s, "cos") || !strcmp(s, "tan") || !strcmp(s, "ln") ||
		!strcmp(s, "lg") || !strcmp(s, "asin") || !strcmp(s, "acos") || !strcmp(s, "atan") ||
		!strcmp(s, "arg") || !strcmp(s, "cjg"))
		return TRUE;
	else
		return FALSE;
}

/*
 * Description:	OprandType EvaluateExpression(OpcharType * const s, OprandType ans, OprandType myrand, int *flag);
 * Calls:		bool IsOperator(OpcharType c);
 *				OprandType Factorial(unsigned int n);
 *				bool Check(const OpcharType * const s);
 *				OpcharType Precede(OpcharType top, OpcharType get);
 *				OprandType Operate(OprandType a, OpcharType theta, OprandType b, bool *flag);
 *				OprandType StringToOprand(OpcharType * const s);
 * Called By:	int main();
 *				bool SolvingEquations(OpcharType *s, OprandType *answer);
 * Input:		's' is the mathematicalexpression that has been pretreated and BODH-convert.
 *				'ans' is the value of reslut of last operation.
 *				'myrand' is used as the value of 'x', if there's any.
 *				'flag' points to the memory that stores the number of times of error-happen.
 * Output:		None.
 * Return:		The value of the final result.
 * Others:		If there's error happens, then 'flag' points to a Nonzero value.
 *				Evaluate the string 's' one by one, judge whether they are OPNUM or OPCHAR,
 *				Push them into different stack after handled.
 */
OprandType EvaluateExpression(OpcharType * const s, OprandType ans, OprandType myrand, int *flag)//sÎªÐèÒª¼ÆËãµÄ×Ö·û´®£¬ ansÊÇÉÏÒ»´ÎµÄ¼ÆËã½á¹û£¬myrandÊÇ¿ÉÄÜ´æÔÚµÄx
{
	bool IsOperator(OpcharType c);
	OprandType Factorial(unsigned int n);
	bool Check(const OpcharType * const s);

	//Stack  _STACK_INITIAL;
	//Stack1 _STACK1_INITIAL;
	//Stack  *OPCHAR = &_STACK_INITIAL;   //¶Ô OPCHAR Õ»Ö¸Õë³õÊ¼»¯
	//Stack1 *OPNUM = &_STACK1_INITIAL;   //¶Ô OPNUM Õ»Ö¸Õë³õÊ¼»¯
	stack<OprandType>* OPNUM;
	stack<OpcharType>* OPCHAR;
	OpcharType top, waste, theta;       // top ÓÃÀ´½ÓÊÕ OPCHAR Õ»µÄÕ»¶¥ÔªËØ£¬ waste ÓÃÀ´½ÓÊÕ·ÏÆúµÄÔËËã·û£¨Ò»°ãÎª×óÀ¨ºÅ£©£¬ theta Îªµ±Ç°µÄÔËËã·û
	OpcharType store[MAXNUM];           //ÓÃstoreÊý×éÀ´´æ´¢¿ÉÄÜµÄ¶à×Ö·ûº¯Êý¡¢¸¡µãÊý¡¢PI»òE
	OprandType lopnum, ropnum;          //×óÓÒ²Ù×÷Êý
	OprandType temp, result;            //tempÓÃÓÚ´æ´¢¼ÆËã¹ý³ÌÖÐµÄÔÝÊ±Á¿£¬Èç¼ìÑé¾ø¶ÔÖµ·ûºÅÄÚµÄÊýÊÇ·ñ´óÓÚÁã £¬result ÓÃÔÚ×îºó´æ´¢¼ÆËã½á¹û
	OpcharType *p = s;
	unsigned int i;

	//¶Ô²Ù×÷·ûÕ»½øÐÐ³õÊ¼»¯£¬Ñ¹ÈëÒ»¸ö¿Õ×Ö·û£¬Óë½ÓÊÕµÄ×Ö·û´®Î²²¿µÄ¿Õ×Ö·û³É¶ÔÆ¥Åä
	OPCHAR = new stack<OpcharType>;
	OPCHAR->push('\0');

	//¶Ô²Ù×÷ÊýÕ»½øÐÐ³õÊ¼»¯
	OPNUM = new stack<OprandType>;
	OPNUM->push(0);

	//Get_Top(OPCHAR, &top); //¸ø e Ò»¸ö³õÊ¼Öµ
	top = OPCHAR->top();
		int kkk = 0;
	while ((*p != '\0' || top != '\0') && !(*flag))
	{
		if(*p == '\r')
		{
			cout << "Windows Error" << endl;
			p++;
			continue;
		}
		if (!IsOperator(*p))
		{
			if ((*p >= '0' && *p <= '9')||*p == 'i')  //´¦Àí¶àÎ»Êý
			{
				for (i = 0; !IsOperator(*p)&& (!isalpha(*p) || *p =='i')&& *p != ','  ; i++)  //¼È·ÇÔËËã·ûÒ²·Ç×ÖÄ¸£¨Èý½Çº¯Êý£©
				{	
					if((*p >= '0' && *p <= '9') || *p == '.')
					{
						if(!isalpha(*p))
							store[i] = *p++;
					}
					
					else if(*p == 'i')
					{
						store[i] = *p++;
						if(*p >= '0' && *p <= '9')
						{
							ShowMeg("\ni cannot followed ny numbers, pleas input again\n");
							(*flag)++;
						}
						else
						{
							++i;
							break;
						}
					}
				}
				store[i] = '\0';

				if(Count_Point(store) > 1)
					(*flag)++;
#ifdef FOR_TEST
				else if(Count_Point(store) == 0 && store[0] == '0' & strlen(store) > 1 && store[strlen(store)-1] != 'i')
				{
					ShowMeg("invalid input");
					(*flag)++;
				}
#endif

				//Push(OPNUM, StringToOprand(store));    //×Ö·û×ªÎªÊý×Ö½øÕ»£¬Ö»ÄÜ´¦ÀíÒ»Î»Êý
				if(store[ strlen(store) - 1 ] != 'i')
				{
					OPNUM->push(StringToOprand(store));
				}
				else
				{
					char temp[MAXSTRING];
					strncpy(temp, store, strlen(store) - 1);
					temp[strlen(store) - 1] = '\0';
					OprandType temp_imag(0, 0);

					if(temp[0] != '\0') // only single 'i'
						temp_imag.imag(StringToOprand(temp).real());
					else
						temp_imag.imag(1);						
					OPNUM->push(temp_imag);
				}
			}

			else if (*p == 's' || *p == 'c' || *p == 't')  // ´¦ÀíÈý½Çº¯Êý
			{
				for (i = 0; isalpha(*p) && *p != 'x' && *p != 'e' && *p != 'p'; i++)
					store[i] = *p++;
				store[i] = '\0';

				if (Check(store))
				{
					if ((*(p - 1) >= '0' && *(p - 1) <= '9') || *(p - 1) == ')')  //¿ÉÄÜ³öÏÖ 9cos0 »ò£¨3+2£©cos1 ÕâÑùµÄ±í´ïÊ½£¬ Ä¬ÈÏÎª³Ë·¨
						OPCHAR->push('*');	
					if(!strcmp(store, "cjg"))
						OPCHAR->push('j');
					else
						OPCHAR->push(store[0]);
				}
				else
				{
					string meg("\n");
					meg = meg & store & " is not a operator, do you mean sin, cos or tan?\n";
					ShowMeg(meg);
					(*flag)++;
				}
			}

			else if (*p == 'l')	//´¦Àí¶ÔÊýº¯Êý
			{
				for (i = 0; isalpha(*p) && *p != 'e' && *p != 'p' && *p != 'x'; i++)
					store[i] = *p++;
				store[i] = '\0';

				if (strlen(store) != 2)  // 2ÊÇlg»òlnµÄ×ÖÄ¸¸öÊý
				{
					string meg("\n");
					meg = meg & store & " is not a operator, do you mean ln, lg or log?\n";
					ShowMeg(meg);
					(*flag)++;
				}
				else
					if (Check(store))
					{	
						if ((*(p - 1) >= '0' && *(p - 1) <= '9') || *(p - 1) == ')')  //¿ÉÄÜ³öÏÖ 9lg10»ò£¨3+2)ln2 ÕâÑùµÄ±í´ïÊ½£¬ Ä¬ÈÏÎª³Ë·¨
							OPCHAR->push('*');
						OPCHAR->push(store[1]);
					}
					else
					{
						string meg("\n");
						meg = meg & store & " is not a operator, do you mean lg, ln or log?\n";
						ShowMeg(meg);
						(*flag)++;
					}
			}

			else if (*p == 'a') //´¦Àí·´Èý½Çº¯Êý
			{
				for (i = 0; isalpha(*p) && *p != 'x' && *p != 'e' && *p != 'p'; i++)
					store[i] = *p++;
				store[i] = '\0';
		
				if (Check(store))
				{
					if ((*(p - 1) >= '0' && *(p - 1) <= '9') || *(p - 1) == ')')  //¿ÉÄÜ³öÏÖ 9cos0 »ò£¨3+2£©cos1 ÕâÑùµÄ±í´ïÊ½£¬ Ä¬ÈÏÎª³Ë·¨
						OPCHAR->push('*');
					OPCHAR->push(store[1]-32);
				}
				else if (!strcmp(store, "ans"))
				{
					if ((*(p - 1) >= '0' && *(p - 1) <= '9') || *(p - 1) == ')')  //¿ÉÄÜ³öÏÖ 9cos0 »ò£¨3+2£©cos1 ÕâÑùµÄ±í´ïÊ½£¬ Ä¬ÈÏÎª³Ë·¨
						OPCHAR->push('*');
					OPNUM->push(ans);
				}
				else
				{
					string meg("\n");
					meg = meg & store & " is not a operator, do you mean atan, asin or acos?\n";
					ShowMeg(meg);
					(*flag)++;
				}
			}

			else if (*p == 'p') //´¦Àí¿ÉÄÜ³öÏÖµÄ pi
			{
				for (i = 0; isalpha(*p); i++)
					store[i] = *p++;
				store[i] = '\0';

				if(!strcmp(store, "pi"))
				{
					if ((*(p - 1) >= '0' && *(p - 1) <= '9') || *(p - 1) == ')')  //¿ÉÄÜ³öÏÖ 9pi »ò£¨3+2£©pi ÕâÑùµÄ±í´ïÊ½£¬ Ä¬ÈÏÎª³Ë·¨
						OPCHAR->push('*');
					OPNUM->push(PI);
				}
				else if (strlen(store) == 2)
				{
					string meg("\n");
					meg = meg & store & " is not a operator, do you mean \"pi\"?\n";
					ShowMeg(meg);
					(*flag)++;
				}
				else
				{
					string meg("\n");
					meg = meg & store & " is not a operator, try again\n";
					ShowMeg(meg);
					(*flag)++;
				}
			}

			else if (*p == 'e')// ´¦Àí¿ÉÄÜ³öÏÖµÄ e 
			{
				for (i = 0; isalpha(*p); i++)
					store[i] = *p++;
				store[i] = '\0';

				if (strlen(store) != 1)
				{
					string meg("\n");
					meg = meg + store + " is not a operator, do you mean \'e\'?\n";
					ShowMeg(meg);
					(*flag)++;
				}
				else
				{
					if (isalpha(*p) && ((*(p - 1) >= '0' && *(p - 1) <= '9') || *(p - 1) == ')'))  //¿ÉÄÜ³öÏÖ 6e »ò£¨3+2£©e ÕâÑùµÄ±í´ïÊ½£¬ Ä¬ÈÏÎª³Ë·¨
						OPCHAR->push('*');
					OPNUM->push(E);
				}
			}

			else if (*p == 'x') //Æ¥Åä x £¬½«ÆäÌæ»»³É¶ÔÓ¦µÄËæ»úÊý
			{
				if ((*p == 'x') && ((*(p - 1) >= '0' && *(p - 1) <= '9') || *(p - 1) == ')'))
					OPCHAR->push('*');
				OPNUM->push(myrand);
				p++;
			}

			else if (*p == 'r')
			{
				for (i = 0; isalpha(*p); i++)
					store[i] = *p++;
				store[i] = '\0';


				if (!strcmp(store, "rand"))
				{
					if (isalpha(*p) && ((*(p - 1) >= '0' && *(p - 1) <= '9') || *(p - 1) == ')'))  //¿ÉÄÜ³öÏÖ 6e »ò£¨3+2£©e ÕâÑùµÄ±í´ïÊ½£¬ Ä¬ÈÏÎª³Ë·¨
						OPCHAR->push('*');
					srand((unsigned)time(NULL));	//ÓÃÊ±¼ä×öÖÖ£¬Ê¹µÃÃ¿´Î²úÉúËæ»úÊý²»Ò»Ñù
					OPNUM->push( (OprandType)(rand() % RAND + 1));
				}
				else
				{
					string meg("\n");
					meg = meg & store & " is not a operator, do you mean rand?\n";
					ShowMeg(meg);
					(*flag)++;
				}
			
			}

			else if(*p == 'd')
			{
				for (i = 0; isalpha(*p); i++)
					store[i] = *p++;
				store[i] = '\0';

				if (!strcmp(store, "dis"))
				{
					if (isalpha(*p) && ((*(p - 1) >= '0' && *(p - 1) <= '9') || *(p - 1) == ')'))  //¿ÉÄÜ³öÏÖ 6e »ò£¨3+2£©e ÕâÑùµÄ±í´ïÊ½£¬ Ä¬ÈÏÎª³Ë·¨
						OPCHAR->push('*');
					OPCHAR->push('d');
				}
				else
				{
					string meg("\n");
					meg = meg & store & " is not a operator, do you mean dis?\n";
					ShowMeg(meg);
					(*flag)++;
				}				
			}

			else
			{
				// if (isalpha(*p) && ((*(p - 1) >= '0' && *(p - 1) <= '9') || *(p - 1) == ')'))
				// 	OPCHAR->push('*');

				for (i = 0; isalpha(*p); i++)
					store[i] = *p++;
				store[i] = '\0';

				string meg("\n");
				meg = meg & store & " is not a operator or something.\n";
				ShowMeg(meg);
				(*flag)++;
			}
		}

		else if (*p == '!') //Ëã½×³Ë
		{
			temp = OPNUM->top();
			if (temp.real() == (int)temp.real() && !temp.imag())
			{
				if (temp.real() >= 0)
				{
					temp = OPNUM->top();
					OPNUM->pop();
					OPNUM->push(Factorial(temp.real()));
				}
				else
				{
					ShowMeg("\nThe left value of factorial should not be less than zero!\n");
					(*flag)++;
				}
				if (temp.real() >= 13)
					ShowMeg("\nThe left value of factorial is too largen\nThe reslut may be overflow!\n");
				p++;
			}
			else
			{
				ShowMeg("\nThe left value of factorial should be integer!\n");
				(*flag)++;
			}
		}

		else
		{

#ifndef FOR_TEST
			if ((*p == '(' || *p == '[') && ((*(p - 1) >= '0' && *(p - 1) <= '9') || *(p - 1) == ')' || *(p-1) == 'i') )//×óÀ¨ºÅÇ°²¹À¨ºÅ
				OPCHAR->push('*');
#endif
			switch (Precede(top, *p, flag))
			{
			case '<': 
				OPCHAR->push(*p++);
				break;

			case '=':
				 if (top == '(')
				 {
				 	if(*p == ')')
					{
						waste = OPCHAR->top();
						OPCHAR->pop();
					}
					else if(*p == ',')
						;
					p++;
				 } 
				 else if (top == '[')  //¼ÆËã¾ø¶ÔÖµ
				 {
					temp = OPNUM->top();
					OPNUM->pop();

					OPNUM->push(fabs(temp));

					waste = OPCHAR->top();
					OPCHAR->pop();
					p++;
				  }
				  break;

			case '>': 
				theta = OPCHAR->top();
				OPCHAR->pop();
				if(theta == ',')
					break;
				else if (!isalpha(theta) || theta == 'd')
				{
					ropnum = OPNUM->top();
					OPNUM->pop();
					lopnum = OPNUM->top();
					OPNUM->pop();
					OPNUM->push(Operate(lopnum, theta, ropnum, flag));
				}
				else
				{
					lopnum = 0;  //²»ÐèÒªÓÃµ½a£¬µ«ÎªÁËµ÷ÓÃOperateº¯Êý£¬ÐèÒª¸øaÒ»¸öÖµ£¬Ò²¿ÉÒÔÉèOperateº¯ÊýµÄÄ¬ÈÏ²ÎÊýÎªÁã
					ropnum = OPNUM->top();
					OPNUM->pop();
					OPNUM->push(Operate(lopnum, theta, ropnum, flag));
				}
				break;
			}
		}
		top = OPCHAR->top();
	}

	result = OPNUM->top();
	return result;
}
