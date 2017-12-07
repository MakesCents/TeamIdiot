/* This is where you initialize everything and call stuff */
#include "EA.h"

int main()
{
    Indv tmp, oldP, newP;
    POPULATION population;
    POPULATION *p = &population;
    p->gen = 0;
    initialize(p);
    while(p->gen < p->maxGen)
    {
        p->gen++;
        generation(p, p->gen);
        statistics(p, p->newP);
        report(p->gen, p, p->newP);
        tmp = p->oldP;
        p->oldP = p->newP;
        p->newP = tmp;
    }

    return 0;
}

void initialize(POPULATION *p)
{
    char oF[] = "oFile";
    dataInit(p, 50, 100, 50, 50, oF);
    printf("After data\n");
    populationInit(p);
    printf("After pop\n");
    reportInit(p);
    printf("After report\n");
}

void dataInit(POPULATION *p, int popS, int mGen, int pCro, int pMu, char* of)
{
    p->popSize = popS;
    if(p->popSize % 2 != 0)
    {
        p->popSize++;
    }
    p->chromL = 87;
    p->maxGen = mGen;
    p->pCross = pCro;
    p->pMut = pMu;
    p->ofile = (char*) calloc((int) strlen(of)+1, sizeof(char));
    strcpy(p->ofile, of);

    printf("\n");
    randomize(p);
    p->highestEverFitness = 0.0;
    p->highestEverGen = 0;
    p->highestEverIndex = 0;
}
void populationInit(POPULATION *p)
{
    Indv pi1, pj1;
    Indv pi2, pj2;
    int i, j;
    FILE *fp;
    double f1;
    fitStruct fit;
    p->oldP = (Indv) calloc(p->popSize, sizeof(INDIVIDUAL));
    p->newP = (Indv) calloc(p->popSize, sizeof(INDIVIDUAL));
    for(i = 0; i < (p->popSize-1); i+=2)
    {
        pi1 = &(p->oldP[i]);
        pi1->chrom = (double *) calloc (p->chromL, sizeof(double));
        pi2 = &(p->oldP[i+1]);
        pi2->chrom = (double *) calloc (p->chromL, sizeof(double));
        pj1 = &(p->newP[i]);
        pj1->chrom = (double *) calloc (p->chromL, sizeof(double));
        pj2 = &(p->newP[i+1]);
        pj2->chrom = (double *) calloc (p->chromL, sizeof(double));
        for(j = 0; j < p->chromL; j++)
        {
            pi1->chrom[j] = generateRandom(0, 100);
            pi2->chrom[j] = generateRandom(0, 100);
        }
        fit = eval(p, pi1, pi2);
        pi1->fitness = fit.fitness1;
        pi2->fitness = fit.fitness2;
        pi1->parent1 = -1;
        pi1->parent2 = -1;
        pi2->parent1 = -1;
        pi2->parent2 = -1;
        statistics(p, p->oldP);
    }
}

void reportInit(POPULATION *p)
{
    FILE *fp;
    
    printf("Population size %d\n", p->popSize);
    printf("Chromosome length used %d\n", p->chromL);
    printf("Max num of generations %d\n", p->maxGen);
    printf("Crossover chance %lf\n", p->pCross);
    printf("Mutation chance %lf\n", p->pMut);
    printf("Generation 1 stats\n");
    printf("Max fitness %f\n", p->max);
    printf("Average fitness %f\n", p->avg);
    printf("Min fitness %f\n", p->min);

    if((fp = fopen(p->ofile, "a")) == NULL)
    {
        printf("Can't open the file %s \n", p->ofile);
        exit(1);
    }
    else
    {
        fprintf(fp, "gen, max, avg, min, highestGen, highestFitness, highestIndex, min, maxIfitness");
        rawStat(fp, p, p->oldP);
        fclose(fp);
    }
    rawStat(stdout, p, p->oldP);
}