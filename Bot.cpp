#include <iostream>
#include <string>
#include <ctime>
#include "Bot.h"



Bot::Bot()
{
   // printf("ctor\n");
}

Bot::~Bot()
{
     //printf("dtor\n");
    //dtor
}

void Bot::playGame()
{
    std::srand(std::time(0)); // use current time as seed for random generator
    for (int i = 0; i < 100; i++)
    {
        randomes[i] = std::rand();
    }
    parser.initParser(this);
    parser.parseInput();
}    //plays a single game of Warlight

void Bot::makeMoves()
{
    cout << "Hi!!\n";
}   //makes moves for a single turn
void Bot::endTurn()
{

}     //indicates to the engine that it has made its move

void Bot::addRegion(unsigned noRegion, unsigned noSuperRegion)
{
    while (  regions.size() <= noRegion)
        {
            regions.push_back(Region());
        }
    regions[noRegion]  = Region(noRegion, noSuperRegion);
    superRegions[noSuperRegion].addRegion(noRegion);
}
void Bot::addNeighbors(unsigned noRegion, unsigned neighbors)
{
    regions[noRegion].addNeighbors(neighbors) ;
    regions[neighbors].addNeighbors(noRegion) ;
}

void Bot::addSuperRegion(unsigned noSuperRegion, int reward)
{
    while (  superRegions.size() <=  noSuperRegion)
        {
            superRegions.push_back(SuperRegion());
        }
    superRegions[noSuperRegion]  = SuperRegion(reward);
}

void Bot::setBotName(string name)
{
    botName = name;
}
void Bot::setOpponentBotName(string name)
{
    opponentBotName = name;
}

void Bot::setArmiesLeft(int nbArmies)
{
    armiesLeft = nbArmies;
}

void Bot::addStartingRegion(unsigned noRegion)
{
     startingRegionsreceived.push_back(noRegion);
}

void Bot::startDelay(int delay)
{

}
void Bot::setPhase(string pPhase)
{
    phase=pPhase;
}
void Bot::executeAction()
{
    if (phase=="")
        return;
    if (phase == "pickPreferredRegion")
    {
        unsigned i,nbAns=0;
        for (i = 0; i< startingRegionsreceived.size() && nbAns<6; i++)
        {
            cout << startingRegionsreceived[i];
            nbAns++;
            if (nbAns < 6)
                cout << " ";
            else
            {
                cout << "\n";
                break;
            }
        }
    }
    if (phase == "place_armies")
    {
        int i = 0;
        
        while(armiesLeft > 0)
        {
            cout << botName << " place_armies " << ownedRegions[i] << " " << 1 << ",";
            i= (i+1) % ownedRegions.size();
            setArmiesLeft(--armiesLeft);
        }
        cout << "\n";
    }
    if (phase == "attack/transfer")
    {
        std::srand(randomes[randCount++]);
        
        int i = 0;
        int temp = 0;
        for(i = 0; i < ownedRegions.size(); i++)
        {
            if(regions[ownedRegions[i]].getArmies() > 2)
            {
                temp = std::rand() %  regions[ownedRegions[i]].getNeighbors().size();
                cout << botName << " attack/transfer " << ownedRegions[i] << " " << regions[ownedRegions[i]].getNeighbors()[temp] << " " << 3 << ",";
                regions[ownedRegions[i]].setArmies(regions[ownedRegions[i]].getArmies() - 3);
                
            }
        }
        cout << "\n";
    }
    phase.clear();
}

void Bot::updateRegion(unsigned noRegion, string playerName, int nbArmies)
{
    regions[noRegion].setArmies(nbArmies);
    regions[noRegion].setOwner(playerName);
    if (playerName == botName)
        ownedRegions.push_back(noRegion);
}
void Bot::addArmies(unsigned noRegion,int nbArmies)
{
    regions[noRegion].setArmies(regions[noRegion].getArmies() + nbArmies);
}
void Bot::moveArmies(unsigned noRegion,unsigned toRegion,int nbArmies)
{
    if (regions[noRegion].getOwner() == regions[toRegion].getOwner()
        && regions[noRegion].getArmies() > nbArmies)
    {
        regions[noRegion].setArmies(regions[noRegion].getArmies() - nbArmies);
        regions[toRegion].setArmies(regions[toRegion].getArmies() + nbArmies);

    }
    else if (regions[noRegion].getArmies() > nbArmies)
    {
        regions[noRegion].setArmies(regions[noRegion].getArmies() - nbArmies);
    }
}

void Bot::resetRegionsOwned()
{
    ownedRegions.clear();
}
