#include "Region.h"
#include <stdio.h>
#include <iostream>
#include "SuperRegion.h"
using namespace std;
Region::Region()
{
    //ctor
}

Region::Region(int pId, int pSuperRegion, double startPriority, double importance)
{
    this->superRegion = pSuperRegion;
    id = pId;
    this->startPriority = startPriority;
    this->importance = importance;
}

Region::~Region()
{
    //dtor
}
void Region::addNeighbors(int Neighbors)
{
    neighbors.push_back(Neighbors);
}

void Region::setArmies(int nbArmies)
{
    armies =nbArmies;
}
void Region::setOwner(string pOwner)
{
    owner = pOwner;
}
int Region::getArmies()
{
    return armies;
}
string Region::getOwner()
{
    return owner;
}

int Region::getSuperRegion()
{
    return superRegion;
}

vector<int>& Region::getNeighbors()
{
    return neighbors;
}

double Region::getStartPriority(){
    return startPriority;
}
void Region::setStartPriority(double priority){
    startPriority = startPriority;
}

double Region::getThreat(){
    return roundThreat;
}

void Region::setThreat(double threat){
    roundThreat = threat;
}

void Region::setImportance(double Importance)
{
    importance = Importance;
}
double Region::getImportance()
{
    return importance;
}