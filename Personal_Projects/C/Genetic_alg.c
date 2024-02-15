#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//Very simple code for a genetic algorithm
//Recommended population size of ~1000 for ~20 generations for large strings
int main(){
    srand(time(NULL));
    char solution[100];
    int i = 0;
    int j = 0;

    printf("Enter a string: ");
    char c;
    int solution_len;
    for(solution_len=0; (c = getchar()) != EOF && c != '\n' && solution_len<99; solution_len++){
        solution[solution_len] = c;
    }
    solution[solution_len] = '\0';
    //Population must be greater than 10
    int population_size = 0;
    printf("Enter a population size greater or equal to 10: ");
    char ch;
    int valid = 1;
    do {
        ch = getchar();
        if (ch >= 48 && ch <= 57) {
            population_size = population_size * 10 + (ch - 48);
        }
        else if (ch == '\n' || ch == EOF){
            if(population_size < 10 || valid == 0){
                printf("Invalid population size\n");
                printf("Enter a population size greater than 10: ");
                population_size = 0;
                valid = 1;
            }
            else
                break;
        }
        else{
            valid = 0;
        }
    } while (1);

    //Get amount of generations to run for
    int generations = 0;
    printf("How many generations to run for: ");
    do {
        ch = getchar();
        if (ch >= 48 && ch <= 57) {
            generations = generations * 10 + (ch - 48);
        }
        else if (ch == '\n' || ch == EOF){
            if(valid == 0){
                printf("Invalid generations amount\n");
                printf("How many generations to run for: ");
                generations = 0;
                valid = 1;
            }
            else
                break;
        }
        else{
            valid = 0;
        }
    } while (1);

    //Create a random initial Population
    char population[population_size][100];
    for(i=0; i<population_size; i++){
        for(j=0; j<solution_len; j++){
            population[i][j] = rand()%95+32;
        }
        population[i][j] = '\0';
    }

    //Display fitness of the population
    int fitness[population_size];
    printf("Initial population:\n");
    for(i=0; i<population_size; i++){
        printf("%d\t", i+1);
        fitness[i]=0;
        for(j=0; j<solution_len; j++){
            printf("%c", population[i][j]);
            if(population[i][j] == solution[j])
                fitness[i]++;
        }
        printf("\tfitness: %d\n", fitness[i]);
    }

    int children=0;
    char new_gen[population_size][100];

    //Copy population to new gen for first iteration to work
    for(i=0;i<population_size;i++){
        for(j=0;j<solution_len;j++){
            new_gen[i][j]=population[i][j];
        }
        new_gen[i][j] = '\0';
    }

    int elite_population_size=population_size/2;
    char elite_population[elite_population_size][100];
    //int fitness_copy[population_size];
    int g = 0;
    while(g<generations){

        //Elitism
        //for(i=0;i<population_size;i++){
            //fitness_copy[i]=fitness[i];
        //}
        int k;
        int l;
        int max;
        for(i=0;i<elite_population_size;i++){
            max=-1;
            for(j=0;j<population_size;j++){
                if(fitness[j]>max){
                    max=j;
                }
            }
            fitness[max]=-1;
            for(k=0;k<solution_len;k++){
                elite_population[i][k]=new_gen[max][k];
            }
        }
        
        
        //printf("Elite gen:\n");
        for(i=0;i<elite_population_size/5;i++){
            for(j=0;j<solution_len;j++){
                new_gen[i][j] = elite_population[i][j];
                //printf("%c", elite_population[i][j]);
            }
            new_gen[i][j] = '\0';
            children++;
            //printf("\n");
        }
        

        while (children<population_size){
            int rand_int1 = rand()%elite_population_size/2;
            int rand_int2 = rand()%elite_population_size/2;
            char parent1[solution_len];
            char parent2[solution_len];
            for(i=0; i<solution_len; i++)
                    parent1[i] = elite_population[rand_int1][i];
            parent1[i] = '\0';
            for(i=0; i<solution_len; i++)
                    parent2[i] = elite_population[rand_int2][i];
            parent2[i] = '\0';

            //Create children
            char child1[solution_len];
            char child2[solution_len];
            int random;
            for(i=0;i<solution_len;i++){
                random = rand()%11;
                if (random<5)
                    child1[i] = parent1[i];
                else if (random>5)
                    child1[i] = parent2[i];
                else
                    child1[i] = rand()%95+32;
            }
            child1[i] = '\0';

            for(i=0;i<solution_len;i++){
                random = rand()%11;
                if (random<5)
                    child2[i] = parent1[i];
                else if (random>5)
                    child2[i] = parent2[i];
                else
                    child2[i] = rand()%95+32;
            }
            child2[i] = '\0';
            //printf("Child 1: %s\n", child1);
            //printf("Child 2: %s\n", child2);
            
            //Put children in new generation
            for(j=0; j<solution_len; j++){
                new_gen[children][j] = child1[j];
            }
            new_gen[children][j] = '\0';
            children++;
            if(children<population_size){
            for(j=0; j<solution_len; j++){
                new_gen[children][j] = child2[j];
            }
            new_gen[children][j] = '\0';
            children++;
            }
        }
        printf("Generation: %d\n", g+1);
        for(i=0; i<population_size; i++){
                printf("%d\t", i+1);
                fitness[i]=0;
                for(j=0; j<solution_len; j++){
                    printf("%c", new_gen[i][j]);
                    if(new_gen[i][j] == solution[j])
                        fitness[i]++;
                }
                printf("\tfitness: %d\n", fitness[i]);
        }
        max=-1;
        for(i=0;i<population_size;i++){
            if(fitness[i]>max){
                max=i;
            }
        }
        printf("\nBest solution of the current generation:\n");
        for(i=0; i<solution_len; i++){
            printf("%c", new_gen[max][i]);
        }
        printf("\tfitness: %d\n", fitness[max]);
        printf("\n");
    children=0;
    g++;
    }
}