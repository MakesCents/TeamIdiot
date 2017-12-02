/*This is where you find the random functions and report functions */

#include "EA.h"
#define RANDCONST 1.0E-9
double oldrand[56];
int currRand;
void rawStat(void);
void incrementRandom(double seed);
double f_random(void);
void storeChromosome(POPULATION *p, Indv ind, int index);

void randomize(POPULATION *p)
{
    int err;
    err = printf("Enter seed between 0.0 and 1.0: ");
    err = scanf("%lf", &p->randomseed);
    while( p->randomseed < 0.0 || p->randomseed > 1.0)
    {
        err = printf("Enter seed between 0.0 and 1.0: ");
        err = scanf("%lf", &p->randomseed);
    }
    incrementRandom(p->randomseed);
}
void newRandom()
{
    int j, i;
    double next, prev;
    for(j = 1; j <= 24; j++)
    {
        next = oldrand[j] - oldrand[j+31];
        if (next < 0.0)
        {
            next = next + 1;
        }
        oldrand[j] = next;
    }
    for(j = 25; j<=55; j++)
    {
        next = oldrand[j] - oldrand[j-24];
        if(next < 0.0)
        {
            next = next + 1;
        }
        oldrand[j] = next;
    }
}

void incrementRandom(double seed)
{
    int j, i;
    double next, prev;
    oldrand[55] = seed;
    next = RANDCONST;
    prev = seed;
    for(j = 1; j < 54; j++)
    {
        i = (21 * j) % 55;
        oldrand[i] = next;
        next = prev - next;
        if(next < 0)
        {
            next += 1.0;
        }
        prev = oldrand[i];
    }
    newRandom();
    newRandom();
    newRandom();
    currRand = 0;
}

int coinFlip(double chance)
{
    if(chance == 1.0) return 1;
    else return (f_random() <= chance);
}

double generateRandom(int low, int high)
{
    double temp;
    int i;
    temp = f_random();
    if(low >= high) {
        i = low;
    }
    else 
    {
        i = (temp * (double)(high - low + 1) + low);
        if (i > high) i = high;
    }
    return i;
}

double f_random()
{
    currRand++;
    if(currRand > 55)
    {
        currRand = 1;
        newRandom();
    }
    return oldrand[currRand];
}
/*************** Random ends here *****************/
void report(int gen, POPULATION *p, Indv pop)
{
    FILE *fp;
    int err;
    if( (fp = fopen(p->ofile, "a")) == NULL)
    {
        err = printf("Can't open this file %s\n", p->ofile);
        exit(1);
    }
    else{
        rawStat(fp, p, pop);
        fclose(fp);
    }
    rawStat(stdout, p, pop);
}

void rawStat(FILE *fp, POPULATION *p, Indv pop)
{
    printf("HI!");
    int err;
    err = fprintf(fp, " %3d, %10.2lf %10.2lf %10.2lf ", p->gen, p->max, p->avg, p->min);
    err = fprintf(fp, " %3d, %10.2lf %3d ", p->highestEverGen, p->highestEverFitness, p->highestEverIndex, p->min);
    err = fprintf(fp, " %10.2lf\n", pop[p->maxi].fitness);
}

void statistics(POPULATION *p, Indv pop)
{
    int size, i, j, s;
    Indv pi;
    p->sumFitness = pop[0].fitness;
    p->max = p->sumFitness;
    p->min = p->sumFitness;
    p->maxi = 0;
    p->mini = 0;
    for(i = 1; i < p->popSize; i++)
    {
        pi = &(pop[i]);
        p->sumFitness += pi->fitness;
        if(p->max < pi->fitness)
        {
            p->max = pi->fitness;
            p->maxi = i;
        }
        if(p->min > pi->fitness )
        {
            p->min = pi->fitness;
            p->mini = i;
        }
    }
    p->avg = p->sumFitness / (double)p->popSize;
    if(p->highestEverFitness < p->max)
    {
        p->highestEverFitness = p->max;
        p->highestEverGen = p->gen;
        p->highestEverIndex = p->maxi;
    }
    storeChromosome(p, pop, p->maxi);
}

void storeChromosome(POPULATION *p, Indv ind, int index)
{
    std::ofstream myfile;
    myfile.open("BestIndividual");
    int i;
    for(i = 0; i < p->chromL; i++)
    {
        myfile << ind->chrom[i] << "\n";
    }
}

