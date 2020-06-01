/*
 * File name: file_calc.cpp
 * Description: To calculate the expressions in a file, and write their answers to correspond files
 * Author: Liu Zechao
 * Version: 1.0
 * Date: 2018/6/15
 * Function List:	bool IsCalcFile(char* s);
 *					bool file_calc(char* filename, int *error);
 *					void Calc(char * s);	
 */
#include "calculator.h"
#include "show.h"
#include "my_complex.h"

#define PREFIX "calc"
#define POSFIX ".answer"

void ShowMeg(string meg, int color = SHOW_RED);
void ConversionOfBODH(OpcharType *s);
void StringHandle(OpcharType * const s, int *error);
OprandType EvaluateExpression(OpcharType * const s, OprandType ans, OprandType myrand, int *error);
void ShowAnswer(OprandType & ans);
void just_for_test(char* const s, int *error);



/*
 * Description:	bool IsCalcFile(char* s);
 * Calls:		None.
 * Called By:	int main();
 * Input:		the input instruction s.
 * Output:		None.
 * Return:		return if s fits the pattern of "calc filename".
 * Others:		None.
 */
bool IsCalcFile(char* s)
{
	char temp[5];
	strncpy(temp, s, strlen(PREFIX));
	return !strcmp(temp, PREFIX) && strlen(s) > strlen(PREFIX);
}


/*
 * Description:	bool file_calc(char* filename, int *error);
 * Calls:		void ShowMeg(string meg, int color = SHOW_RED);
 *				void ConversionOfBODH(OpcharType *s);
 *				void StringHandle(OpcharType * const s, int *error);
 *				OprandType EvaluateExpression(OpcharType * const s, OprandType ans, OprandType myrand, int *error);
 *				void ShowAnswer(OprandType & ans);
 * Called By:	void Calc(char * s);
 * Input:		filename extracted by Calc(), and error to record the number of  the sum of invalid situations.
 * Output:		open a new file to record the corresponding file, and print the result of each calculating in the same time.
 * Return:		return if the file exists.
 * Others:		Should add some control variables for user to determin whether they want to result to be printed or not.
 */
bool file_calc(char* filename, int *error)
{
	ifstream cal_file(filename, ios::in);
	if(cal_file.is_open())
	{
		char tempfilename[MAXSTRING];
		strcpy(tempfilename, filename);
		strcat(tempfilename, POSFIX);

		ofstream ans_file(tempfilename, ios::out);
		
		char s[MAXSTRING];
		OprandType ans = 0;
		int temperror = 0;
		while(!cal_file.eof())
		{
			cal_file.getline(s, MAXSTRING);
			if(!strcmp(s,""))
				continue;
			else
			{
				temperror = 0;
				cout << "Calculating: " << s << endl;
				ConversionOfBODH(s);
#ifdef FOR_TEST
				just_for_test(s, &temperror);
#endif
				StringHandle(s, &temperror);
				ans = EvaluateExpression(s, ans, 0, &temperror);
				*error += temperror;
				if(fabs(ans.real()) <= 1e-8)
   					ans.real(0);
    			if(fabs(ans.imag()) <= 1e-8)
        			ans.imag(0);
				ans_file << s << endl;
				ans_file << "Answer: ";
				cout << "Answer: ";
				if(0 == temperror)
				{
					if( ans.real() != 0)
						ans_file << ans.real() ;
					if (ans.imag() > 0)
					{
						ans_file << "+";
					}
					if(fabs(ans.imag()) != 0)
						ans_file << ans.imag() << "i";
					if(ans.imag() == 0 && ans.real() == 0)
						ans_file << "0";
					ShowAnswer(ans);
				}
				else
				{
					ans_file << "invalid";
					ShowMeg("invalid", SHOW_RED);
				}
				ans_file << endl << endl;
			}
		}	
		ans_file.close();
		cal_file.close();
		return true;
	}

	else
	{
		cal_file.close();
		return false;
	}
}

/*
 * Description:	void Calc(char * s);
 * Calls:		bool file_calc(char* filename, int *error);
 * Called By:	int main();
 * Input:		the input instruction s.
 * Output:		print the information of this instructions, according to the result of file_calc().
 * Return:		None.
 * Others:		None.
 */
void Calc(char * s)
{
	char filename[MAXSTRING];
	int i = 0;
	for(char *p = s+strlen(PREFIX)+1; *p != '\0'; p++)
		filename[i++] = *p;
	filename[i] = '\0';
	int error = 0;
	if(file_calc(filename, &error))
	{
		string meg("The answer file has been saved in ");
		meg = meg & filename & POSFIX;
		if(0 == error)
		{
			ShowMeg("Done.", SHOW_GREEN);
			ShowMeg("Congratulation, no error occurred", SHOW_GREEN);
			ShowMeg(meg, SHOW_GREEN);
		}
		else
		{
			ShowMeg("\nDone.", SHOW_WHITE);
			string meg("Total error: ");
			meg = meg & error;
			ShowMeg(meg);
		}
	}
	else
		ShowMeg("Error: this file doesn't exist");
}