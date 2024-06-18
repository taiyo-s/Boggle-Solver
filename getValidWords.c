/*
    Written by Grady Fitzpatrick and Taiyo Siew-Adachi
    
    Main file for the case where all valid words for a given board should be 
    retrieved

*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <error.h>
#include "problem.h"

int main(int argc, char **argv){
    struct problem *problem;
    struct solution *solution;
    /* Load file with dictionary */
    FILE *dictFile = NULL;
    
    /* Attempt to open dictionary and board files. */
    dictFile = fopen("10k-dict.txt", "r");
    if(! dictFile){
        fprintf(stderr, "File given as dictionary file was 10k-dict.txt, "
            "which was unable to be opened\n");
        perror("Reason for file open failure");
        return EXIT_FAILURE;
    }
    
    problem = readProblem(dictFile);

    /* Files are no longer needed since they're read in. */
    if(dictFile){
        fclose(dictFile);
    }

    solution = solveWords(problem);

    outputProblem(problem, solution, stdout);

    freeSolution(solution, problem);

    freeProblem(problem);

    return EXIT_SUCCESS;
}
