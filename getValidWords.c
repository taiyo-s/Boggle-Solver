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

#define DICT_FILE_ARG 1
#define BOARD_FILE_ARG 2

int main(int argc, char **argv){
    struct problem *problem;
    struct solution *solution;
    /* Load file with dictionary from argv[1]. */
    FILE *dictFile = NULL;
    /* Load file with table from argv[2]. */
    FILE *boardFile = NULL;

    if(argc < 3){
        fprintf(stderr, "You only gave %d arguments to the program, \n"
            "you should run the program with in the form \n"
            "\t./problem2a dictionary board\n", argc);
        return EXIT_FAILURE;
    } 
    
    /* Attempt to open dictionary and board files. */
    dictFile = fopen(argv[DICT_FILE_ARG], "r");
    if(! dictFile){
        fprintf(stderr, "File given as dictionary file was \"%s\", "
            "which was unable to be opened\n", argv[DICT_FILE_ARG]);
        perror("Reason for file open failure");
        return EXIT_FAILURE;
    }

    boardFile = fopen(argv[BOARD_FILE_ARG], "r");
    if(! boardFile){
        fprintf(stderr, "File given as board file was \"%s\", "
            "which was unable to be opened\n", argv[BOARD_FILE_ARG]);
        perror("Reason for file open failure");
        return EXIT_FAILURE;
    }
    
    problem = readProblem(dictFile, boardFile);

    /* Files are no longer needed since they're read in. */
    if(dictFile){
        fclose(dictFile);
    }

    if(boardFile){
        fclose(boardFile);
    }

    solution = solveWords(problem);

    outputProblem(problem, solution, stdout);

    freeSolution(solution, problem);

    freeProblem(problem);

    return EXIT_SUCCESS;
}
