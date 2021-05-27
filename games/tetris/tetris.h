#ifndef TETRIS_H
#define TETRIS_H

#include <windows.h>
#include <time.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <fstream>
#include <string>

class tetris
{
private:
	//tabla za redjanje oblika
	int tabla[20][10] =
	{
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0},
	};

	//svi moguci oblici u tetrisu
	int pices[7][4][4] =
	{
		{
			{0,0,0,0},
			{0,0,0,0} ,
			{1,1,1,1} ,
			{0,0,0,0}
		},

		{
			{0,0,0,0},
			{1,1,1,0} ,
			{0,1,0,0} ,
			{0,0,0,0}
		},

		{
			{0,0,0,0},
			{0,1,1,0} ,
			{0,1,1,0} ,
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,0,0} ,
			{0,1,1,0} ,
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{0,1,1,0} ,
			{1,1,0,0} ,
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,1,0} ,
			{0,0,1,0} ,
			{0,0,0,0}
		},
		{
			{0,0,0,0},
			{1,1,1,0} ,
			{1,0,0,0} ,
			{0,0,0,0}
		}
	};

	//trenutni oblik
	int tetronim[4][4] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };
	//sledeci oblik
	int next[4][4] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

#define SCREEN_WIDTH 43
#define SCREEN_HEIGHT 21

	bool game = true;
	bool keys[4];
	bool falling = false;
	bool flevel = true;
	bool stampanje = true;
	bool rotate = true;
	bool left = true;
	bool right = true;
	int mrotate;

	int pice = 0;
	int next_pice;

	short x, y, max_x, min_x, max_y, tlines, soft_drop_points = 0;

	//int speed[20] = {360000,320000,290000,250000,220000,180000,150000,110000,70000,50000,40000,40000,40000,30000,30000,30000,20000,20000,20000,10000};
	int speed[30] = { 48,43,38,33,28,23,18,13,8,6,5,5,5,4,4,4,3,3,3,2,2,2,2,2,2,2,2,2,2,1 };
	int level = 0;
	int frame = 0;

	int points[5] = { 0,40,100,300,1200 };
	int score = 0;

	int lines = 0;
	int mlines;

	int frotate = 3;

public:
	int odredjivanje_oblika();
	void zamena();
	void rotacija(int pice);
	void pos_rotacija(int pice);
	bool provera(int x, int y);
	int max_X();
	int min_X();
	int max_Y();
	void stapanje(int x, int y);
	int nDlines();
	void stampaj(int x, int y, int score, int level);
	int inicijalizacija();
	int main();
	
};

#endif;