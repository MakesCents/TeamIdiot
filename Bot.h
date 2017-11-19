
#ifndef BOT_H
#define BOT_H
#include <iostream>
#include <stdio.h>

#include <fstream>
#include <iostream>
#include <vector>
#include "Parser.h"
#include "Region.h"
#include "SuperRegion.h"
using namespace std;
class Bot
{
    ifstream in;
    vector<Region> regions;
    vector<SuperRegion> superRegions;
    string botName;
    string opponentBotName;
    vector<int> startingRegionsreceived;
    vector<int> ownedRegions;
    vector<int> edgeRegions;
    int armiesLeft;
    Parser parser;
    string phase;
    double superRegionModify[6];
    public:
        Bot();
        virtual ~Bot();

    void playGame(char *argv[]);    //plays a single game of Warlight
    int maxVal(vector<double> vec);   //
    void makeMoves();   //makes moves for a single turn
    void endTurn();     //indicates to the engine that it has made its moves
    void addRegion(unsigned noRegion, unsigned noSuperRegion);
    void addSuperRegion(unsigned noSuperRegion, int reward);
    void addNeighbors(unsigned noRegion, unsigned Neighbors);
    void setBotName(string name);
    void setOpponentBotName(string name);
    void setArmiesLeft(int nbArmies);
    void addArmies(unsigned noRegion,int nbArmies);
    void moveArmies(unsigned noRegion,unsigned toRegion,int nbArmies);
    void addStartingRegion(unsigned noRegion);
    void startDelay(int delay);
    void setPhase(string pPhase);
    void executeAction();
    void updateRegion(unsigned noRegion, string playerName, int nbArmies);
    void resetRegionsOwned();
    bool noEnemies(int reg);
<<<<<<< HEAD
    void readInFile(char *argv[]);
=======
    void updateEdgeRegions(void);
>>>>>>> a6c4618be286f42e1536e0a22a956d94c87b6ce3

    private:
};

#endif // BOT_H
