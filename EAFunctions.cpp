#include "EA.h"

double f_random();
int roulette(Indv pop, double sumFitness, int popsize);
void crossover(POPULATION *p, Indv p1, Indv p2, Indv c1, Indv c2);
fitStruct eval(POPULATION *p, Indv p1, Indv p2);
double Mutate(POPULATION *p,double child, double parent, double sigma);
int coinFlip(double chance);
double generateRandom(int low, int high);
double limit(double child, double min, double max);
double gaussianMutation(double gene, double sigma);

fitStruct eval(POPULATION *p, Indv p1, Indv p2)
{
    //First step: dump all chromosome into file
    std::ofstream myfile;
    std::ofstream myfile2;
    myfile.open("infile1");
    myfile2.open("infile2");
    int i;
    for(i = 0; i < p->chromL; i++)
    {
        myfile << p1->chrom[i] << "\n";
        myfile2 << p2->chrom[i] << "\n";
    }
    system("cd conquest-engine-master;cd bin;java main.RunGame 0 0 0 \"gcc ../../main.cpp < infile1\" \"gcc ../../main.cpp < infile2\" 2>\"../../err.txt\" 1> \"../../out.txt\"");
    myfile.close();
    myfile2.close();
    std::ifstream inFile ("out.txt");
    bool asteriskSeen = false;
    bool tracker = false;
    int regionsHeld1[20];
    int regionsHeld2[20];
    int winner;
    int turns;
    double fit1;
    double fit2;
    i = -1;
    std::string line;
    if(inFile.is_open())
    {
        while( getline (inFile, line) )
        {
            const char *c = line.c_str();
            if(i == -1)
            {
                i = -2;
                continue;
            }
            else if(i == -2)
            {
                i = 0;
                continue;
            }
            if(line[0] == '*')
            {
                asteriskSeen = true;
                i = 0;
            }
            if(tracker && !asteriskSeen)
            {
                sscanf(c, "%i", &(regionsHeld1[i]));
                tracker = false;
            }
            else if(tracker && !asteriskSeen)
            {
                sscanf(c, "%i", &(regionsHeld2[i]));
                i++;
                tracker = true;
            }
            else if(asteriskSeen && i == 0)
            {
                i = 1;
                sscanf(c, "%i", &winner);
            }
            else
            {
                sscanf(c, "%i", &turns);
            }
            if(turns == 100)
            {
                for(i = 0; i < 20; i++)
                {
                    fit1 = regionsHeld1[i] * (1 + (20 - i)/100);
                    fit2 = regionsHeld2[i] * (1 + (20 - i)/100);
                }
            }
            else if(winner == 1){
                int tempTurns = (turns - (turns % 5)) / 5;
                for(i = 0; i < tempTurns; i++)
                {
                    fit1 = regionsHeld1[i] * (1 + (20 - i)/100);
                    fit2 = regionsHeld2[i] * (1 + (20 - i)/100);
                }
                fit1 *= 1.4 * (100-turns);
            }
            else
            {
                int tempTurns = (turns - (turns % 5)) / 5;
                for(i = 0; i < tempTurns; i++)
                {
                    fit1 = regionsHeld1[i] * (1 + (20 - i)/100);
                    fit2 = regionsHeld2[i] * (1 + (20 - i)/100);
                }
                fit2 *= 1.4 * (100-turns);
            }
        }
        inFile.close();
    }
    else
    {
        printf("couldn't open the file :(\n");
    }
    fitStruct result;
    result.fitness1 = fit1;
    result.fitness2 = fit2;
    return result;

}

int roulette(Indv pop, double sumFitness, int popsize)
{
    double rand = f_random() * sumFitness;
    int partsum = 0;
    int i = -1;
    do{
        i++;
        partsum += pop[i].fitness;
    }while(partsum < rand && i < popsize - 1);
    return i;
}

void generation(POPULATION *p, int t)
{
    int i, p1, p2, c;
    Indv pi, piPlus1, om1, om2;
    fitStruct fit;
    for(i = 0; i < p->popSize; i += 2)
    {
        p1 = roulette(p->oldP, p->sumFitness, p->popSize);
        p2 = roulette(p->oldP, p->sumFitness, p->popSize);
        pi = &(p->newP[i]);
        piPlus1 = &(p->newP[i+1]);
        om1 = &(p->oldP[p1]);
        om2 = &(p->oldP[p2]);

        crossover(p, om1, om2, pi, piPlus1);
        fit = eval(p, pi, piPlus1);
        pi->parent1 = p1;
        pi->parent2 = p2;
        pi->fitness = fit.fitness1;
        piPlus1->fitness = fit.fitness2;
        piPlus1->parent1 = p1;
        piPlus1->parent2 = p2;
    }
}

void crossover(POPULATION *p, Indv p1, Indv p2, Indv c1, Indv c2)
{
    double *pi1, *pi2, *ci1, *ci2;
    int xp, i;
    pi1 = p1->chrom;
    pi2 = p2->chrom;
    ci1 = c1->chrom;
    ci2 = c2->chrom;
    double sigma = (100-0) / 6;
    if(coinFlip(p->pCross))
    {
        for(i = 0; i < p->chromL; i++)
        {
            if(coinFlip(0.5))
                ci1[i] = Mutate(p,ci1[i], pi1[i], sigma);
            else
                ci1[i] = Mutate(p,ci1[i], pi2[i], sigma);
            
            if(coinFlip(0.5))
                ci2[i] = Mutate(p,ci2[i], pi1[i], sigma);
            else
                ci2[i] = Mutate(p,ci2[i], pi2[i], sigma);
        }
    }
    else
    {
        for(i = 0; i < p->chromL; i++)
        {
            ci1[i] = Mutate(p,ci1[i], pi1[i], sigma);
            ci2[i] = Mutate(p, ci2[i], pi2[i], sigma);
        }
    }
}

double Mutate(POPULATION *p, double child, double parent, double sigma)
{
    if(coinFlip(p->pMut))
    {
        double temp = sigma * exp(gaussianMutation(0, 1));
        if(0 > temp)
            sigma = 0;
        else
            sigma = temp;
        
        child = gaussianMutation(parent, sigma);
        child = limit(child, 0, 100);
        return child;
    }
    else
    {
        // If flip says don't mutate then just return child
        return parent;
    }
}

double gaussianMutation(double gene, double sigma)
{
    double i = f_random();
    double j = f_random();
    if(i == 0)
        i = 1;
    if(j == 0)
        j = 1;
    double y = sqrt(-2.0 * log(i)) * cos(2.0 * PI * j);
    return y * sigma + gene;
}

double limit(double child, double min, double max)
{
    if (child >= max)
    {
        return max;
    }
    if(child <= min)
    {
        return min;
    }
    return child;
}

