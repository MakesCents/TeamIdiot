#include <iostream>
#include <string>
#include "Bot.h"

superRegionModify = {1.1, 1.3, 1.0, 1.2, 0.9, 1.4}

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
    randCount = 0;
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
        //Step 1. Give all 6 regions a rank based on which region they are 
        //(start with 1 before evolution, to make certain regions inside a super-region more desirable)
        //Step 2. Increase the value depending on which super-region they're in
        //(Make higher rewarding continents less valuable and lower rewarding continents more valuable since
        // they can be defended easier)
        //Step 3. Increase the value depending on how many shared super-regions you have
        //(Having 2 in NA is more important than having 1 in Au and 1 in SA possibly)
        double startScore[6];
        int countShared[6] = {0, 0, 0, 0, 0, 0}
        unsigned i,nbAns=0;
        unsigned j = 0;
        for(i = 0; i < startingRegionsreceived.size(); i++)
        {
            for(j = 0; j < startingRegionsreceived.size(); j++)
            {
                if(getSuperRegion(startingRegionsreceived[j]) == getSuperRegion(startingRegionsreceived[i]))
                {
                    countShared[i]++;
                }
            }
            //startScore = regionScore * superRegionScore * sharedScore (or add?)
            startScore[i] = 1 * startScore[startingRegionsreceived[i]-1] * (1 + countShared[i] * 0.1);
            //We want to prevent them from getting shared regions anyways, but we can 
            //make the sharedScore a changeable parameter
        }
        
        //Need to find a way to sort the indexes (probably with an int array)
        //Then print out startingRegionsreceived[sortedIndex[i]], where i goes from 0 to 5
        for(i = 0; i < startingRegionsreived.size() && nbAns < 6; i++)
        {
            //print startingRegionsreceived[i] where i is index of startScore with greatest -> lowest value
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
        //Want to consider value of the regions along with how much threat they're under
        //Regions surrounded by allies are under no threat, so don't worry about them
        //Regions next to neutral regions aren't under much threat, but more than an ally
        //Enemy regions produce threat based on how many troops are in it along with how
        //many troops in their neighboring allied regions
        //We're doing this simply, so don't go past one layer (ie stop looking once you find enemy "islands")
        //Consider number of troops, value of region, threat it feels
        int i = 0;
        
        while(armiesLeft > 0)
        {
            cout << botName << " place_armies " << ownedRegions[i] << " " << 1 << ",";
            i= (i+1) % ownedRegions.size();
            setArmiesLeft(--armiesLeft);
        }
        cout << "\n";
    }
    //Firstly if attacking a neutral, wait for 4 if possible
    //The value of a neutral is less than the value of an enemy
    //When considering if we should attack: consider how much threat the region produces combined with the
    //Likelihood of success, threat depends on our internal importance metric combined with number of troops it
    //holds along with reduced strength threat generated solely from it's adjacent neighbors owned by the enemy
    //Expanding should be done if it doesn't hurt our position so we have 4+ in the expander
    

    //For attacking we should 
    if (phase == "attack/transfer")
    {
        std::srand(randomes[randCount++]);
        int i = 0;
        int temp = 0;
        for(i = 0; i < ownedRegions.size(); i++)
        {
            if(regions[ownedRegions[i]].getArmies() > 3)
            {
                temp = std::rand() % regions[ownedRegions[i]].getNeighbors().size();
                cout << botName << " attack/transfer " << ownedRegions[i] << " " << regions[ownedRegions[i]].getNeighbors()[temp] << " " << 4 << ",";
                regions[ownedRegions[i]].setArmies(regions[ownedRegions[i]].getArmies()-4;
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
