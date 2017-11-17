#include <iostream>
#include <string>
#include "Bot.h"

superRegionModify = {1.1, 1.3, 1.0, 1.2, 0.9, 1.4}
//Quick definition: when I say landlocked it means surrounded by allies. Not landlocked means there's at least 1
// Enemy or neutral adjacent to the region.
// Also superregion border is one of the x regions which holds the border between superregions
// Whereas border means the series of connected owned regions which often surround landlocked regions
//We also want to have a "is on superregion border" function/structure
//Also make a structure/function for internal borders, so a list of all our regions without/with enemy/neutral neighbors
//Our placement theory would be to prioritize the ones with enemy neighbors, so the "threat" of landlocked regions of ours
//would basically be zero
//Our transfer theory could be to move troops wherever we feel threat. If a neighbor of one of our landlocked regions is not 
//landlocked, move to them. More importantly we want to minimize threat, so we want to move them to the neighbor with the highest
//threat. If surrounded by landlocked regions maybe we could try to find the shortest path to a border region? Or do we assume
//we never get to that point since our placement strategy only places on borders, and we only attack neutrals/enemies?
//
//Our attacking strategy should probably be several steps: Determine who we want to attack, how much we should attack,
//and if we want to attack. We could probably implement the if as part of how much, since we'd decide to commit 0
//Probably never want to commit all in a losing scenario
//Do we want to only commit as many as it takes to win some percent of the time?
//Which aspects should be evolvable?

//Probable evolvable stuff:
//superRegionModify score, region importances, shared super-region importance value, minimum % attack win chance(calculate odds of winning sending all, all-1 ...),
//max percent of regions troops sent on attack, minimum troop count for attack

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
        int startRegionIndex[6];
        int countShared[6] = {0, 0, 0, 0, 0, 0}
        unsigned i,nbAns=0;
        unsigned j = 0;
        unsigned index = 0;
        for(i = 0; i < startingRegionsreceived.size(); i++)
        {
            //Assume that starting score values are determined outside of program and passed as a parameter to each 
            // Region when it is created
           startScore[i] = getStartPriority(startingRegionsreceived[i]);
            
        }

        for (i = 0; i < startScore.size; i++){
            for (j = 0; j < startScore.size; j++){
                index = 0;
                if (startScore[j] > startScore[index]){
                    index = j;
                }
            }
            startScore[index] = 0;
            startRegionIndex[i] = index;
        }
        
        //Need to find a way to sort the indexes (probably with an int array)
        //Then print out startingRegionsreceived[sortedIndex[i]], where i goes from 0 to 5
        for(i = 0; i < startingRegionsreived.size() && nbAns < 6; i++)
        {
            //print startingRegionsreceived[i] where i is index of startScore with greatest -> lowest value
            cout << startingRegionsreceived[startRegionIndex[i]];
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
        // Vector which keeps track of all 
        vector<double> edgeRegionThreat;
        while(armiesLeft > 0)
        {
            for(i = 0; i < edgeRegions.size(); i++)
            {
                edgeRegionThreat[i] = 0;
                for(j = 0; j < regions[edgeRegions[i]].getNeighbors().size(); j++)
                {
                    if(regions[regions[edgeRegions[i]].getNeighbors()[j]].getOwner() == opponentBotName)
                    {
                        edgeRegionThreat[i] += (regions[regions[edgeRegions[i]].getNeighbors()[j]].getArmies())/regions[edgeRegions[i]].getArmies;
                    }
                }
            }
            cout << botName << " place_armies " << ownedRegions[maxVal(edgeRegionThreat)] << " " << 1 << ",";
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
int maxVal(vector<double> vec)
{
    int i = 0;
    int result = 0;
    for(i = 1; i < vec.size(); i++)
    {
        if (vec[i] > vec[result])
            result = i;
    }
    return result;
}
void Bot::updateRegion(unsigned noRegion, string playerName, int nbArmies)
{
    regions[noRegion].setArmies(nbArmies);
    regions[noRegion].setOwner(playerName);
    if (playerName == botName)
    {

        ownedRegions.push_back(noRegion);
    }
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
