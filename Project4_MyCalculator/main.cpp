/*
 * File name: main.cpp
 * Description: control the functions and show the result.
 * Author: Liu Zechao
 * Version: 3.61
 * Date: 2018/6/15
 * Function List:	int main(); 
 */

#include "calculator.h"
#include "show.h"
#include "my_complex.h"

int Menu();
void ShowLine();
void ShowInformation();
void ShowHelp();
void ShowSample();
void ConversionOfBODH(OpcharType *s);
void StringHandle(OpcharType * const s, int *error);
OprandType EvaluateExpression(OpcharType * const s, OprandType ans, OprandType myrand, int *myflag);
OprandType Variance(int num, int *error);
OprandType StringToOprand(OpcharType * const s);
bool SolvingEquations(OpcharType *s, OprandType *answer);
char *readCmdLine();
int ShowSomething_or_not(char * s);
void ShowSomething(char *s, int index, int use_time, char* instructions[MAXSTRING], int instruction_num);
void Record(char *input, OprandType ans, bool valid, int use_time);
int Init(char **(&instructions), int argc, char * argv[]);
void Clean(char **(&instructions), int instruction_num);
void ShowMeg(string meg, int color = SHOW_RED);
void ShowAnswer(OprandType & ans);
bool IsCalcFile(char* s);
void Calc(char * s);
void just_for_test(char* const s, int *error);


int main(int argc, char *argv[])
{
	system("clear");
	OpcharType save[MAXSTRING];					//the input string
	OpcharType *s;
	OprandType lwrlim = 0, uprlim = 0;			//lower and upper limit of definite integral
	OprandType ans = 0;							//the value of last operation
	OprandType sum = 0;							//used in the formula of seeking the value of definite integral
	OprandType xvalue = 0;						//the value of 'x'
	int num = 0;								//the amount of input to seek the variance 
	int i = 1;									//the label of functions
	unsigned int index = 0;						//record the number of times of operation
	bool flag = 1;								//record the number of times of operation, company with index
	bool isfound = 1;							//whether the solution of equation is founded
	int error = 0;								//the number of times of error-happening
	bool last_error = 0;
	int use_time = 0;
	int record_time = 0;
	int show = 0;
	bool is_calc = false;
	OprandType ans_save;

	char **instructions;
	// system("title MyCalculator");
	// system("color E9");

	int instruction_num =  Init(instructions, argc, argv);

	i = Menu();

#ifndef FLUSH	
	ShowInformation();
	ShowLine();
#endif

	while(1)
	{
		switch(i)
		{
			case CALCULATE:
			{

#ifdef FLUSH
				if(index == 0)
				{
					ShowInformation();
					ShowLine();
				}
#endif
				if(!flag)
				{
					index = 0;
					flag = 1;
				}

				if (index == 0)
				{
					cout << "Enter a mathematical expression:\n\n";
                }
                else
                {
	                if(!show && !is_calc)
					{
#ifndef FOR_TEST							
	                    ConversionOfBODH(s);
#endif
						ans = EvaluateExpression(s, ans_save, xvalue, &error);
	
	                	if (index > 1)
						{
							cout << "\nLast ans: " ;
							if(!last_error)
							{
								ShowAnswer(ans_save);
							}
							else
							{
								ShowMeg("invalid");
							}
							cout << endl;
							cout << "Your expression:\n\n";  

					    	string meg(save);
					    	if(!error)
					    		ShowMeg(meg, SHOW_DEEPGREEN);
						    else
						    {
						    	ShowMeg(meg);
						    	ans = 0;
		              		}
		              	}		

	        
						last_error = error;
						ans_save = ans;
						
						cout << "\nAnswer: " ;
		                if(!error)
							ShowAnswer(ans);
						else
						{
							ShowMeg("invalid");
							// string megtemp("total error: ");
							// megtemp = megtemp & error;
							// ShowMeg(megtemp);
						}
						ShowLine();
#ifdef HISTORY		
						if(strcmp(s,""))
						{
							Record(save, ans, error, ++record_time);
						}
#endif
						printf("Press \"enter\" to return to the menu\n\n");
						
						if (!error)
						{
							printf("Continue:\n\n");
						}
						else
						{
							printf("Input error happens, try again:\n\n");
							// last_error = error;
							// error = 0;
						}
					}
			
					else if(show)
					{
						if(index == 1)
							ShowLine();
						ShowSomething(save, show, use_time, instructions, instruction_num);
						ShowLine();
						printf("Press \"enter\" to return to the menu.\n\n");
						printf("Continue:\n\n");
						show = 0;
					}

					else if(is_calc)
					{
						Calc(save);
						ShowLine();
						printf("Press \"enter\" to return to the menu.\n\n");
						printf("Continue:\n\n");
						is_calc = false;
					}
				}

                s = readCmdLine();
                is_calc = IsCalcFile(s);
				error = 0;

#ifdef FLUSH
                if(index > 0)
     	        {
     	          	system("clear");
					ShowInformation();
					ShowLine();
				}
#endif
#ifndef FLUSH
				if(index > 0)
					ShowLine();
#endif
                if(strcmp(s,""))
                {
                    strcpy(save, s);
                    add_history(s);
                    if(!is_calc)
						StringHandle(s, &error);
#ifndef COUNT_SHOW
                    if(strcmp(s, "show"))
#endif
                	{
 						if(use_time<instruction_num)
	                		strcpy(instructions[use_time++%instruction_num], save); 
    					else
    					{
    						for(int i = 0; i < instruction_num - 1; i++)
    							strcpy(instructions[i], instructions[i+1]);
    						strcpy(instructions[instruction_num - 1],save);
    					}            	
                	}
                }
				index++;
                show = ShowSomething_or_not(s);
                if(show)
                {
                	error = 0;
				}
#ifdef FOR_TEST
				else if(!is_calc)
					just_for_test(s, &error);
#endif
            	if(strlen(s) == 0)
				{
					flag = 0;
					index=0;
					system("clear");
					i = Menu();
				}
				break;
			}
/****************************************************/		
			case VARIANCE:
			{
				if (!flag)
				{
					index = 0;
					flag = 1;
				}
				ShowInformation();
				ShowLine();
				if (index++ > 0)
				{
					if (!error)
					{
						//printf("\tLast variance: %g\n\n", ans);
						cout << "\t" << "Last variance: " << ans << endl << endl;
						// printf("\tLast standard deviation: %g\n\n", sqrt(ans));
						cout << "Last standard deviation: " << sqrt(ans) << endl << endl;
						printf("\tYour inputs are : %d\n\n", num);
					}
					else
					{
						error = 0;
						printf("\tYour inputs are : %d\n\n", num);
						printf("Input error,try again:\n\n");
					}
				}
				if (index <= 1)
				{
					printf("\tplease enter the number that need to input:_\b");
					// scanf_s("%d", &num);
					scanf("%d", &num);
					getchar();
					putchar('\n');
				}

				ans = Variance(num, &error);
				if (ans.real() == -1)
				{
					flag = 0;
					getchar();
					system("clear");
					i = Menu();
					break;
				}
				// printf("\n\tvariance = %lf\n\n", ans);
				cout << "\n\tvariance = " << ans << endl << endl;
				// printf("\tstandard deviation = %lf\n\n", sqrt(ans));
				cout << "\tstandard deviation = " << sqrt(ans) << endl << endl;
				ShowLine();

				printf("\tPress \"enter\" to return to the menu\n\n");
				if (!error && (ans.real() != -1))
				{
					cout << "\t" << "Last variance: " << ans << endl << endl;
					// printf("\tLast standard deviation: %g\n\n", sqrt(ans));
					cout << "Last standard deviation: " << sqrt(ans) << endl << endl;
				}
				else
				{
					printf("Input error, try again\n\n");
					error = 0;
				}
				printf("\tPlease enter the number that need to input:_\b");
				//gets_s(s);
				s=readCmdLine();
				if(strcmp(s,"")) 
					add_history(s);
				if (strlen(s) == 0)
				{
					flag = 0;
					system("clear");
					i = Menu();
				}
				else
				{
					num = StringToOprand(s).real();
					system("clear");
				}
				break;
			}
/****************************************************/		
			case INTEGRAL:
			{
				srand((unsigned)time(NULL));
				if(!flag)
				{
					index = 0;
					flag = 1;
				}
				ShowInformation();
				ShowLine();
				if (index++ > 0)
				{
					if(!error)
						// printf("\tLast ans: %g\n", ans);
						cout << "Last ans: " << ans << endl;
					else
					{
						printf("\tInput error, try again\n");
						error = 0;
					}
				}
				if (index <= 1)
				{
					printf("\n\tEnter a continuous function :\n\n\t");
					//gets_s(s);
					s=readCmdLine();
					if(strcmp(s,"")) 
						add_history(s);

					printf("\n\tEnter lower and upper limit :_ _\b\b\b");
					// scanf_s("%lf %lf", &lwrlim, &uprlim);
					// scanf("%lf %lf", &lwrlim, &uprlim);
					cin >> lwrlim >> uprlim;
					getchar();					//½ÓÊÕÉÏÊ½µÄ»Ø³µ¼ü
				}

				else
				{
					//printf("\tlower and upper limit:%g %g\n\n", lwrlim, uprlim);
					printf("\tcontinuous function :%s\n",s);
				}

				StringHandle(s, &error);
				for(i = 0, sum = 0; i < MAX; i++)				//½«²»Í¬µÄËæ»úÊý×÷Îª x µÄÖµ£¬¼ÆËãº¯ÊýÖµ£¬ ½á¹ûÀÛ¼Óµ½ sum ±äÁ¿ÖÐ
				{
					xvalue = ((double)rand()/(RAND_MAX))*(uprlim - lwrlim) + lwrlim;	//Ã¿´Î²úÉúÒ»¸öÔÚ(lwrlim, uprlim)ÄÚµÄËæ»úÊý
					sum += EvaluateExpression(s, ans, xvalue, &error);
				}

				if(!error)
					// printf("\n\tAnswer :%g\n",ans = sum.real() *(uprlim - lwrlim).real()/MAX);
					cout << "Answer: " << sum.real() *(uprlim - lwrlim).real()/MAX << endl;
				ShowLine();
				printf("\tPress \"enter\" to return to the menu\n\n");
				if (!error)
				{
					// printf("\tLast ans: %g\n", ans);
					cout << "Last ans: " << ans << endl;
					printf("\tContinue:\n\n");
				}
				else
				{
					printf("\tInput error, try again:\n\n");
					error = 0;
				}
				printf("\tEnter a continuous function :\n\n\t");
				//gets_s(s);
				s=readCmdLine();if(strcmp(s,"")) add_history(s);
				if(strlen(s) == 0)
				{
					flag = 0;
					system("clear");
					i = Menu();
				}
				else
				{
					printf("\n\tEnter lower and upper limit :_ _\b\b\b");
					// scanf_s("%lf %lf", &lwrlim, &uprlim);
					// scanf("%lf %lf", &lwrlim, &uprlim);
					cin >> lwrlim >> uprlim;
					getchar();
					system("clear");
					i = 3;
				}
				break;
			}
/****************************************************/		
		
			case EQUATION:
			{
				if (!flag)
				{
					index = 0;
					flag = 1;
				}
				ShowInformation();
				ShowLine();
				if (index++ > 0)
				{
					if(!error)
						// printf("Last ans: %.2lf\n", ans);
						cout << "Last ans: " << ans << endl;
					else
					{
						printf("Input error, try again:\n");
						error = 0;
					}
				}
				printf("Enter a one-dollar-equation:\n\n");
	
				if (index <= 1)
				{	// gets_s(s);
					s=readCmdLine();
					if(strcmp(s,"")) add_history(s);
				}
				else
					puts(s);

				StringHandle(s, &error);
				ConversionOfBODH(s);
				if (isfound = SolvingEquations(s, &ans))
					printf("\nSorry, the answer can not be calculated\n\n");
				else
					// printf("\nAnswer: x = %.2lf\n\n", ans);
					cout << "Answer: x = " << ans << endl << endl; 
				ShowLine();
				printf("Press \"enter\" to return to the menu\n\n");
				if (!isfound)
				{
					// printf("Last ans: x = %.2lf\n", ans);
					cout << "Last ans: x = " << ans << endl;
				}
	
				printf("Continue:\n\n");
				//gets_s(s);
				s=readCmdLine();if(strcmp(s,"")) add_history(s);
				if (strlen(s) == 0)
				{
					flag = 0;
					system("clear");
					i = Menu();
				}
				else
					system("clear");
				break;
			}
/****************************************************/		
		
			case INFORMATION:
			{
				ShowInformation();
				ShowLine();
				ShowHelp();
				ShowLine();
			    // getchar();
                getchar();
				system("clear");
				i = Menu();
				break;
			}
/****************************************************/		
		
			case SAMPLE:
			{
				ShowInformation();
				ShowLine();
				ShowSample();
				ShowLine();
				// getchar();
                getchar();
				system("clear");
				i = Menu();
				break;
			}
/****************************************************/		
		
			default :
			{
				//if(i == 99) index++;
				ShowInformation();
				ShowLine();
				ShowMeg("Input error!");
				ShowMeg("Press any key to continue");
				ShowLine();
				getchar();
				system("clear");
				//if(index == 9) Myshow();
				i = Menu();
			}
		}
	}

	//system("pause");
	Clean(instructions, instruction_num);
	return EXIT_SUCCESS;
}


char *readCmdLine()
{
	static char pszCmdPrompt[] = "\033[35m>>\033[0m";

    char *pszLineRead = NULL;
    char *pszStripLine = NULL;
     
    pszLineRead = readline(pszCmdPrompt);
   
    return pszLineRead;
}

int Init(char **(&instructions), int argc, char * argv[])
{
	int instruction_num = INSTRUCTION;
	int len;
	if(argc == 2)
	{
		char s[BUFFER];
		strcpy(s, argv[1]);
		for(char * temp = s; *temp != '\0';temp++)
			*temp = *(temp+1);
		len = StringToOprand(s).real();
		instruction_num = len;
	}
	
	instructions = (char**) malloc(instruction_num*(sizeof(char*)));
	for(int i = 0; i < instruction_num; i++)
		instructions[i] = (char*)malloc(MAXSTRING*(sizeof(char)));

	for(int i = 0; i < instruction_num; i++)
		strcpy(instructions[i], "Nothing");

#ifdef HISTORY
	time_t rawtime;
	struct tm * timeinfo;
	time(&rawtime);
	timeinfo = localtime(&rawtime);

	string filename = LOGNAME;
	ofstream file_history;

	file_history.open(filename, ios::in);
	if(!file_history.is_open())
	{
		file_history.close();
		file_history.open(filename, ios::out);
		file_history << LOGWORD << endl ;
		file_history << endl;	
#ifdef COLOR_RECORD	
		file_history << "\033[34m--timestamp: \033[0m" << asctime(timeinfo) << endl;
#else
		file_history << "--timestamp: " << asctime(timeinfo) << endl;
#endif
		file_history.close();
	}
	else
	{
		file_history.close();
		file_history.open(filename, ios::app);
		file_history << endl;
#ifdef COLOR_RECORD	
		file_history << "\033[34m--timestamp: \033[0m" << asctime(timeinfo) << endl;
#else
		file_history << "--timestamp: " << asctime(timeinfo) << endl;
#endif
		file_history.close(); 
	}
#endif
	return instruction_num;
	// FLUSH = true;
}

void Clean(char **(&instructions), int instruction_num)
{
	for(int i = 0; i < instruction_num; i++)
		free(instructions[i]);

	free(instructions);
}