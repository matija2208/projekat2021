#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <iostream>
#include <Windows.h>
#include <fstream>

using namespace std;

namespace tictactoe
{
	int tabla[3][3] = { 0,0,0,0,0,0,0,0,0 };
    char t[47][101];
    char x[15][33];
    char o[15][33];

    void brisanje()
    {
        ifstream file("games/tic-tac-toe/tabla.IN");
        char x;
 
        for (int i = 0; i < 47; i++)
        {
            for (int j = 0; j < 101; j++)
            {
                
                file.get(t[i][j]);
            }
        }
        file.close();
    }

    void ispis()
    {
		system("clear");
        for (int i = 0; i < 47; i++)
        {
            for (int j = 0; j < 101; j++)
            {
                cout << t[i][j];
            }
            cout << endl;
        }
    }

    void inicijalizacija_fonta()
    {
        CONSOLE_FONT_INFOEX cfi;
        cfi.cbSize = sizeof(cfi);
        cfi.nFont = 0;
        cfi.dwFontSize.X = 9;
        cfi.dwFontSize.Y = 20;
        cfi.FontFamily = FF_DONTCARE;
        cfi.FontWeight = FW_NORMAL;
        wcscpy_s(cfi.FaceName, L"Consolas"); // Choose your font
        SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
    }

	bool provera(int p)
	{
        if (tabla[0][0] == p && tabla[0][1] == p && tabla[0][2] == p)
            return true;
        else if (tabla[1][0] == p && tabla[1][1] == p && tabla[1][2] == p)
            return true;
        else if (tabla[2][0] == p && tabla[2][1] == p && tabla[2][2] == p)
            return true;
        else if (tabla[0][0] == p && tabla[1][0] == p && tabla[2][0] == p)
            return true;
        else if (tabla[0][1] == p && tabla[1][1] == p && tabla[2][1] == p)
            return true;
        else if (tabla[0][2] == p && tabla[1][2] == p && tabla[2][2] == p)
            return true;
        else if (tabla[0][0] == p && tabla[1][1] == p && tabla[2][2] == p)
            return true;
        else if (tabla[0][2] == p && tabla[1][1] == p && tabla[2][0] == p)
            return true;
        else
            return false;
	}

    void ucitavanje(char file)
    {
        if (file == 'X')
        {
            ifstream F("games/tic-tac-toe/X.IN");

            for (int i = 0; i < 15; i++)
            {
                for (int j = 0; j < 33; j++)
                {
                    F.get(x[i][j]);
                }
            }
            F.close();
        }
        else if (file == 'O')
        {
            ifstream F("games/tic-tac-toe/O.IN");

            for (int i = 0; i < 15; i++)
            {
                for (int j = 0; j < 33; j++)
                {
                    F.get(o[i][j]);
                }
            }
            F.close();
        }
    }

    void popuni(int m, int n, int z)
    {
        int poc[2] = { m * 16,n * 34 };
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 33; j++)
            {
                if(z==1)
                    t[i + poc[0]][j + poc[1]] = x[i][j];
                else
                    t[i + poc[0]][j + poc[1]] = o[i][j];
            }
        }
    }

	int main()
	{
    pocetak:
        brisanje();
        ucitavanje('X');
        ucitavanje('O');
        system("clear");
        int n;
        cout << "\033[?25l";
        system("clear");
        int k[2], K, X[2][5], O[2][4];
        bool T;

		for (int i = 0; i < 5; i++)
		{
		ponovo1:
			ispis();
			
			cout << "Enter coordinate [i] for player X : \033[?25h";
			cin >> X[0][i];
            cout << "\033[?25l";
			cout << "Enter coordinate [j] for player X : \033[?25h";
			cin >> X[1][i];
            cout << "\033[?25l";
            T = false;
            for (int j = 0; j < i; j++)
            {
                if ((X[0][i] == X[0][j] && X[1][i] == X[1][j]) || (X[0][i] == O[0][j] && X[1][i]==O[1][j]))
                {
                    T = true;
                    break;
                }
            }
            if ((X[0][i] > 3 || X[0][i] < 1 || X[1][i] > 3 || X[1][i] < 1) || T)
                goto ponovo1;
			
			tabla[X[0][i]-1][X[1][i]-1] = 1;
            popuni(X[0][i]-1, X[1][i]-1, 1);

            T = provera(1);
            if (T)
            {
                ispis();
                cout<<"Player X has won!";
                break;
            }

			if (i == 4)
			{
                ispis();
				cout << "Game is a stalemate!!!" << endl;
				break;
			}

        ponovo2:
			ispis();
            cout << "Enter coordinate [i] for player O : \033[?25h";
			cin >> O[0][i];
            cout << "\033[?25l";
			cout << "Enter coordinate [j] for player O : \033[?25h";
			cin >> O[1][i];
            cout << "\033[?25l";
            T = false;
            for (int j = 0; j < i; j++)
            {
                if ((O[0][i] == O[0][j] && O[1][i] == O[1][j]) || (O[0][i] == X[0][j] && O[1][i] == X[1][j]))
                {
                    T = true;
                    break;
                }
            }
			if ((O[0][i] > 3 || O[0][i] < 1 || O[1][i]>3 || O[1][i] < 1) || T || (O[0][i] == X[0][i] && O[1][i] == X[1][i]))
				goto ponovo2;

			tabla[O[0][i]-1][O[1][i]-1] = 2;
            popuni(O[0][i] - 1, O[1][i] - 1, 2);

            T = provera(2);
            if (T)
            {
                ispis();
                cout<<"Player O has won!";
                break;
            }
            
		}

        cout << "\nNew game? Y/N\033[?25h" << endl;
        char c;
        cin >> c;
        if (c == 'y' || c == 'Y')
        {
            goto pocetak;
        }
        
        system("clear");

		return 0;
	}
}

#endif // !"TicTacToe.h"
