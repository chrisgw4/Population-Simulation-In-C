#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <time.h>


typedef struct Habitat_s{

    int dailyProduce;

} Habitat_t;

typedef struct Creature_s {

    int daysAlive; // The number of iterations the creature has been alive
    int lifetime; // The creature can survive n iterations
    int daysSinceLastEaten; // The creature can survive n iterations without eating
    int needToEatEvery; // The amount of iterations the creature needs to eat every
    int willDie; // keeps track if the creature needs to die
    char gene[3];
    struct Creature_s * nextCreature;

} Creature_t;

typedef struct {

    int iteration;
    int numCreatures;
    Creature_t * startCreature; // the head of the creature linked list
    Habitat_t habitat;

} Population_t;



void incrementDaysAlive(Creature_t * creature);
Creature_t * update(Population_t * pop);
int checkReproduce(Creature_t * creature);
int checkDeath(Creature_t * creature, int * numCreatures);
Creature_t * createNewCreature(char gene[]);


int main(void)
{
    srand(time(NULL));
    int iterations = 1000;

    Population_t pop;
    pop.iteration = 0;
    pop.numCreatures = 400; 
    pop.startCreature = createNewCreature("Aa");
    pop.habitat.dailyProduce = 10;

    Creature_t * temp = pop.startCreature;

    for (int x = 0; x < pop.numCreatures; ++x)
    {
        Creature_t * newCreature = createNewCreature("Aa");
        temp->nextCreature = newCreature;
        temp = newCreature;
    }

    FILE * popFile = fopen("population_stats.txt", "w");
    FILE * geneAAFile = fopen("gene1AA_stats.txt", "w");
    FILE * geneAaFile = fopen("gene2Aa_stats.txt", "w");
    FILE * geneaaFile = fopen("gene3aa_stats.txt", "w");
    
    
    while (pop.iteration <= iterations)
    {
        int num = 0;
        int numGenes[3];
        numGenes[0] = 0;
        numGenes[1] = 0;
        numGenes[2] = 0;
        
        double percentDone = (double)(pop.iteration)/iterations;

        Creature_t * temp = pop.startCreature;

        while (temp->nextCreature != NULL)
        {
            if (strcmp(temp->gene, "AA") == 0)
            {
                numGenes[0]++;
            }
            else if (strcmp(temp->gene, "Aa") == 0)
            {
                numGenes[1]++;
            }
            else if (strcmp(temp->gene, "aa") == 0)
            {
                numGenes[2]++;
            }
            
            num++;
            temp = temp->nextCreature;
        }

        if (iterations >= 10000 && (pop.iteration % (int)(iterations*0.001) == 0 || pop.iteration == iterations))
        {
            fprintf(popFile, "%d %d ", pop.iteration, num);
            fprintf(geneAAFile, "%d %d ", pop.iteration, numGenes[0]);
            fprintf(geneAaFile, "%d %d ", pop.iteration, numGenes[1]);
            fprintf(geneaaFile, "%d %d ", pop.iteration, numGenes[2]);
        }
        else if (iterations < 10000)
        {
            fprintf(popFile, "%d %d ", pop.iteration, num);
            fprintf(geneAAFile, "%d %d ", pop.iteration, numGenes[0]);
            fprintf(geneAaFile, "%d %d ", pop.iteration, numGenes[1]);
            fprintf(geneaaFile, "%d %d ", pop.iteration, numGenes[2]);
        }
        pop.numCreatures = num;
        printf("Num Creatures: %d %.1lf%% Done\n", num, percentDone*100);

        pop.startCreature = update(&pop);        

        pop.iteration++;
    }

    // free the malloced memory
    Creature_t * temp2;
    if (pop.startCreature != NULL && pop.startCreature->nextCreature != NULL)
    {
        temp2 = pop.startCreature->nextCreature;
    }

    while (temp2->nextCreature != NULL)
    {
        Creature_t * temp3 = temp2->nextCreature;
        free(temp2);
        temp2 = temp3;
    }
    free(temp2);

    if (pop.startCreature != NULL)
        free(pop.startCreature);

    fclose(popFile);
    fclose(geneAAFile);
    fclose(geneAaFile);
    fclose(geneaaFile);

    return 0;
}


Creature_t * update(Population_t * pop)
{
    Creature_t * creatureStart = pop->startCreature;
    Creature_t * creatureTraverse = creatureStart;
    int addNumCreatures = 0;
    Creature_t * bornCreatures = NULL;
    Creature_t * bornCreaturesTraverse = bornCreatures;

    Creature_t * lastReproducingCreature = NULL; // keep track of the last creature trying to reproduce

    int foodEaten = 0;
    
    while (creatureTraverse->nextCreature != NULL)
    {
        int bred = 0;

        // check if the creature can eat
        if (foodEaten < pop->habitat.dailyProduce*250)
        {
            foodEaten++; // increment the amount of food eaten that iteration
            creatureTraverse->daysSinceLastEaten = 0; // reset the days since last eaten
        }
        else
        {
            creatureTraverse->daysSinceLastEaten++; // if does not eat, increment the daysSinceLastEaten counter
        }
        
        // if the creature hasnt eaten past its need to eat deadline
        if (creatureTraverse->daysSinceLastEaten > creatureTraverse->needToEatEvery)
        {
            creatureTraverse->willDie = 1; // let it die
        }

        
        incrementDaysAlive(creatureTraverse);


        int reproduceResult = checkReproduce(creatureTraverse);
        
        // check if the creature will reproduce
        if (reproduceResult && !creatureTraverse->willDie && lastReproducingCreature != NULL)
        {
            if (bornCreatures == NULL)
            {
                char newGene[3] = "";

                int gene1 = rand() % 2; // get random val between 0 and 1
                int gene2 = rand() % 2; // get random val between 0 and 1

                // combine genes randomly
                newGene[0] = lastReproducingCreature->gene[gene1];
                newGene[1] = creatureTraverse->gene[gene2];
                // printf("%s\n", newGene);

                bornCreatures = createNewCreature(newGene);

                bornCreaturesTraverse = bornCreatures;
            }
            else if (bornCreaturesTraverse->nextCreature == NULL)
            {
                // declare new char[] to hold new genome
                char newGene[3] = "";

                int gene1 = rand() % 2; // get random val between 0 and 1
                int gene2 = rand() % 2; // get random val between 0 and 1

                // combine genes randomly
                newGene[0] = lastReproducingCreature->gene[gene1];
                newGene[1] = creatureTraverse->gene[gene2];

                bornCreaturesTraverse->nextCreature = createNewCreature(newGene);

                bornCreaturesTraverse = bornCreaturesTraverse->nextCreature;
            }
            bred = 1;
            lastReproducingCreature = NULL;
            pop->numCreatures++;
            addNumCreatures++;
        }
        
        if (creatureTraverse->willDie || checkDeath(creatureTraverse, &pop->numCreatures))
        {
            Creature_t * temp1 = creatureTraverse->nextCreature;
            creatureTraverse->nextCreature = temp1->nextCreature;
            pop->numCreatures--;
            free(temp1);
        }

        if (lastReproducingCreature == NULL && reproduceResult && !bred && !creatureTraverse->willDie)
        {
            lastReproducingCreature = creatureTraverse;
        }
        

        // last bit in loop, goes to the next creature in linked list
        if (creatureTraverse->nextCreature != NULL)
            creatureTraverse = creatureTraverse->nextCreature;
    }

    bornCreaturesTraverse = bornCreatures;
    // printf("%p\n", bornCreaturesTraverse);
    creatureTraverse->nextCreature = bornCreaturesTraverse;

    
    // while (bornCreaturesTraverse != NULL)
    // {
        
    //     bornCreaturesTraverse = bornCreaturesTraverse->nextCreature;
    //     creatureTraverse = creatureTraverse->nextCreature;
    // }

    // for (int i = 0; i < addNumCreatures; ++i)
    // {
    //     creatureTraverse->nextCreature = createNewCreature();
    //     creatureTraverse = creatureTraverse->nextCreature;
    // }

    return creatureStart;
}

Creature_t * createNewCreature(char gene[])
{
    if (strncmp(gene, "aA", 2) == 0)
    {
        gene[0] = 'A';
        gene[1] = 'a';
    }
    Creature_t * newCreature = malloc(sizeof(Creature_t));
    newCreature->daysAlive = 0;
    newCreature->daysSinceLastEaten = 0;
    newCreature->willDie = 0;
    newCreature->lifetime = 10;
    newCreature->needToEatEvery = 2;
    newCreature->nextCreature = NULL;
    newCreature->gene[2] = '\0';
    newCreature->gene[0] = gene[0];
    newCreature->gene[1] = gene[1];
    return newCreature;
}


void incrementDaysAlive(Creature_t * creature)
{
    creature->daysAlive++;
    if (creature->daysAlive > creature->lifetime)
    {
        creature->willDie = 1;
    }
}

int checkReproduce(Creature_t * creature)
{
    int r = rand() % 100;
    // if (strncmp(creature->gene, "aa", 2) == 0)
    // {
    //     if (r <= 51)
    //         return 1;
    // }
    // else if (strncmp(creature->gene, "AA", 2) == 0)
    // {
    //     if (r <= 49)
    //         return 1;
    // }
    
    if (r <= 50) // 50% chance to reproduce
    {
        return 1;
    }
    
    
    return 0;
    
}

int checkDeath(Creature_t * creature, int * numCreatures)
{
    int r = rand() % 100;

    // // if (strncmp(creature->gene, "aa", 2) == 0)
    // // {
    // //     if (r <= (15 + (0.001 * (*numCreatures))))
    // //         return 1;
    // // }
    // // else if (strncmp(creature->gene, "AA", 2) == 0)
    // // {
    // //     if (r <= (15 + (0.001 * (*numCreatures))))
    // //         return 1;
    // // }

    // if (r <= (5 + (0.001 * (*numCreatures))))
    // {
    //     return 1;
    // }
    return 0;

}