/*
 * File name: equation.cpp
 * Description:	To find the solution of the input-equation
				Use EvaluateExpression() to seek the value of the left-string and right-string indenpendly
				Compare their result ,and get the first right answer
 * Author: Liu Zechao
 * Version: 1.3
 * Date: 2017/6/10
 * Function List:	bool SolvingEquations(OpcharType *s, OprandType *answer);
 */

#include "calculator.h"
#include "my_complex.h"


OprandType EvaluateExpression(OpcharType * const s, OprandType ans, OprandType myrand, int *flag);

/*
 * Description:	bool SolvingEquations(OpcharType *s, OprandType *answer);
 * Calls:		OprandType EvaluateExpression(OpcharType * const s, OprandType ans, OprandType myrand, bool *flag).
 * Called By:	int main();
 * Input:		's' is the equation that has been pretreated and BODH-convert;
 *				A OprandType-type pointer 'answer',which point to the memory of solution of the equation.
 * Output:		None.
 * Return:		If finds the solution,then return 0,else return 1.
 * Others:		Divide the equation according to the position of '=' into two parts, called leftstring and rightstring.
 *				A variable 'x' loops through [-1000,1000] , each time plus 0.001.
 *				Seek the value of leftstring and rightstring independently.
 *				If the absolute value of the difference of their value meets the qualifications, 
 *					the memory that 'answer' points would be assigned the value of 'x', and return EXIT_SUCCESS.
 *				If not, return EXIT_FAILURE.
 */
 bool SolvingEquations(OpcharType *s, OprandType *answer)
{
	OpcharType leftstring[MAXSTRING];
	OpcharType rightstring[MAXSTRING];
	unsigned int i = 0;
	OprandType x = 0;
	OprandType ans = 0;
	OpcharType *first = s;
	OpcharType *second = s;
	int flag = 0;

	while (*first++ != '=');

	while (*second != '=')
		leftstring[i++] = *second++;

	leftstring[i] = '\0';
	i = 0;

	while (*first != '\0')
		rightstring[i++] = *first++;
	rightstring[i] = '\0';

	for (x -= x.real() - 1000; x.real() <= 1000; x += 0.001)
	{
		//printf("leftstring = %lf\n", EvaluateExpression(leftstring, ans, rands));
		//printf("rightstring = %lf\n", EvaluateExpression(rightstring, ans, rands));
		if (fabs(EvaluateExpression(leftstring, ans, x, &flag) - EvaluateExpression(rightstring, ans, x, &flag)) <= 8 * 1e-3)
			break;
		else if (flag)
			break;
	}
	if (x.real() > 100 || flag )
		//printf("The answer is not in [-100,100]\n");
		return EXIT_FAILURE;
	else
	{
		*answer = x.real();
		return EXIT_SUCCESS;
	}
}