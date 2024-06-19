/*
    Written by Grady Fitzpatrick and Taiyo Siew-Adachi

    Main file for the case where all valid next characters for a given board 
    and partial word, should be retrieved
    

*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "problem.h"

int main(int argc, char **argv){
    struct problem *problem;
    struct solution *solution;
    /* Load file with dictionary */
    FILE *dictFile = NULL;
    
    /* Attempt to open dictionary */
    dictFile = fopen("10k-dict.txt", "r");
    if(! dictFile){
        fprintf(stderr, "File given as dictionary file was 10k-dict.txt, "
            "which was unable to be opened\n");
        perror("Reason for file open failure");
        return EXIT_FAILURE;
    }
    
    problem = readProblemWithPartialWord(dictFile);

    /* Files are no longer needed since they're read in. */
    if(dictFile){
        fclose(dictFile);
    }

    solution = solveHint(problem);

    outputProblem(problem, solution, stdout);

    freeSolution(solution, problem);

    freeProblem(problem);

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    printf("------------------------------");
    printf("\nPress 'Enter' to exit...\n");
    printf("------------------------------\n");
    getchar();

    return EXIT_SUCCESS;
}
