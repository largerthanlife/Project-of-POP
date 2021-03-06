//
//  Reversi.h
//  ReversiClient
//
//  Created by ganjun on 2018/3/6.
//  Copyright © 2018年 ganjun. All rights reserved.
//

#ifndef Reversi_h
#define Reversi_h
#include <stdio.h>
#include "ClientSocket.h"
#include <vector>
#include <fstream>
#include <ctime>
#include <direct.h>

#define ROWS 8
#define COLS 8
#define ROUNDS 10
#define WHITE 1
#define BLACK 0
#define NOSTEP -1
#define BANSTEP -2
#define DIRECTION 8

using namespace std;

class Reversi{
private:
	int error;
    ClientSocket client_socket;
    int ownColor;
    int oppositeColor;
	vector<vector<int>> chessnow;	//本地棋盘
	vector<pair<int,int>> hasstep;	//对方已走步
	//vector<int> free_direction;		//能走的方向
	int index;	//局数
	int stepnum; //步数
	string time_stamp;
	string filename;
	void init();
	int whichpair(pair<int,int>, vector<vector<int>>&, pair<int, int>&);
	int count(int, int, vector<vector<int>>&);
	int countcolor(int color);
	//function 
	void handleMessage(int row, int col, int color);

	 // according to chessman position (row , col) , generate one step message in order to send to server
    void generateOneStepMessage(int row , int col);
    
    pair<int,int> step();
    
    void saveChessBoard();

public:
    Reversi();
    ~Reversi();
    
    void authorize(const char *id , const char *pass);
    
    void gameStart();
    
    void gameOver();
    
    void roundStart(int round);
    
    void oneRound();
    
    void roundOver(int round);
    
    int observe();
    
};

#endif /* Reversi_h */
