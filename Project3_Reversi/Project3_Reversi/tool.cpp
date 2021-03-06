#include "Reversi.h"
#include <stdio.h>
bool toup(int &temprow, int &tempcol)
{
	temprow--;
	tempcol;
	return temprow >= 0;
}

bool todown(int &temprow, int &tempcol)
{
	temprow++;
	tempcol;
	return temprow < ROWS;
}

bool toleft(int &temprow, int &tempcol)
{
	temprow;
	tempcol--;
	return tempcol >= 0;
}

bool toright(int &temprow, int &tempcol)
{
	temprow;
	tempcol++;
	return tempcol < COLS;
}

bool toupleft(int &temprow, int &tempcol)
{
	temprow--;
	tempcol--;
	return temprow >= 0 && tempcol >= 0;
}

bool toupright(int &temprow, int &tempcol)
{
	temprow--;
	tempcol++;
	return temprow >= 0 && tempcol < COLS;
}

bool todownleft(int &temprow, int &tempcol)
{
	temprow++;
	tempcol--;
	return temprow < ROWS && tempcol >= 0;
}

bool todownright(int &temprow, int &tempcol)
{
	temprow++;
	tempcol++;
	return temprow < ROWS && tempcol < COLS;
}

bool (*toarray[8])(int&, int&) = {toup, todown, toleft, toright, toupleft, toupright, todownleft, todownright};


void reverse(int row, int col, int color, vector<vector<int>> &chessnow)
{
	int reversecolor = color?0:1;
	int temprow, tempcol;
	bool rightway = false;

	chessnow[row][col] = color;

	for(int i = 0; i < 8; i++)
	{
		rightway = false;
		temprow = row;
		tempcol = col;

		while(toarray[i](temprow, tempcol))
			if(chessnow[temprow][tempcol] == color)
				rightway = true;
			else if(chessnow[temprow][tempcol] == NOSTEP)
				break;
		
		temprow = row;
		tempcol = col;
		if(rightway)
			while(toarray[i](temprow, tempcol))
				if(chessnow[temprow][tempcol] == reversecolor)
					chessnow[temprow][tempcol] = color;
				else
					break;
	}
}

bool isSTAR(pair<int, int> temppair)
{
	return (temppair.first == 0 && temppair.second == 1) ||
		   (temppair.first == 0 && temppair.second == COLS-2) ||
		   (temppair.first == 1 && temppair.second == 0) ||
		   (temppair.first == 1 && temppair.second == COLS-1) ||
		   (temppair.first == ROWS-2 && temppair.second == 0) ||
		   (temppair.first == ROWS-2 && temppair.second == COLS-1) ||
		   (temppair.first == ROWS-1 && temppair.second == 1) ||
		   (temppair.first == ROWS-1 && temppair.second == COLS-2)||
		   (temppair.first == 1 && temppair.second == 1) ||
		   (temppair.first == 1 && temppair.second == COLS-2) ||
		   (temppair.first == ROWS-2 && temppair.second == 1) ||
		   (temppair.first == ROWS-2 && temppair.second == COLS-2);
}

bool isEMPTY(pair<int,int> temppair, vector<vector<int>> &chessnow, int color)
{
	if ((temppair.first == 0 && temppair.second == 1) ||
			(temppair.first == 1 && temppair.second == 0) ||
			(temppair.first == 1 && temppair.second == 1))
		return chessnow[0][0] == color;

	if ((temppair.first == 0 && temppair.second == COLS-2) ||
			(temppair.first == 1 && temppair.second == COLS-1) ||
			(temppair.first == 1 && temppair.second == COLS-2))
		return chessnow[0][COLS-1] == color;

	if ((temppair.first == ROWS-2 && temppair.second == 0) ||
			(temppair.first == ROWS-2 && temppair.second == 1) ||
			(temppair.first == ROWS-1 && temppair.second == 1))
		return chessnow[ROWS-1][0] == color;

	if ((temppair.first == ROWS-2 && temppair.second == COLS-1) ||
			(temppair.first == ROWS-1 && temppair.second == COLS-2)||
			(temppair.first == ROWS-2 && temppair.second == COLS-2))
		return chessnow[ROWS-1][COLS-1] == color;

	if (temppair.first == 0 && (temppair.second >= 2 && temppair.second <= 5))
		return chessnow[0][0] == color || chessnow[0][COLS-1] == color;
	
	if (temppair.first == ROWS-1 && (temppair.second >= 2 && temppair.second <= 5))
		return chessnow[ROWS-1][0] == color || chessnow[ROWS-1][COLS-1] == color;

	if (temppair.second == 0 && (temppair.first >= 2 && temppair.first <= 5))
		return chessnow[0][0] == color || chessnow[ROWS-1][0] == color;

	if (temppair.second == 0 && (temppair.first >= 2 && temppair.first <= 5))
		return chessnow[0][COLS-1] == color || chessnow[ROWS-1][COLS-1] == color;
}

bool isLINE(pair<int,int> temppair)
{
	return (temppair.first == 0) || (temppair.first == ROWS - 1) ||
		   (temppair.second == 0) || (temppair.second == COLS - 1);
}

bool isCENTER(pair<int,int> temppair)
{
	return (temppair.first <= 4) && (temppair.first >= 3) &&
		(temppair.second <= 4) && (temppair.second >= 3);
}

bool isBORDER(pair<int,int> temppair)
{
	return (temppair.first <= 5) && (temppair.first >= 2) &&
		(temppair.second <= 5) && (temppair.second >= 2);
}

bool comp(pair<pair<int,int>, int> &a, pair<pair<int,int>, int> &b)
{
	return a.second < b.second;
}

int randomnum(int len)
{
	return rand() % len;
}