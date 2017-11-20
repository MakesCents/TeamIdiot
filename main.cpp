#include <iostream>
#include <fstream>

#include "Bot.h"

using namespace std;

int main(int argc, char *argv[])
{
    cout.sync_with_stdio(0);
    Bot bot;
    bot.playGame(argv);
    return 0;
}
