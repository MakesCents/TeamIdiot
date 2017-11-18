#include <iostream>
#include <fstream>

#include "Bot.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout.sync_with_stdio(0);
    ifstream inFile;

    inFile.open("file.txt");
    if(!inFile)
    {
        cout << "Unable to open file";
        exit(1);
    }

    Bot bot;
    bot.playGame();
    return 0;
}
