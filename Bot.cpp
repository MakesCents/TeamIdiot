#include <iostream>
#include <string>
#include "Bot.h"
#include <string.h>

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
void Bot::readInFile(char *argv[])
{
    /*
    char *Ifile;
    int  nameLength = (int) strlen(argv[1]);
    Ifile = (char *) calloc(nameLength + 1, sizeof(char));
    strcpy(Ifile, argv[1]);
    int i;
    FILE *inpfl;
    char tmp[1024];
    int err;
    
    if( (inpfl = fopen(Ifile,"r")) == NULL){
      printf("error in opening file %s \n", Ifile);
      exit(1);
    }
    */
    //err = fscanf(inpfl,"%d",placethinghere);//Do this for every thing you want to read in
    
    //get the initial priorities of each of the regions as starting locations
    int i;
    for (i = 0; i < MAX_REGIONS; i++){
        i_startingPriorities.push_back(0.0);
        //err = fscanf(inpfl, "%d", i_startingPriorities[i]);
        i_startingPriorities[i] = rand() % 100 + 1;
    }

    //get the importance for each region
    for (i = 0; i < MAX_REGIONS; i++){
        i_regionImportance.push_back(0.0);
        //err = fscanf(inpfl, "%d", i_regionImportance[i]);
        i_regionImportance[i] = rand() % 100 + 1;
    } 
    
    //err = fscanf(inpfl, "%d", i_minOutcome);
    i_minOutcome = rand() % 100 + 1;

    //err = fscanf(inpf1, "%d", i_neutralMin);
    i_neutralMin = rand() % 100 + 1;

    i_importanceModifier = rand() % 100 + 1;

}
void Bot::playGame(char *argv[])
{   
    readInFile(argv);
    parser.initParser(this);
    parser.parseInput();
}    //plays a single game of Warlight

void Bot::makeMoves()
{
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
    regions[noRegion]  = Region(noRegion, noSuperRegion, i_startingPriorities[noRegion], i_regionImportance[noRegion]);
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
        int countShared[6] = {0, 0, 0, 0, 0, 0};
        unsigned i,nbAns=0;
        unsigned j = 0;
        unsigned index = 0;
        for(i = 0; i < startingRegionsreceived.size(); i++)
        {
            //Assume that starting score values are determined outside of program and passed as a parameter to each 
            // Region when it is created
           startScore[i] = regions[startingRegionsreceived[i]].getStartPriority();
            
        }

        for (i = 0; i < 6; i++){
            index = 0;
            for (j = 0; j < 6; j++){
                
                if (startScore[j] > startScore[i]){
                    index = j;
                }
            }
            startScore[index] = 0;
            startRegionIndex[i] = index;
        }
        
        //Need to find a way to sort the indexes (probably with an int array)
        //Then print out startingRegionsreceived[sortedIndex[i]], where i goes from 0 to 5
        for(i = 0; i < startingRegionsreceived.size() && nbAns < 6; i++)
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
        int j;
        
        // Vector which keeps track of all 
        vector<double> edgeRegionThreat;
        //Update at the start of the turn all of the regions which are neighbor to an enemy
        updateEdgeRegion();
        while(armiesLeft > 0)
        {
            for(i = 0; i < edgeRegions.size(); i++)
            {
                edgeRegionThreat[i] = 0;
                for(j = 0; j < regions[edgeRegions[i]].getNeighbors().size(); j++)
                {
                    if(regions[regions[edgeRegions[i]].getNeighbors()[j]].getOwner() != botName)
                    {
                        edgeRegionThreat[i] += ((regions[regions[edgeRegions[i]].getNeighbors().at(j)].getArmies())/regions[edgeRegions[i]].getArmies());
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
        int i, j, currRegion, currentArmies;
        double expectedResult;
        double modifiedResult;
        //Consider every region
        for(i = 0; i < ownedRegions.size();i++)
        {
            if(noEnemies(i) == 0 && regions[ownedRegions[i]].getArmies() > 1)
            {
                // transfer stuff
                //Assume that all of the provinces have been assingned a threat value
                //Send a commensurate amount of troops to locations which have a higher threat
                
                double total_threat_diff = 0, threat_diff = 0;

                for (j = 0; j < regions[ownedRegions[i]].getNeighbors().size(); j++){
                    if (regions[ownedRegions[i]].getThreat() < regions[regions[ownedRegions[i]].getNeighbors()[j]].getThreat()){
                        total_threat_diff += regions[regions[ownedRegions[i]].getNeighbors()[j]].getThreat() - regions[ownedRegions[i]].getThreat();
                    }
                }

                for (j = 0; j < regions[ownedRegions[i]].getNeighbors().size(); j++){
                    if (regions[ownedRegions[i]].getThreat() < regions[regions[ownedRegions[i]].getNeighbors()[j]].getThreat()){
                        threat_diff = regions[regions[ownedRegions[i]].getNeighbors()[j]].getThreat() - regions[ownedRegions[i]].getThreat();
                        cout <<botName << " attack/transfer " << ownedRegions[i] << " " << regions[ownedRegions[i]].getNeighbors()[j] << " " << regions[ownedRegions[i]].getArmies() * (threat_diff/total_threat_diff) << ",";
                    }
                }

            }
            else
            {
                // attack stuff
                //Look at all neighbors
                for (j = 0; j < regions[ownedRegions[i]].getNeighbors().size(); j++)
                {
                    currRegion = regions[ownedRegions[i]].getNeighbors()[j];
                    if(regions[currRegion].getOwner() != botName)
                    {
                        //If an enemy region, consider attacking
                        //step 1: Determine if we should attack: 
                        //Consider how many dudes the guy has
                        //Consider how many neighbors 

                        //Figure out expected result if attacking with all dudes
                        expectedResult = ((regions[ownedRegions[i]].getArmies()-1) * 0.6) - (regions[currRegion].getArmies() * 0.7);
                        //Increase it by importanceModifier, which is the absolute value of expectedResult * importanceModifier * importance of defending region as a percent
                        if(expectedResult < 0)
                        {
                            modifiedResult = (expectedResult * -1 * i_importanceModifier * regions[currRegion].getImportance()/100) + expectedResult;
                        }
                        else
                        {
                            modifiedResult = (expectedResult * i_importanceModifier * regions[currRegion].getImportance()/100) + expectedResult;
                        }
                        if(modifiedResult >= i_minOutcome)
                        {
                            //find min number of troops unless this is the only neighbor and expected Result > 0
                            if((expectedResult > 0) && noEnemies(ownedRegions[i]) == 1)
                            {
                                cout << botName << " attack/transfer " << ownedRegions[i] << " " << currRegion << " " << regions[ownedRegions[i]].getArmies() - 1 << ",";
                                regions[ownedRegions[i]].setArmies(1);
                            }
                            else
                            {
                                int currentArmies = regions[ownedRegions[i]].getArmies()-1;
                                //Not a for loop because easier to try it lowered one then set it. Obviously for loop could work but nah
                                for(currentArmies = regions[ownedRegions[i]].getArmies()-1; currentArmies > 1; currentArmies--)
                                {
                                    //This chunk just for looking ahead, not current place
                                    expectedResult = (currentArmies * 0.6) - (regions[currRegion].getArmies() * 0.7);
                                    if(expectedResult < 0)
                                    {
                                        modifiedResult = (expectedResult * -1 * i_importanceModifier * regions[currRegion].getImportance()/100) + expectedResult;
                                    }
                                    else
                                    {
                                        modifiedResult = (expectedResult * i_importanceModifier * regions[currRegion].getImportance()/100) + expectedResult;
                                    }
                                    //check if next step violates thing
                                    if(modifiedResult < i_minOutcome)
                                    {
                                        break;
                                    }
                                }
                                //Attack with minimum needed
                                cout << botName << " attack/transfer " << ownedRegions[i] << " " << currRegion << " " << currentArmies << ",";
                                regions[ownedRegions[i]].setArmies(regions[ownedRegions[i]].getArmies() - currentArmies);
                            }
                        }
                    }
                    else if(regions[currRegion].getOwner() == botName)
                    {
                    // If an allied region, maybe do something?
                    
                    }
                    else
                    {
                        // If neutral expand if it doesn't neighbor any enemies
                        if(noEnemies(currRegion) == 0 && regions[ownedRegions[i]].getArmies() > 4)
                        {
                            cout << botName << " attack/transfer " << ownedRegions[i] << " " << currRegion << " " << 4 << ",";
                            regions[ownedRegions[i]].setArmies(regions[ownedRegions[i]].getArmies() - 4);
                        }//If important enough, then 
                        else if(regions[ownedRegions[i]].getImportance() > i_neutralMin)
                        {
                            for(currentArmies = regions[ownedRegions[i]].getArmies()-1; currentArmies > 1; currentArmies--)
                            {
                                //This chunk just for looking ahead, not current place
                                expectedResult = (currentArmies * 0.6) - (regions[currRegion].getArmies() * 0.7);
                                if(expectedResult < 0)
                                {
                                    modifiedResult = (expectedResult * -1 * i_importanceModifier * regions[currRegion].getImportance()/100) + expectedResult;
                                }
                                else
                                {
                                    modifiedResult = (expectedResult * i_importanceModifier * regions[currRegion].getImportance()/100) + expectedResult;
                                }
                                //check if next step violates thing
                                if(modifiedResult < i_minOutcome)
                                {
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    phase.clear();
}
int Bot::maxVal(vector<double> vec)
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

int Bot::noEnemies(int reg)
{
    int j, result;
    for(j = 0; j < regions[reg].getNeighbors().size();j++)
    {
        if(regions[regions[reg].getNeighbors()[j]].getOwner() != botName)
            result++;
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

void  Bot::updateEdgeRegion(void){
    int i, j;
    edgeRegions.clear();
    for (i = 0; i < ownedRegions.size(); i++){
        for (j = 0; j < regions[ownedRegions[i]].getNeighbors().size(); j++){
            if (regions[regions[ownedRegions[i]].getNeighbors()[j]].getOwner() != botName){
                edgeRegions.push_back(ownedRegions[i]);
            }
        }
    }
}
