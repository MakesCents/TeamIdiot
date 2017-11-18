#include <iostream>
#include <fstream>

#include "Bot.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout.sync_with_stdio(0);
    
    ifstream myFile ("file.txt");
    if (myFile.is_open())
    {
        while (getline (myFile, line))
        {
            cout << line << '\n';
        }
    }
    else cout << "Unable to open file";

    Bot bot;
    bot.playGame(argv);

    ofstream myFile("file.txt");
    if(myFile.is_open())
    {
        myFile << "Write here\n";
        myFile.close();
    }
    else cout << "Unable to open file";

    return 0;
}
