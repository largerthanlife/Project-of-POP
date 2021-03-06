//
//  Reversi.cpp
//  ReversiClient
//
//  Created by ganjun on 2018/3/6.
//  Copyright © 2018年 ganjun. All rights reserved.
//

#include "Reversi.h"
#include <algorithm>
#include <cmath>
//#define CONSOLEOUTPUT
//#define TEST
#define random(x) (rand()%x)

Reversi::Reversi(){
    //client_socket = ClientSocket();
    oppositeColor = ownColor = -1;

	index = 0;

	time_t lt; 
	lt = time(NULL); 
	time_stamp = (int)lt%1000;

	// 基于当前系统的当前时间
	time_t now = time(0);
	tm *ltm = localtime(&now);
	time_stamp = to_string((long double)ltm->tm_hour)+"." + to_string((long double)ltm->tm_min)+"." + to_string((long double)ltm->tm_sec) ;

	//_mkdir("history");
}

Reversi::~Reversi(){};

/*
 send id and password to server by socket
 rtn != 0 represents socket transfer error
 */
void Reversi::authorize(const char *id , const char *pass)
{
    client_socket.connectServer();
    std::cout << "Authorize " << id << std::endl;
    char msgBuf[BUFSIZE];
    memset(msgBuf , 0 , BUFSIZE);
    msgBuf[0] = 'A';
    memcpy(&msgBuf[1] , id , 9);
    memcpy(&msgBuf[10] , pass , 6);
    int rtn = client_socket.sendMsg(msgBuf);
    if (rtn != 0) printf("Authorized Failed!\n");

}

// 用户id输入，服务器上需要有对应的账号密码：对应文件 players-0.txt
void Reversi::gameStart()
{
    char id[12] = {0}, passwd[10] = {0};
    //char id[12] = "111111110", passwd[10] = "123456";
    printf("ID: %s\n" , id);
    scanf("%s" , id);
    printf("PASSWD: %s\n", passwd);
    scanf("%s", passwd);
    
    authorize(id, passwd);
    
    printf("Game Start!\n");
    
    for (int round = 0 ; round < ROUNDS ; round++){
        roundStart(round);
        oneRound();
        roundOver(round);
    }
    gameOver();
    client_socket.close();
}

void Reversi::gameOver()
{
    printf("Game Over!\n");
}

// 发一次消息，走哪一步，等两个消息，1.自己的步数行不行 2.对面走了哪一步

void Reversi::roundStart(int round)
{
    printf("Round %d Ready Start!\n" , round);
    
    // first time receive msg from server
    int rtn = client_socket.recvMsg();
    if (rtn != 0) return ;
    if(strlen(client_socket.getRecvMsg()) < 2)
        printf("Authorize Failed!\n");
    else
        printf("Round start received msg %s\n", client_socket.getRecvMsg());
    switch (client_socket.getRecvMsg()[1]) {
            // this client : black chessman
        case 'B':
            ownColor = 0;
            oppositeColor = 1;
            rtn = client_socket.sendMsg("BB");
            printf("Send BB -> rtn: %d\n", rtn);
            if (rtn != 0) return ;
            break;
        case 'W':
            ownColor = 1;
            oppositeColor = 0;
            rtn = client_socket.sendMsg("BW");
            printf("Send BW -> rtn: %d\n", rtn);
            if (rtn != 0) return ;
            break;
        default:
            printf("Authorized Failed!\n");
            break;
    }
}

void Reversi::oneRound()
{
	init();

    int STEP = 1;
    switch (ownColor) {
        case 0:
            while (STEP < 10000) {
                
                pair<int,int> chess = step();                        // take action, send message
                
                // lazi only excute after server's message confirm  in observe function
                generateOneStepMessage(chess.first,chess.second);
                
                if (observe() >= 1) break;     // receive RET Code
                
                if (observe() >= 1) break;    // see white move
                
				STEP++;
                ////saveChessBoard();
            }
            printf("One Round End\n");
            break;
        case 1:
            while (STEP < 10000) {
                
                if (observe() >= 1) break;    // see black move
                
                pair<int,int> chess = step();                        // take action, send message
                // lazi only excute after server's message confirm  in observe function
                generateOneStepMessage(chess.first,chess.second);
                
                
                if (observe() >= 1) break;     // receive RET Code
                STEP++;
				//saveChessBoard();
            }
            printf("One Round End\n");
            break;
            
        default:
            break;
    }
}

void Reversi::roundOver(int round)
{
    printf("Round %d Over!\n", round);
    // reset initializer
    
    ownColor = oppositeColor = -1;
	cout << "error = " << error << endl;
}

int Reversi::observe()
{
    int rtn = 0;
    int recvrtn = client_socket.recvMsg();
    if (recvrtn != 0) return 1;
   // printf("receive msg %s\n" , client_socket.getRecvMsg());
    switch (client_socket.getRecvMsg()[0]) {
        case 'R':
        {
            switch (client_socket.getRecvMsg()[1]) {
                case 'Y':   // valid step
                    switch (client_socket.getRecvMsg()[2]) {
                        case 'P':   // update chessboard
                        {
                            int desRow = (client_socket.getRecvMsg()[3] - '0') * 10 + client_socket.getRecvMsg()[4] - '0';
                            int desCol = (client_socket.getRecvMsg()[5] - '0') * 10 + client_socket.getRecvMsg()[6] - '0';
                            int color = (client_socket.getRecvMsg()[7] - '0');
                            handleMessage(desRow, desCol, color);                            
                            printf("a valid step of : (%d %d)\n" , desRow , desCol);
                            break;
                        }
                        case 'N':   // RYN: enemy wrong step
                        {
                            //
                            printf("a true judgement of no step\n");
                            int desRow = -1;
                            int desCol = -1;
                            
                            int color = (client_socket.getRecvMsg()[3] - '0');
                            handleMessage(desRow, desCol, color);
                            break;
                        }
                    }
                    
                    break;
                case 'W':
                    // invalid step
                    switch (client_socket.getRecvMsg()[2]) {
                        case 'P':{
                            int desRow = (client_socket.getRecvMsg()[3] - '0') * 10 + client_socket.getRecvMsg()[4] - '0';
                            int desCol = (client_socket.getRecvMsg()[5] - '0') * 10 + client_socket.getRecvMsg()[6] - '0';
                            
                            int color = (client_socket.getRecvMsg()[7] - '0');
                            printf("Invalid step , server random a true step of : (%d %d)\n" , desRow , desCol);
							error++;
                            handleMessage(desRow, desCol, color);
                            break;
                        }
                        case 'N':{
                            printf("a wrong judgement of no step\n");
                            int desRow = -1;
                            int desCol = -1;
                            
                            int color = (client_socket.getRecvMsg()[3] - '0');
                            handleMessage(desRow, desCol, color);
                            break;
                        }
                        default:
                            break;
                    }
                    break;
                case '1':
                    
                    printf("Error -1: Msg format error!\n");
                    rtn = -1;
                    break;
                case '2':
                    
                    printf("Error -2: Corrdinate error!\n");
                    rtn = -2;
                    break;
                case '4':
                    
                    printf("Error -4: Invalid step!\n");
                    rtn = -4;
                    break;
                default:
                    
                    printf("Error -5: Other error!\n");
                    rtn = -5;
                    break;
            }
            break;
        }
        case 'E':
        {
            switch (client_socket.getRecvMsg()[1]) {
                case '0':
                    // game over
                    rtn = 2;
                    break;
                case '1':
                    // round over
                    rtn = 1;
                default:
                    break;
            }
            break;
        }
        default:
            break;
    }
	saveChessBoard();
    return rtn;
}

void Reversi::generateOneStepMessage(int row, int col)
{
    //char msg[BUFSIZE];
    //memset(msg , 0 , sizeof(msg));
    //
    ////put row and col in the message
    //msg[0] = 'S';
    //msg[1] = 'P';
    //msg[2] = '0' + row / 10;
    //msg[3] = '0' + row % 10;
    //msg[4] = '0' + col / 10;
    //msg[5] = '0' + col % 10;
    //msg[6] = '\0';
    //
    ////print
    //printf("generate one step at possition (%2d,%2d) : %s\n", row , col , msg);
    //
    //
    //client_socket.sendMsg(msg);
	char msg[BUFSIZE];
	memset(msg , 0 , sizeof(msg));


	//put row and col in the message
	msg[0] = 'S';
	msg[1] = 'P';
	msg[2] = '0' + row / 10;
	msg[3] = '0' + row % 10;
	msg[4] = '0' + col / 10;
	msg[5] = '0' + col % 10;
	msg[6] = '\0';
	if(row < 0 || col < 0){
		row = -1;
		col = -1;
	}
	if(row == -1 && col == -1){
		msg[2] = '-';
		msg[3] = '1';
		msg[4] = '-';
		msg[5] = '1';
	}
	//print
	printf("generate one step at possition (%2d,%2d) : %s\n", row , col , msg);


	client_socket.sendMsg(msg);
}

/*-------------------------last three function--------------------------------
 * step : find a good position to lazi your chess.
 * saveChessBoard : save the chess board now.
 * handleMessage: handle the message from server.
 * 类中存在ownColor表示自己当前的棋子颜色，0表示黑棋，1表示白棋
 */

extern bool (*toarray[8])(int&, int&);

void Reversi::saveChessBoard()
{

	ofstream file(filename,ios::app);
	if(!file)
	{
		cout << "somthing wrong" << endl;
	}
	else
	{

		file << "-----------------------------------" << endl;
		file << "|@| |0| |1| |2| |3| |4| |5| |6| |7|" << endl;
		file << "-----------------------------------" << endl;

		for(int i = 0; i < ROWS; i++)
		{	
			file << "|" << i << "| " ;
			for(int j = 0; j < COLS; j++)
			{
				file << "|";
				if(chessnow[i][j] == NOSTEP)
					file << " ";
				else if(chessnow[i][j] == BANSTEP)
					file << "?";
				else
				{
					char a;
					if(chessnow[i][j] == WHITE)
						a = 'O';
						//a = '○';					
					else if(chessnow[i][j] == BLACK)
						a = '#';
						//a = '●';					
					else
					{
						cout << "illegal step status" << endl;
						exit(0);
					}
					file << a;
				}
				file << "| "; 
			}
			file << endl;
			file << "-----------------------------------" << endl;
		}
		file << "oppositecolore: " << countcolor(oppositeColor) << "," << "owncolor: " << countcolor(ownColor) << endl;

	}
#ifdef TEST
	if(stepnum > 61 && countcolor(oppositeColor) > countcolor(ownColor))
		file << "Result: You lost" << endl << endl;
	else if(stepnum > 61 && countcolor(oppositeColor) < countcolor(ownColor))
		file << "Result: You win" << endl << endl;
	else if(stepnum > 61 && countcolor(oppositeColor) == countcolor(ownColor))
		file << "Result: Qquel" << endl << endl;
#endif
	file << endl;
	file.close();

#ifdef CONSOLEOUTPUT
	cout << "-----------------------------------" << endl;
	cout << "|@| |0| |1| |2| |3| |4| |5| |6| |7|" << endl;
	cout << "-----------------------------------" << endl;

	for(int i = 0; i < ROWS; i++)
	{	
		cout << "|" << i << "| " ;
		for(int j = 0; j < COLS; j++)
		{
			cout << "|";
			if(chessnow[i][j] == NOSTEP)
				cout << " ";
			else if(chessnow[i][j] == BANSTEP) 
				cout << "?";
			else
			{
				char a;
				if(chessnow[i][j] == WHITE)
					//a = '●';					
					a = 'O';
				else if(chessnow[i][j] == BLACK)
					//a = '○';					
					a = '#';
				else
				{
					cout << "illegal step status" << endl;
					exit(0);
				}
				cout << a;
			}
			cout << "| "; 
		}
		cout << endl;
		cout << "-----------------------------------" << endl;
	}
	cout << "oppositecolore: " << countcolor(oppositeColor) << "," << "owncolor: " << countcolor(ownColor) << endl;

#endif

}

int Reversi::countcolor(int color)
{
	int sum = 0;
	for(int i = 0; i < ROWS; i++)
		for(int j = 0; j < COLS; j++)
			if(chessnow[i][j] == color)
				sum++;
	return sum;
}

void Reversi::handleMessage(int row, int col, int color)
{
	void reverse(int row, int col, int color, vector<vector<int>> &chessnow);
	//cout<<"********************************************"<<endl;
	//cout<<"in hadnleMessage: color: "<<color<<" " << "pos: "<<"("<<row<<","<<col<<")"<<endl;
	ofstream file(filename,ios::app);
	file << "step " << stepnum++ << ":";
	if(color == ownColor)
		file << "owncolor :" << color << ", pos: " << "(" << row << "," << col << ")" << endl;
	else if(color == oppositeColor)
		file << "oppositecolor :" << color << ", pos: " << "(" << row << "," << col << ")" << endl;
	file.close();

	////this pos has been step
	//if(chessnow[row][col] != -1)
	//{
	//	cout << "错误，在("<< row << ", "<< col << "已经下过" << endl;
	//	return;
	//}
	
	//解除禁手
	for(int i = 0; i < ROWS; i++)
		for(int j = 0; j < COLS; j++)
			if(chessnow[i][j] == BANSTEP)
				chessnow[i][j] = NOSTEP;

	//no step to handle
	if(row == -1 && col == -1)
		return;

	//设置禁手
	if(color == oppositeColor)
	{
		hasstep.push_back(make_pair(row, col));
		if(row < ROWS - 1)
			if(chessnow[row+1][col] == NOSTEP)
				chessnow[row+1][col] = BANSTEP;

		if(col < COLS - 1)		
			if(chessnow[row][col+1] == NOSTEP)
				chessnow[row][col+1] = BANSTEP;

		if(row > 0)
			if(chessnow[row-1][col] == NOSTEP)
				chessnow[row-1][col] = BANSTEP;

		if(col > 0)
			if(chessnow[row][col-1] == NOSTEP)
				chessnow[row][col-1] = BANSTEP;
	}

	reverse(row, col, color, chessnow);
	//vector<pair<int,int>> tempstep;
	//for(int i = 0; i < chessnow.size(); i++)
	//	for(int j = 0; j < chessnow[i].size(); j++)
	//		if(chessnow[i][j] == oppositeColor)
	//		{
	//				tempstep.push_back(make_pair(i,j));
	//		}
	//hasstep = tempstep;
}

pair<int,int> Reversi::step()
{
    //此处写算法
	bool rightway = false;

	int max = 0;
	int ret;
	pair<int, int> bestpair = make_pair(-1,-1);
	pair<int, int> temppair;
	for(vector<pair<int,int>>::iterator i = hasstep.begin(); i != hasstep.end(); i++)
	{
		if( (ret = whichpair(*i, chessnow, temppair)) > max )
		{
			bestpair = temppair;
			max = ret;
		}
		else if(ret == -1)
		{
			bestpair = temppair;
			break;
		}
	}
	if(max == 0)
	{
		for (int i = 0; i < ROWS; i++)
			for(int j = 0; j < COLS; j++)
			{
				if(chessnow[i][j] == oppositeColor)
				{
					if(ret = whichpair(make_pair(i, j), chessnow, temppair) > max)
					{
						bestpair = temppair;
						max = ret;
					}
					else if(ret == -1)
					{
						bestpair = temppair;
						return bestpair;
					}
				}
			}
		cout << "here : " << bestpair.first << ", " << bestpair.second << endl;
	}
	
	return bestpair;

	//int r = rand()%8;
 //   int c = rand()%(8);
 //   return make_pair(r,c);
}

int Reversi::whichpair(pair<int,int> onestep, vector<vector<int>>& chessnow, pair<int, int>& temppair)
{
	bool isSTAR(pair<int, int> temppair);
	bool isLINE(pair<int,int> temppair);
	bool isCENTER(pair<int,int> temppair);
	bool isBORDER(pair<int,int> temppair);
	bool isEMPTY(pair<int,int> temppair, vector<vector<int>> &chessnow, int color);
	bool comp(pair<pair<int,int>, int> &a, pair<pair<int,int>, int> &b);

	int max = 0;
	int temprow = -1, tempcol = -1;
	int a;
	temppair = make_pair(temprow, tempcol);
	vector<pair<pair<int,int>, int>> vectorpair;
	vector<pair<pair<int,int>, int>> rightstep;

	for(int i = 0; i < DIRECTION; i++)
	{
		int row = onestep.first;
		int col = onestep.second;
		if(toarray[i](row, col))
		{
			if(chessnow[row][col] == NOSTEP)
			{
				temprow = row;
				tempcol = col;

				//special rule1: 
				if((row==0&&col==0) || (row==ROWS-1&&col==COLS-1) || (row==0&&col==COLS-1) || (row==ROWS-1&&col==0) )
				{
					if((a = count(temprow, tempcol, chessnow)) > 0)
					{
						temppair = make_pair(temprow, tempcol);
						return -1;
					}
				}
				a = count(temprow, tempcol, chessnow);
				if( a > max)
				{
					temppair = make_pair(temprow, tempcol);
					max = a;
				}
				else if(a == max)
				{
					;//vectorpair.push_back(make_pair(temprow, tempcol));
				}

				if(a > 0)
				{
					rightstep.push_back(make_pair(make_pair(temprow, tempcol), a));
				}
				

			}
			//else if(chessnow[row][col] == ownColor || chessnow[row][col] == oppositeColor)
			//	;
		}
	}


	if(temppair.first == -1 && temppair.second == -1)
	{
		//cout << rightstep.size()<<endl;
		return 0;
	}
	else
	{
		if(rightstep.size() == 1)
			return max;
		else
		{
			vector<pair<pair<int,int>, int>> temprightstep;
			for(vector<pair<pair<int,int>, int>>::iterator i = rightstep.begin(); i != rightstep.end(); i++)
			{
				if(isSTAR((*i).first))
				{
					//(*i).second = 1;
					if(isEMPTY((*i).first, chessnow, ownColor))
						(*i).second += 100;
					else
						continue;
				}
				else if(isLINE((*i).first))
				{
					if(isEMPTY((*i).first, chessnow, ownColor))
						(*i).second += 100;
					else
						(*i).second += 3;
				}
				else if(isCENTER((*i).first))
				{
					if(stepnum >= 10)	
						(*i).second += 100;
					else
						(*i).second = 1;
				}
				else if(isBORDER((*i).first))
				{
					(*i).second += 2;
				}

				temprightstep.push_back(*i); 
			}

			if(temprightstep.size() != 0)
			{	
				max = 0;
				for(vector<pair<pair<int,int>, int>>::iterator i = temprightstep.begin(); i != temprightstep.end(); i++)
				{
					if((*i).second > max)
					{
						temppair = (*i).first;
						max = (*i).second;
					}
					else if((*i).second == max && max != 0)
					{
						vectorpair.push_back((*i));
					}
				}
				if(vectorpair.size() > 1)
				{
					int r = rand()%vectorpair.size();
					temppair = vectorpair[r].first;
					max = vectorpair[r].second;
				} 
			}
		}
		return max;
	}
}

int Reversi::count(int row, int col, vector<vector<int>> &chessnow)
{
	int sum = 0;
	int temprow, tempcol;
	bool rightway = false;
	//cout << "original: " << row << "," << col << endl;
	for(int i = 0; i < 8; i++)
	{
		rightway = false;
		temprow = row;
		tempcol = col;

		while(toarray[i](temprow, tempcol))
			if(chessnow[temprow][tempcol] ==oppositeColor)
			{
				continue;
			}
			else if(chessnow[temprow][tempcol] == ownColor)
			{
				rightway = true;
				break;
			}
			//else if(chessnow[temprow][tempcol] == NOSTEP ||chessnow[temprow][tempcol] == BANSTEP)
			else
				break;


		if(rightway)
		{
			temprow = row;
			tempcol = col;
			while(toarray[i](temprow, tempcol))
				if(chessnow[temprow][tempcol] == oppositeColor)
					sum++;
				else
					break;
		}
	}
	//cout << "sum = " << sum << endl;
	return sum;
}

void Reversi::init()
{
	stepnum = 0;
	// -1 表示该位置还未下子
	error = 0;
	chessnow.resize(ROWS);
	for(int i = 0; i < ROWS; i++)
	{
		chessnow[i].resize(COLS);
		for(int j = 0; j < COLS; j++)
		{
			chessnow[i][j] = NOSTEP;
		}
	}
	// 0 表示该子为黑色， 1 为白色

	chessnow[3][3] = chessnow[4][4] = BLACK;
	chessnow[3][4] = chessnow[4][3] = WHITE;

	hasstep.resize(2);
	if(ownColor == 0)
	{
		cout << "I AM BLACK" << endl;
		hasstep[0] = make_pair(3, 4);
		hasstep[1] = make_pair(4, 3);
	}
	else
	{
		cout << "I AM WHITE" << endl;
		hasstep[0] = make_pair(3, 3);
		hasstep[1] = make_pair(4, 4);
	}

	filename = time_stamp + "history" + to_string((long double)index++)+ ".log";
	cout << time_stamp << "," << filename << endl;
	ofstream file(filename , ios::out);
	file.close();
	saveChessBoard();
}

