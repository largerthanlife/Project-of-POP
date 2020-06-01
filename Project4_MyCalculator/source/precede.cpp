/*
 * File name: precede.cpp
 * Description: compare the priority of  the two OPCHAR.
 * Author: Liu Zechao
 * Version: 1.91
 * Date: 2018/6/14
 * Function List:	OpcharType Precede(OpcharType top, OpcharType get);
 */

#include "calculator.h"
#include "show.h"
void ShowMeg(string meg, int color = SHOW_RED);

/*
 * Description:	OpcharType Precede(OpcharType top, OpcharType get, int *flag);
 * Calls:		None.
 * Called By:	OprandType EvaluateExpression(OpcharType * const s, OprandType ans, OprandType myrand, bool *flag)
 * Input:		Two operator('top' and 'get') that need to be compared.
 * Output:		None.
 * Return:		The priority of these operators, expressed with '>' , '<' or '='.
 * Others:		If they don't have a obvious priority, solute as the same(priority).
 */
OpcharType Precede(OpcharType top, OpcharType get, int *flag) //??ȷ???Ѽ??飬???ݴ??Բ????? ??Ϊ else ???䷶Χ̫?���????ǿ??????????ʱ??????ȷ??
{
	if (top == get)
	{
		if (top == '(' || top == '[')
			return '<';
		else if (top == '\0')
			return '=';
		else
			return '>';
	}

	if ((top == '+') || (top == '-'))
	{
		if ((get == '+') || (get == '-') || (get == ')') || (get == '\0') || (get == ']') || (get == ','))
			return '>';
		else if ((get == '*') || (get == '/') || (get == '(') || (get == '^') || (get == '@') || (get == 's') || (get == 'c') || (get == 't') || (get == 'g') || (get == 'n') || (get == '[') || (get == '%') || (get == 'S') || (get == 'C') || (get == 'T')|| (get == 'R') || (get == 'j') || get =='d' || (get == '\\') )
		{
			return '<';
		}
	}

	if ((top == '*') || (top == '/') || (top == '%') || (top == '\\'))
	{
		if ((get == '(') || (get == '^') || (get == '@') || (get == 's') || (get == 'c') || (get == 't') || (get == 'g') || (get == 'n') || get == '[' || (get == 'S') || (get == 'C') || (get == 'T') || (get == 'R') || (get == 'j') || (get == 'd'))
			return '<';
		else
			return '>';
	}

	if (top == '(')
	{
		if (get == ')' || get == ',')
			return '=';
		else if (get == '\0')
		{
			ShowMeg("Error happens, check the brackets of your input ");
			(*flag)++;
		}
		else
			return '<';
	}

	if (top == '[')
	{
		if (get == ']')
			return '=';
		else
			return '<';
	}

	if (top == '^' || top == '@')
	{
		if (get == '(')
			return '<';
		else if (get == '[')
		{
			string meg("Error: ");
			meg = meg & top & " cannot followed with [ ";
			ShowMeg(meg);
			(*flag)++;
		}
		else
			return '>';
	}

	if (top == 's' || top == 'c' || top == 't' || top == 'S' || top == 'C' || top == 'T'|| top == 'R' || top == 'j' || top == 'd')
	{
		if (get == '(' || get == '^' || get == '@')
			return '<';
		else if (get == '[')
		{
			ShowMeg("Error: trigonometric function cannot followed with [ ");
			(*flag)++;
		}
		else
			return '>';
	}

	if (top == 'n' || top == 'g')
	{
		if (get == '(' || get == '^' || get == '@')
			return '<';
		else if (get == '[')
		{
			string meg("Error: ");
			meg = meg & top & " cannot followed with [ ";
			ShowMeg(meg);
			(*flag)++;
		}
		else
			return '>';
	}

	if (top == '\0')
	{		
		if (get == '\0')
			return '=';
		else if (get == ')')
		{
			ShowMeg("Error happens, check the brackets of your input ");
			(*flag)++;
		}
		else
			return '<';
	}

	if(top == ',')
	{
		if(get == '\0')
			return '>';
		else
			return '<';
	}

	return 0;// modified at 20.5.31 to finish make progress 
}
