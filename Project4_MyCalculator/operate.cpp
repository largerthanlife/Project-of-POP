/*
 * File name: operate.cpp
 * Description: according to the type of "theta",  return the ans of  exression
 * Author: Liu Zechao
 * Version: 1.81
 * Date: 2018/6/15
 * Function List:	OprandType Operate(OprandType a, OpcharType theta, OprandType b, bool *flag);
 */

#include "calculator.h"
#include "show.h"
void ShowMeg(string meg, int color = SHOW_RED);

/*
 * Description:	OprandType Operate(OprandType a, OpcharType theta, OprandType b, int *flag);
 * Calls:		some functions of <math.h>.
 * Called By:	OprandType EvaluateExpression(OpcharType * const s, OprandType ans, OprandType myrand, bool *flag)
 * Input:		'a' and 'b' are the numbers that used to calculated.
 *				'theta' is the operator.
 *				'flag' points to the memory that stores the number of times of error-happen.
 * Output:		None.
 * Return:		The value of  "a{theta}b" or "{theta}b"(according to the type of 'theta').
 * Others:		The memory that 'flag' points should be preset as zero.
 *				'theta' may be a unary operator or binary operator.
 */
OprandType Operate(OprandType a, OpcharType theta, OprandType b, int *flag)
{
	if (theta == '+')
		return a + b;
	else if (theta == '-')
		return a - b;
	else if (theta == '*')
	{
#ifdef FOR_TEST
		// if(b.real() == 0) // b is purely imaginary
		// {
		// 	if(a.imag() != 0)
		// 	{
		// 		ShowMeg("Warning: i should multiply with real number", SHOW_YELLOW);
		// 		(*flag)++;
		// 	}
		// }
  //       if(a.real() == 0)
  //       {
  //           if(b.imag() != 0)
  //           {
  //               ShowMeg("Warning: i should multiply with real number", SHOW_YELLOW);
		// 		(*flag)++;
  //           }
  //       }
#endif
		return a*b;
	}
	else if (theta == '/')
	{
		if(fabs(b.real()) == 0 && fabs(b.imag()) == 0)
		{
			ShowMeg("The Denominator should not be zero");
			(*flag)++;
		}
		else
		{
#ifdef MYCOMPLEX
			return a/b;
#else
			return Operate(a, '*', conj(b), flag) / norm(b);
#endif
		}
	}
	else if (theta == '\\')
	{
		if (b.real() != 0)
			return (int)(a.real()/b.real());
		else
		{
			ShowMeg("The Denominator should not be zero");
			(*flag)++;
		}
	}
	else if (theta == '^')
		{
#ifdef FOR_TEST
			if(b.imag() != 0)
			{
				ShowMeg("The Power exponent must be real num");
				(*flag)++;
			}
#endif
			return pow(a, b);
		}
	else if (theta == '@')
		return pow(a, 1.0 / b);
	else if (theta == 's')
		return sin(b);
	else if (theta == 'c')
		return cos(b);
	else if (theta == 't')
		return tan(b);
	else if (theta == 'S')
	{
		if (fabs(b) <= 1)
			return asin(b);
		else
		{
			ShowMeg("The domain of inverse trigonometric function should be [-1,1]");
			(*flag)++;
		}
	}
	else if (theta == 'C')
	{
		if (fabs(b) <= 1)
			return acos(b);
		else
		{
			ShowMeg("The domain of inverse trigonometric function should be [-1,1]");
			(*flag)++;
		}
	}
	else if (theta == 'T')
		return atan(b);
	else if (theta == 'n')
	{
		if (b.real() <= 0)
		{
			ShowMeg("The antilogarithm of logarithm should be larger than zero");
			(*flag)++;
		}
		else
			return log(b);
	}
	else if (theta == 'g')
	{
		if (b.real() <= 0)
		{
			ShowMeg("The antilogarithm of logarithm should be larger than zero");
			(*flag)++;
		}
		else
			return log10(b);
	}
	else if (theta == '%')
	{
		if (b.real() != 0)
			return fmod(a.real(), b.real());
		else
		{
			string meg("The left operator of ");
			meg = meg & theta & " should not be zero";
			ShowMeg(meg);
			(*flag)++;
		}
	}
	else if(theta == 'R') // arg(complex)
	{
		if(b.real() == 0)
		{					
			ShowMeg("Error: in arg(), the real part of parameter cannot be zero");
			return 0;
		}
		else
			return arg(b);
	}
	else if(theta == 'j')
	{
		return conj(b);
	}

	else if(theta == 'd')
	{
		return pow( pow(a.real()-b.real(),2) + pow(a.imag()- b.imag(),2), 0.5);
	}
}
