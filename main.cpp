#include <iostream>
#include <fstream>

#include "Bot.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout.sync_with_stdio(0);
    ifstream myFile;

    myFile.open("PATH\\file.txt");
    if(!myFile)
    {
        cout << "Unable to open file";
        exit(1);
    }

    Bot bot;
    bot.playGame();

    ofstream myFile("file.txt");
    if(myFile.is_open())
    {
        myFile << "Write here\n";
        myFile.close();
    }
    else cout << "Unable to open file";

    return 0;
}
