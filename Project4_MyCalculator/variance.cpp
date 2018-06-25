/*
 * File name: variance.cpp
 * Description: Define the Variance function.
 * Author: Liu Zechao
 * Version: 1.3
 * Date: 2017/6/10
 * Function List:	OprandType Variance(int num);
 */

#include "calculator.h"
#include "my_complex.h"


void ShowLine();
void StringHandle(OpcharType * const s, int *error);

/*
 * Description:	OprandType Variance(int num, int *error);
 * Calls:		void ShowLine();
 *				void StringHandle(OpcharType * const s);
 * Called By:	int main();
 * Input:		'n' is the number of input that needs to calculate.
 * Output:		None.
 * Return:		The variance of input numbers.
 * Others:		Those input numbers must end with a '@' or '$'.
 *				If end with a '@' ,then calculate the sample variance, else ,calculate the (normal) variance.
 *				If 'n' is larger than the amount of cctual input, take the real amount to calculate.
 *				If 'n' is less than the amount of cctual input,take 'n' to calculate.
 */
OprandType Variance(int num, int *error)
{
	OprandType sum = 0, usum = 0, average = 0, variance = 0;
	OpcharType s[MAXNUM] = { 0 };
	OpcharType c;
	unsigned int i = 0, index = 0;

	if (num > 0)
	{
		if (num == 1)
			return 0;
		double *oprand = (double *)calloc(num, sizeof(double));
		if (oprand == NULL)
			printf("�ռ����ʧ�ܣ�������\n");
		printf("\tplease enter your numbers and ends with a '@' or '$':\n\t");
		while ( (scanf("%lf", &oprand[i]) == 1 ) && (i < (unsigned)num))
			sum += oprand[i++];

		while (((c = getchar()) != 'n') && (c != '@') && (c != '$'))
		;//���տ��ܵĶ����ַ�
		//gets_s(s,BUFFER);   //���ն���Ŀո� @ ���� �����з�
		fgets(s, BUFFER, stdin);

		StringHandle(s, error);
		index = i;
		average = sum.real() / i;
		for (i = 0; i < index; i++)
			usum += (average - oprand[i]) * (average - oprand[i]);

			//if (index > 1)
			//���ֶ��壬'@' ��Ӧ������ƫ����
			if ( c == '$' || !strcmp(s, "$"))
				variance = usum.real() / index;
			else if ( c == '@' || !strcmp(s, "@"))
				variance = usum.real() / (index - 1);

			return variance;
			//else{}
	}

	else
	{
		printf("\tinput error!\n\n\tThe number should be larger than zero\n\n");
		printf("\tPress any key to continue\n\n");
		ShowLine();
		return -1;
	}
}