/*
 * File name: string_handle.cpp
 * Description: simplify the input string , so that i can do less work in the following parts.
 * Author: Liu Zechao
 * Version: 2.2
 * Date: 2018/6/15
 * Function List:	
 	void StringHandle(OpcharType * const s, int *error);
	void Exclude(char * const s, int *error);
 */

#include"calculator.h"
#include"show.h"
int ShowSomething_or_not(char * s);
void ShowMeg(string meg, int color = SHOW_RED);
void Exclude(char * const s, int *error);
void just_for_test(char* const s, int *error);
bool IsOperator(OpcharType c);


 /*
  * Description:void StringHandle(OpcharType * const s, int *error);
  * Calls:		None.
  * Called By:	unsigned int Menu();
  *				int main();
  *				OprandType Variance(int num);
  * Input:		's' is the original input string.
  * Output:		None.
  * Return:		None.
  * Others:		After soluted, 's' is simplified.with no space and zerofill, which can be uesd in EvaluateExpression() function 
  *					to seek the result.
  */
void StringHandle(OpcharType * const s, int *error)
{
	OpcharType temp[MAXSTRING];
	OpcharType t[MAXSTRING];
	OpcharType *p;
	unsigned int i;
	unsigned int flag = 0;

	int num_of_vline = 0; // num of '|'
	int half_num_vline = 0;

	for (p = s, i = 0; *p != '\0'; p++)
	{
		if (*p != ' ' && *p != '\t')
			temp[i++] = *p;
		if(*p == '|')
			num_of_vline++;
	}
	temp[i] = '\0';

#ifndef FOR_TEST
	Exclude(temp, error);
#endif

	if(num_of_vline % 2)
	{
		ShowMeg("\' | \' must be paired");
		(*error)++;
		return;
	}
	// replace '\' as '[' or ']'
	for(p = temp; *p != '\0'; p++)
	{
		if(*p == '|')
		{
			if((++half_num_vline) % 2)
				*p = '[';
			else
				*p = ']';
		}
	}

	//½øÒ»²½´¦Àí
	for(p = temp,i = 0; *p != '\0'; p++)
	{
		//½«ËùÓÐ´óÐ´×ÖÄ¸Í³Ò»ÎªÐ¡Ð´
		if(*p >= 'A' && *p <= 'Z')
			t[i++] = *p + 32;

		//´¦ÀíÖîÈç 3*-2, (3)/-2 5%-sin(pi/2)ÕâÑùµÄÊäÈë£¬ ·½·¨ÊÇ¼ÓÉÏÀ¨ºÅ,²¢²¹Áã
		else if ((*p == '*' || *p == '/' || *p == '%' || *p == '^' || *p == '@' || *p == '\\') && *(p + 1) == '-')
		{
			t[i++] = *p++; //´æÈëµ±Ç°µÄ·ûºÅ
			t[i++] = '(';
			t[i++] = '0';

			t[i++] = *p++;  //´æÈë¸ººÅ

			//½«¸ººÅºó¡¢ÏÂÒ»¸ö²Ù×÷·ûÇ°µÄËùÓÐÄÚÈÝ´æÈëtÊý×é£¬µ«Èô´æÔÚÀ¨ºÅ»ò¾ø¶ÔÖµ·ûºÅ£¬Ôò¿ÉÄÜÄÚº¬²Ù×÷·û£¬´ËÊ±ÕâÐ©²Ù×÷·ûÒ²Ó¦±»´æÈë£¨ÓÃflagÀ´±êÖ¾ÊÇ·ñº¬ÓÐÀ¨ºÅ»ò¾ø¶ÔÖµ·ûºÅ£©¡£
			while ((*p != '+' && *p != '-' && *p != '*' && *p != '/' && *p != '%' && *p != '^' && *p != '@' && *p != '\0' && *p != '\\') || (flag))
			{
				if (*(p + 1) == '(' || *(p + 1) == '[')
					flag++;
				else if (*(p + 1) == ')' || *(p + 1) == ']')
					flag--;

				t[i++] = *p++;
			}
			t[i++] = ')';
			if (*p == '\0')
				p--;
			else
				t[i++] = *p;
				
		}
			
		else if((*p == '-' || *p == '+') && (*(p - 1) == '('||*(p-1) == '[')) //³öÏÖ8*(-2) cos(+(2)) ÕâÑùµÄÇé¿ö£¬ ²¹Áã
		{
#ifndef FOR_TEST
			t[i++] = '0';
#endif
			if(*p == '-')
				t[i++] = '0';
			t[i++] = *p;
		}
			
		//ÏàÍ¬µÄÔËËã·û³öÏÖ¶à¸ö£¬´¦ÀíÎªÒ»¸ö
		else if (*p == *(p + 1) && (*p == '*' || *p == '/' || *p == '+' || *p == '-' || *p == '@' || *p == '%' || *p == '^' || *p == '!' || *p == '\\'))
		{
			t[i++] = *p;
			string meg("\"");
			meg = meg & *p & "\" is repeated from " & (p-temp+1);
			while (*p == *(p + 1))
				p++;
#ifdef FOR_TEST
			ShowMeg(meg);
			(*error)++;
#else
			meg  = meg & " to " & (p - temp + 1) & ", handled as one.";
			ShowMeg(meg, SHOW_YELLOW);
#endif
		}

		else if(*p == 'i')
		{
#ifdef FOR_TEST
			if((*(p+1) == '[' || (isalpha(*(p+1))) || *(p+1) == '('|| (*(p+1) >= '0' && *(p+1) <= '9')) && *(p-1) != 'p')
#else
			if((isalpha(*(p+1))) || (*(p+1) >= '0' && *(p+1) <= '9') && *(p-1) != 'p')				
#endif
			{
				string meg("Error: i cannot be followed by ");
				if(*(p+1) != '[')
					meg = meg & *(p+1);
				else
					meg = meg & '|';

				if(!(*(p-1) == 'l' && *(p+1) == 'n') && !(*(p-1) == 'r' && *(p+1) == 'c') && !(*(p-1) == 's' && *(p+1) == 'n') && !(*(p-1) == 'd' && *(p+1) == 's') )
				{
#ifndef FOR_TEST
					ShowMeg(meg);
#endif
					(*error)++;
				}
			}

			if(*(p-1) == ')' || *(p-1) == ']')
				t[i++] = '*';
			t[i++] = *p;
		}

		else if(*p == '\r' || *p == '\n')
		{
			cout << "Windows Error" << endl;
			p++;
		}

		else
			t[i++] = *p;

	}
	t[i] = '\0';

	strcpy(s, t);
	if(!ShowSomething_or_not(s))
	{
		if ((s[strlen(s) - 1] == '+') || (s[strlen(s) - 1] == '-') ||
			(s[strlen(s) - 1] == '*') || (s[strlen(s) - 1] == '/') ||
			(s[strlen(s) - 1] == 'n') || (s[strlen(s) - 1] == 'g') ||
			(s[strlen(s) - 1] == '\\'))
		{
			string meg("wrong end with ");
			meg = meg & s[strlen(s) - 1];
			ShowMeg(meg);
			
			(*error)++;
		}
	}
	else
		*(error) = 0;
}

 /*
  * Description:void Exclude(char * const s, int *error);
  * Calls:		None.
  * Called By:	StringHandle();
  * Input:		's' is the original input string, the value of error is obvious.
  * Output:		Maybe some error-message.
  * Return:		None.
  * Others:		This function is used to check if there are invalid characters.
  *				maybe i should use some regex expression for convenient.
  */
void Exclude(char * const s, int *error)
{
	for(char *p = s; *p != '\0'; p++)
		if(!isalpha(*p) && !isdigit(*p) && !IsOperator(*p) && *p != '!' && *p != '\0' && *p != '\n' && *p != '\r' && *p != '.')
		{
			string meg("Error: invalid character: ");
			int c = *p;
			cout << "c = " << c << endl;
			meg = meg & *p;
			ShowMeg(meg);
			(*error)++;
		}
}


 /*
  * Description:bool just_for_test(char* const s, int *error);
  * Calls:		ShowMeg().
  * Called By:	StringHandle();
  * Input:		's' is the original input string, the value of error is obvious.
  * Output:		Maybe some error-message.
  * Return:		None.
  * Others:		This function is only used for checking, since there are some exceptions(errors) i dont think they are.
  */
void just_for_test(char* const s, int *error)
{
	//judge if the beginning of this input is valid or not
	if(*s == 'p' || *s == 'e' || *s == '|' || *s == '-' || *s == 'i' || *s == 'a' || *s == 'c' || *s == '(' || (*s >= '0' && *s <= '9') || *s == 'd'|| *s == '[')
	{
		;//pass
	}
	else
	{
		string meg("Error: ");
		meg = meg & *s & " is not a valid beginning of an expression";
		ShowMeg(meg);
		ShowMeg("Expression must begin with numbers, \'|\', \',\', \'i\',\'(\' or some other functions");
		(*error)++;
	}

	char *p = s;
	int num_of_right_col = 0;
	//judge if middle part of this input is valid or not
	for(p = s; *p != '\0'; p++)
	{
		if(*p == '|')
		{	
			num_of_right_col++;
			if( (*(p+1) == '+' || *(p+1) == '-' || *(p+1) == '*' || *(p+1) == '/') && num_of_right_col%2)
			{
				string meg("Error: operator ");
				meg = meg & *p & " shouldn't be followed by " & *(p+1) & ",pos: " & (p-s);
				ShowMeg(meg);
				(*error)++;
			}				
		}

		if(*p == '+' || *p == '-' || *p == '*' || *p == '/')
		{
			if(*(p+1) == ')' || *(p+1) == '+' || *(p+1) == '-' || *(p+1) == '*' || *(p+1) == '/')
			{
				string meg("Error: operator ");
				meg = meg & *p & " shouldn't be followed by " & *(p+1) & ",pos: " & (p-s);
				ShowMeg(meg);
				(*error)++;
			}
		}

		else if(*p >= '0' && *p <= '9')
		{
			if(*(p+1) == '(')
			{
				string meg("Error: number ");
				meg = meg & *p & " shouldn't be followed by " & *(p+1) & ",pos: " & (p-s);
				ShowMeg(meg);
				(*error)++;
			}
			if(*(p+1) == '|' && (num_of_right_col % 2 == 0))
			{
				string meg("Error: number ");
				meg = meg & *p & " shouldn't be followed by " & *(p+1) & ",pos: " & (p-s);
				ShowMeg(meg);
				(*error)++;
			}
			if(*(p+1) == '[')
			{
				string meg("Error: number ");
				meg = meg & *p & " shouldn't be followed by |" & ",pos: " & (p-s);
				ShowMeg(meg);
				(*error)++;
			}

			if(*(p+1) == 'c' || *(p+2) == 'j' || *(p+3) == 'g')
			{
				string meg("Error: number ");
				meg = meg & *p & " shouldn't be followed by " & "cjg" & ",pos: " & (p-s);
				ShowMeg(meg);
				(*error)++;	
			}
			if(*(p+1) == 'a' || *(p+2) == 'r' || *(p+3) == 'g')
			{
				string meg("Error: number ");
				meg = meg & *p & " shouldn't be followed by " & "arg" & ",pos: " & (p-s);
				ShowMeg(meg);
				(*error)++;	
			}
		}

		else if(*p == 'i')
		{
			if(*(p+1) == '(' || *(p+1) == 'i' || (*(p+1) >= '0' && *(p+1) <= '9'))
			{
				string meg("Error: ");
				meg = meg & *p & " shouldn't be followed by " & *(p+1) & ",pos: " & (p-s);
				ShowMeg(meg);
				(*error)++;
			}
			else if(*(p+1) == '|' && (num_of_right_col % 2 == 0))
			{
				string meg("Warning: ");
				meg = meg & *p & " be followed by " & *(p+1) & " is not advisable" & ",pos: " & (p-s);
				ShowMeg(meg, SHOW_YELLOW);
				(*error)++;	
			}
		}

		else if(*p == '(')
		{
			if(*(p+1) == ')' || *(p+1) == '+'/* || *(p+1) == '-'*/ || *(p+1) == '*' || *(p+1) == '/')
			{
				string meg("Error:　operator ");
				meg = meg & *p & "　shouldn't be followed by " & *(p+1) & ",pos: " & (p-s);
				ShowMeg(meg);
				(*error)++;
			}
		}

		else if(*p == ')')
		{
			if(*(p+1) == '(' || *(p+1) == '|' || (*(p+1) >= '0' && *(p+1) <= '9'))
			{
				string meg("Error: operator ");
				meg = meg & *p & " shouldn't be followed by " & *(p+1) & ",pos: " & (p-s);
				ShowMeg(meg);
				(*error)++;
			}
			if(*(p+1) == 'c' || *(p+2) == 'j' || *(p+3) == 'g')
			{
				string meg("Error: operator ");
				meg = meg & *p & " shouldn't be followed by " & "cjg" & ",pos: " & (p-s);
				ShowMeg(meg);
				(*error)++;	
			}
			if(*(p+1) == 'a' || *(p+2) == 'r' || *(p+3) == 'g')
			{
				string meg("Error: operator ");
				meg = meg & *p & " shouldn't be followed by " & "arg" & ",pos: " & (p-s);
				ShowMeg(meg);
				(*error)++;	
			}
		}


	}

	--p;
	//judge the end of this input is valid or not
	{
		if(*p == 'e' || *p == '|' || *p == 'i' || *p == ')' || (*p >= '0' && *p <= '9') || *p == ']' || *p == 's')
			;
		else
		{
			ShowMeg("Expression must be end with numbers, \'|\', \'i\' or \')\'");
			(*error)++;
		}

	}
}
