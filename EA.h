#include <stdio.h>
//We will use discrete recombination probably (if we want recombination of course)
//This means we choose the value from one of the parents randomly, so a child might be 1 2 1 1...
//and the other might be 2 2 2 1 1 2 .. which means opposite children aren't guarenteed
//For mutation lets use gaussian mutation which means we add a random value (+, 0, or -) to each value
//in the individual


struct{
    double *chrom;      //Array of doubles which stores all the stuff for now
                        //To be placed in a file and used with our bot
                        //Maybe make into seperate pieces?
    double fitness;     //Recorded fitness for individual
    int parent1;        //Index of parent 1 and 2
    int parent2;
}INDIVIDUAL

typedef INDIVIDUAL *Indv;


struct{
    Indv    oldP;       //Array of individuals from before selection process
    Indv    newP;       //Array of new individuals
    int     chromL;     //Number of things in the chromosome
    int     gen;        //Current generation
    double  sumFitness; // statistics parameters for selection and tracking
    double  max;
    double  avg;
    double  min;

    double  pCross;     // probability of Xover
    double  pMut;       // probability of Mutation 
    double  randomseed;


    double  highestEverFitness;
    int     highestEverGen;
    int     highestEverIndex;

    int     maxi;       // index of best individual in current population
    int     mini;       // index of worst individual in current population

    int     maxGen;     // when to stop
    int     popSize;    // population size

}POPULATION