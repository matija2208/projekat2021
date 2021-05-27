#ifndef TETRIS_CPP
#define TETRIS_CPP

#include "tetris.h"

int tetris::main()
{

	std::cin >> level;

	next_pice = inicijalizacija();

	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;
	cfi.dwFontSize.Y = 48;
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	system("cls");
	std::cout << "\033[?25l";
	system("color 02");
	while (game)
	{
		//timing
		std::this_thread::sleep_for(std::chrono::milliseconds(17));
		//input--------------
		for (int i = 0; i < 4; i++)
		{
			keys[i] = (0x8000 & GetAsyncKeyState((unsigned char)("\x25\x11\x27\x28"[i])));
		}

		//testing------------
			//stvara novi oblik
		if (!falling)
		{
			falling = true;
			zamena();
			pice = next_pice;
			next_pice = odredjivanje_oblika();
			x = 3;
			y = -1;

			if (!provera(x, y))
				game = false;
			stampanje = true;
		}

		//levo
		if (keys[0] && x - 1 >= min_X() && provera(x - 1, y) && frame % 2 == 0)
		{
			x -= 1;
			stampanje = true;
		}
		//desno
		else if (keys[2] && x + 1 <= max_X() && provera(x + 1, y) && frame % 2 == 0)
		{
			x += 1;
			stampanje = true;
		}

		//dole
		if (keys[3] && y + 1 <= max_Y() && provera(x, y + 1) && frame % 2 == 0)
		{
			y += 1;
			soft_drop_points++;
			stampanje = true;
		}

		//rotacija
		if (keys[1] && rotate && frotate >= 3)
		{
			frotate = 0;
			rotate = false;
			mrotate = frame;
			rotacija(pice);
			if (!provera(x, y) || x > max_X() || x<min_X() || y>max_Y())
			{
				rotacija(pice);
				rotacija(pice);
				rotacija(pice);
			}
			else
			{
				stampanje = true;
			}
		}
		else
		{
			frotate++;
			mrotate = frame;
			rotate = true;
		}

		//ako je level veci od 19/29
		int t = level;
		if (t > 29)
			t = 29;

		//tajming
		if (frame == speed[t])
		{
			//provera mogucnosti spustanja oblika
			frame = -1;
			if (provera(x, y + 1) && y + 1 <= max_Y())
			{
				y += 1;
			}
			else
			{
				//dobijanje novog oblika
				falling = false;
				//brisanje starog oblika
				stapanje(x, y);
				//dobijanje sastavljenih linija ako ih ima i njihovo brisanje sa table
				tlines = nDlines();

				//uvecavanje poena
				score += (points[tlines] * (level + 1));
				score += soft_drop_points;
				soft_drop_points = 0;
				//uvecavanje broja unistenih linija
				lines += tlines;

				//da li je trenutni level pocetni level
				if (flevel)
				{
					//uslov za uvecavanje pocetnog levela levela
					if (lines >= level * 10 + 10 || lines >= max(100, (level * 10 - 50)))
					{
						flevel = false;
						level++;
						mlines = lines;
					}
				}
				//uslov za uvecavanje ostalih levela
				else if (lines - mlines >= 10)
				{
					mlines = lines;
					level++;
				}
			}
			stampanje = true;
		}

		//output-------------
		if (stampanje)
		{
			stampanje = false;
			stampaj(x, y, score, level);
		}

		frame++;
	}
	system("cls");
	
	
	cfi.dwFontSize.Y = 20;
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
	std::cout << "\033[0m\033[?25h";


	return 0;
}

//nalazenje sledeceg oblika (next)
int tetris::odredjivanje_oblika()
{
	srand(time(NULL));

	int pice = rand() % 7;

	for (int i = 0; i < 4 * 4; i++)
	{
		next[i / 4][i % 4] = pices[pice][i / 4][i % 4];
	}

	srand(time(NULL));
	int ponavljanje = rand() % 4;
	for (int i = 0; i < ponavljanje; i++)
		pos_rotacija(pice);
	return pice;
}

//tetronim = next
void tetris::zamena()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			tetronim[i][j] = next[i][j];
		}
	}
}

//rotacija trenutnog oblika (tetronim)
void tetris::rotacija(int pice)
{
	int Tetris[4][4] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	int k = 3;
	int p = pice;
	if (p == 0 || p == 2)
	{
		k = 4;
	}

	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < k; j++)
		{
			int poz = i - (k - 1);
			if (poz < 0)
				poz *= -1;
			Tetris[j][poz] = tetronim[i][j];
		}
	}
	for (int i = 0; i < 4 * 4; i++)
	{
		tetronim[i / 4][i % 4] = Tetris[i / 4][i % 4];
	}
}

//rotacija sledeceg oblika (next)
void tetris::pos_rotacija(int pice)
{
	int Tetris[4][4] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

	int k = 3;
	int p = pice;
	if (p == 0 || p == 2)
	{
		k = 4;
	}

	for (int i = 0; i < k; i++)
	{
		for (int j = 0; j < k; j++)
		{
			int poz = i - (k - 1);
			if (poz < 0)
				poz *= -1;
			Tetris[j][poz] = next[i][j];
		}
	}
	for (int i = 0; i < 4 * 4; i++)
	{
		next[i / 4][i % 4] = Tetris[i / 4][i % 4];
	}
}

//da li se oblik uklapa na zadate kordinate
bool tetris::provera(int x, int y)
{
	int k = 0;
	int l = 0;

	if (y < 0)
	{
		k = -y;
	}
	if (x < 0)
	{
		l = -x;
	}

	for (int i = k; i < 4; i++)
	{
		for (int j = l; j < 4; j++)
		{
			if (tabla[i + y][j + x] != 0 && tetronim[i][j] != 0)
			{
				return false;
			}
		}
	}
	return true;
}

//maximalno x za dati oblik
int tetris::max_X()
{
	int Max_x = 9;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (tetronim[i][j] == 1 && 9 - j < Max_x)
			{
				Max_x = 9 - j;
			}
		}
	}
	return Max_x;
}

//minimalno x za dati oblik
int tetris::min_X()
{
	int Min_x = -3;
	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			if (tetronim[i][j] == 1 && 0 - j > Min_x)
			{
				Min_x = -j;
			}
		}
	}
	return Min_x;
}

//maximalno x za dati oblik
int tetris::max_Y()
{
	int Max_y = 19;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (tetronim[i][j] == 1 && 19 - i < Max_y)
			{
				Max_y = 19 - i;
			}
		}
	}
	return Max_y;
}

//pretvaranje trenutnog oblika u pozadinu
void tetris::stapanje(int x, int y)
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (tetronim[i][j] == 1)
			{
				tabla[y + i][x + j] = 1;
			}
		}
	}
}

//dobijanje broja unistenih linija
int tetris::nDlines()
{
	bool test;
	int num_lines = 0;
	for (int i = 0; i < 20; i++)
	{
		test = true;
		for (int j = 0; j < 10; j++)
		{
			if (tabla[i][j] == 0)
			{
				test = false;
				break;
			}
		}
		if (test)
		{
			num_lines += 1;
			for (int k = i; k > 0; k--)
			{
				for (int l = 0; l < 10; l++)
				{
					tabla[k][l] = tabla[k - 1][l];
				}
			}
			for (int l = 0; l < 10; l++)
			{
				tabla[0][l] = 0;
			}
		}
	}
	return num_lines;
}

//ispis
	//void tetris::stampaj(int x, int y, int score, int level)
	//{
	//	system("cls");
	//	std::cout << "\033[0;32m" << std::endl;

	//	for (int i = 0; i < 21; i++)
	//	{
	//		for(int j=0;j<17;j++)
	//		{
	//			if ((i == 0 && j < 5) || (j == 0 && i < 5) || (i == 5 && j < 5) || j == 5 || j == 16 || (i == 20 && j > 5 && j < 16))
	//			{
	//				//std::cout << char(178) << char(178) << char(178) << char(178);
	//				char buffer[] = {178, 178};
	//				fwrite(buffer, sizeof(char), sizeof(buffer), stderr);
	//				if (j == 16)
	//				{
	//					if (i == 0)
	//					{
	//						char buffer[] = { 32,32,32, 83, 67, 79, 82, 69, 58 };
	//						fwrite(buffer, sizeof(char), sizeof(buffer), stderr);
	//						//std::cout << "   SCORE:";
	//					}
	//					else if (i == 1)
	//					{
	//						std::cout << "   " << score;
	//					}
	//					else if (i == 3)
	//					{
	//						char buffer[] = { 32,32,32, 76, 69, 86, 69, 76, 58 };
	//						fwrite(buffer, sizeof(char), sizeof(buffer), stderr);
	//					}
	//					else if (i == 4)
	//					{
	//						std::cout << "   " << level;
	//					}
	//				}
	//			}
	//			else if ((i > 0 && i < 5) && (j > 0 && j < 5))
	//			{
	//				if (next[i - 1][j - 1] == 1)
	//				{
	//					char buffer[] = { 219,219};
	//					fwrite(buffer, sizeof(char), sizeof(buffer), stderr);
	//				}
	//					//std::cout << char(219) << char(219) << char(219) << char(219);
	//				else
	//				{
	//					char buffer[] = { 32,32};
	//					fwrite(buffer, sizeof(char), sizeof(buffer), stderr);
	//				}
	//					//std::cout << "    ";
	//			}
	//			else if ((i > y - 1 && i < y + 4) && (j - 6 > x - 1 && j - 6 < x + 4) && tetronim[i - y][j - x - 6] == 1)
	//			{
	//				char buffer[] = { 219,219};
	//				fwrite(buffer, sizeof(char), sizeof(buffer), stderr);
	//				//std::cout << char(219) << char(219) << char(219) << char(219);
	//			}
	//			else if (i >= 0 && i < 20 && j>5 && j < 16)
	//			{
	//				if (tabla[i][j - 6] == 1)
	//				{
	//					char buffer[] = { 219, 219};
	//					fwrite(buffer, sizeof(char), sizeof(buffer), stderr);
	//				}
	//				else
	//				{
	//					char buffer[] = { 32,46};
	//					fwrite(buffer, sizeof(char), sizeof(buffer), stderr);
	//				}
	//			}
	//	
	//			else
	//			{
	//				char buffer[] = { 32,32};
	//				fwrite(buffer, sizeof(char), sizeof(buffer), stderr);
	//			}
	//			
	//		}
	//		std::cout << std::endl;
	//	}
	//}

void tetris::stampaj(int x, int y, int score, int level)
{

	
	HANDLE hOutput = (HANDLE)GetStdHandle(STD_OUTPUT_HANDLE);

	COORD dwBufferSize = { 43,21 };
	COORD dwBufferCoord = { 0, 0 };
	SMALL_RECT rcRegion = { 0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1 };

	CHAR_INFO buffer[SCREEN_HEIGHT][SCREEN_WIDTH];

	ReadConsoleOutput(hOutput, (CHAR_INFO*)buffer, dwBufferSize,dwBufferCoord, &rcRegion);
	
	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			if ((i == 0 && j < 5) || (j == 0 && i < 5) || (i == 5 && j < 5) || j == 5 || j == 16 || (i == 20 && j > 5 && j < 16))
			{

				if (j == 16)
					buffer[i][j * 2].Char.UnicodeChar = 35;
				else if (((i == 0 || i == 5) && j != 0) || (i == 20 && j != 5) || (j == 5 && i < 5))
				{
					buffer[i][j * 2].Char.UnicodeChar = 35;
					buffer[i][j * 2 + 1].Char.UnicodeChar = 35;
				}
				else
					buffer[i][j * 2 + 1].Char.UnicodeChar = 35;
			}
			else if ((i > 0 && i < 5) && (j > 0 && j < 5))
			{
				if (next[i - 1][j - 1] == 1)
				{
					buffer[i][j * 2].Char.AsciiChar = 91;
					buffer[i][j * 2 + 1].Char.AsciiChar = 93;
				}
				else
				{
					buffer[i][j * 2].Char.AsciiChar = 32;
					buffer[i][j * 2 + 1].Char.AsciiChar = 32;
				}

			}
			else if ((i > y - 1 && i < y + 4) && (j - 6 > x - 1 && j - 6 < x + 4) && tetronim[i - y][j - x - 6] == 1)
			{
				buffer[i][j * 2].Char.AsciiChar = 91;
				buffer[i][j * 2 + 1].Char.AsciiChar = 93;

			}
			else if (i >= 0 && i < 20 && j>5 && j < 16)
			{
				if (tabla[i][j - 6] == 1)
				{
					buffer[i][j * 2].Char.AsciiChar = 91;
					buffer[i][j * 2 + 1].Char.AsciiChar = 93;
				}
				else
				{
					buffer[i][j * 2].Char.AsciiChar = 32;
					buffer[i][j * 2 + 1].Char.AsciiChar = 46;
				}
			}

			else
			{
				buffer[i][j * 2].Char.AsciiChar = 32;
				buffer[i][j * 2 + 1].Char.AsciiChar = 32;
			}

		}
		char sc[] = { 32,32,32,'S','C','O','R','E',':' };
		char sc_p[9] = { 32,32,32 };
		int S = score;
		char lv[] = { 32,32,32,'L','E','V','E','L',':' };
		char lv_p[9] = { 32,32,32,32,32,32,32,32,32 };
		int L = level;

		for (int i = 8; i > 2; i--)
		{
			sc_p[i] = S % 10 + 48;
			S /= 10;
		}

		for (int i = 4; i > 2; i--)
		{
			lv_p[i] = L % 10 + 48;
			L /= 10;
		}

		for (int k = 0; k < 9; k++)
		{
			if (i == 0)
			{
				buffer[i][k + 34].Char.AsciiChar = sc[k];
			}

			else if (i == 1)
			{
				buffer[i][k + 34].Char.AsciiChar = sc_p[k];
			}

			else if (i == 3)
			{
				buffer[i][k + 34].Char.AsciiChar = lv[k];
			}

			else if (i == 4)
			{
				buffer[i][k + 34].Char.AsciiChar = lv_p[k];
			}

			else
			{
				buffer[i][k + 34].Char.AsciiChar = 32;
			}
		}
	}
	WriteConsoleOutput(hOutput, (CHAR_INFO*)buffer, dwBufferSize, dwBufferCoord, &rcRegion);
}

//set funkcija koje se moraju pozvati pre pocetka igre
int tetris::inicijalizacija()
{
	int pice = odredjivanje_oblika();
	std::this_thread::sleep_for(std::chrono::seconds(1));
	return pice;
}

#endif;
