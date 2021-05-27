#include <iostream>
#include <fstream>

#include "games/tetris/tetris.h"
#include "games/solitaire/podaci.h"
#include "games/tic-tac-toe/TicTacToe.h"

using namespace std;

void speak(string mess);
bool login(string* name);
int game_choices();

int main()
{
    string name;
    if (!login(&name))
    {
        cout << endl;
        cout << "IDENTIFICATION NOT RECOGNIZED BY SYSTEM\n--CONNECTION TERMINATED--\n\n";
        goto end;
    }
    else
    {
        string g = "GREETINGS " + name + ".";
        speak(g);
    }

    game_choices();

end:
    return 0;
}

void speak(string mess)
{
    std::ofstream f("req.txt");

    f << mess;
    f.close();
    system("cd \"voice synth\" && env1\\scripts\\python.exe \"PythonApplication1.py\"");
}

bool login(string* name)
{
    string pass;
    string x;

begin:

    cout << "\n\nLOGON:  ";
    getline(cin, pass);

    if (pass == "help games")
    {
        cout << "\n  FALKEN'S MAZE\n  BLACK JACK\n  GIN RUMMY\n  HEARTS\n  BRIDGE\n  CHEKERS\n  CHESS\n  POKER\n  FIGHTER COMBAT\n  ";
        cout << "GUERRILLA ENGAGEMENT\n  DESERT WARFARE\n  AIR-TO-GROUND ACTIONS\n  THEATERWIDE TACTICAL WARFARE\n  THEATERWIDE BIOTOXIC AND CHEMICAL WARFARE\n  \n  GLOBAL THERMONUCLEAR WAR\n";
        goto begin;
    }
    
    ifstream f("login.txt");

    getline(f, x);

    while (!f.eof())
    {
        if (pass == x)
        {
            getline(f, *name);
            f.close();
            return true;
        }
        else
        {
            getline(f, x);
        }
        getline(f, x);
    }
    f.close();
    return false;
}

int game_choices()
{
    string ans;
again:
    ans = "";
    speak("SHALL WE PLAY A GAME?");
    cout << endl;
    getline(cin, ans);
    cout <<"A"<<ans<<"A"<< endl;
    if (ans == "game list" || ans == "list")
    {
        speak("  TETRIS");
        speak("  SOLITAIRE");
        speak("  TIC-TAC-TOE");
    }
    else if (ans == "No" || ans == "no" || ans == "No, we shan't.")
    {
        speak("GOODBYE!");
        return 0;
    }
    else if (ans == "tetris")
    {
        tetris ig;
        ig.main();
    }
    else if (ans == "solitaire")
    {
        solitaire::main();
    }
    else if (ans == "tic-tac-toe"|| ans == "tic tac toe")
    {
        tictactoe::main();
    }

    goto again;
}