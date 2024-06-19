/*
    Reading in and outputting, written by Grady Fitzpatrick for 
    COMP20007 Assignment 2 2024 Semester 1. Further modified by 
    Taiyo Siew-Adachi
    
    Solving of game algorithms implemented by Taiyo Siew-Adachi

    Functions used to read the board, read the dictionary, solve the game, 
    give hints, and to output the solution or hint.

*/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "problem.h"
#include "problemStruct.c"
#include "solutionStruct.c"
#include "prefixTree.h"
#include "traversal.h"
/* Number of cells a Boggle Board has */
#define BOARD_CELLS 16
/* Number of rows/cols a Boggle Board has */
#define BOARD_SIDE_LEN 4

/* Number of words to allocate space for initially. */
#define INITIALWORDSALLOCATION 64
/* Number of spaces available to move from a single cell */
#define LETTERSALLOCATION 8

/* Denotes that the dimension has not yet been set. */
#define DIMENSION_UNSET (-1)

#define NOT_VISITED 0
#define UNUSED 0

struct problem;
struct solution;

/* Sets up a solution for the given problem. */
struct solution *newSolution(struct problem *problem);
/* Creates a new 2D array representing the board cells that have been visited */
int **newVisitedMatrix(int dimension);
/* Sets all flags to not visited */
void resetVisitedMatrix(int **visited_matrix, int dimension);
/* Free the memory for the visited matrix */
void freeVisitedMatrix(int **visited_matrix, int dimension);
/* Creates a new array representing used characters to avoid re-using */
int *newCharUsage();
/* Sets all characters to not used */
void resetCharUsage(int *usage);
/* Free the memory for the array */
void freeCharUsage(int *usage);

/* reads from stream until it encounters a character matching the delimiter 
   character
*/
ssize_t getdelim(char **lineptr, size_t *n, int delimiter, FILE *stream) {
    if (lineptr == NULL || n == NULL || stream == NULL) {
        return -1;
    }

    if (*lineptr == NULL) {
        *n = 128;
        *lineptr = malloc(*n);
        if (*lineptr == NULL) {
            return -1;
        }
    }

    size_t pos = 0;
    int c;
    while ((c = fgetc(stream)) != EOF) {
        if (pos + 1 >= *n) {
            *n *= 2;
            char *new_ptr = realloc(*lineptr, *n);
            if (new_ptr == NULL) {
                return -1;
            }
            *lineptr = new_ptr;
        }

        (*lineptr)[pos++] = (char)c;

        if (c == delimiter) {
            break;
        }
    }

    if (pos == 0 && c == EOF) {
        return -1;
    }

    (*lineptr)[pos] = '\0';
    return pos;
}

/* 
    Reads the given dict file into a list of words 
    and the given board file into a nxn board.
*/
struct problem *readProblem(FILE *dictFile){
    struct problem *p = (struct problem *) malloc(sizeof(struct problem));
    assert(p);

    p->partialString = NULL;

    int wordCount = 0;
    int wordAllocated = 0;
    char *dictText = NULL;
    char **words = NULL;

    /* Read in text. */
    size_t allocated = 0;
    /* Exit if we read no characters or an error caught. */
    int success = getdelim(&dictText, &allocated, '\0', dictFile);

    if(success == -1){
        /* Encountered an error. */
        perror("Encountered error reading dictionary file");
        exit(EXIT_FAILURE);
    } else {
        /* Assume file contains at least one character. */
        assert(success > 0);
    }

    /* Progress through string. */
    int progress = 0;
    /* Table string length. */
    int dictTextLength = strlen(dictText);

    /* Count words present. */
    while(progress < dictTextLength){
        char *word = NULL;
        int nextProgress;
        /* Read each value into the dictionary. */
        if(progress == 0){
            /* First line. */
            int wordNumberGuess;
            assert(sscanf(dictText + progress, "%d %n", &wordNumberGuess, &nextProgress) == 1);
            /* Will fail if integer missing from the start of the words. */
            assert(nextProgress > 0);
            if(wordNumberGuess > 0){
                wordAllocated = wordNumberGuess;
                words = (char **) malloc(sizeof(char *) * wordAllocated);
                assert(words);
            }
            progress += nextProgress;
            continue;
        } else {
            assert(sscanf(dictText + progress, "%m[^\n] %n", &word, &nextProgress) == 1);
            assert(nextProgress > 0);
            progress += nextProgress;
        }

        /* Check if more space is needed to store the word. */
        if(wordAllocated <= 0){
            words = (char **) malloc(sizeof(char *) * INITIALWORDSALLOCATION);
            assert(words);
            wordAllocated = INITIALWORDSALLOCATION;
        } else if(wordCount >= wordAllocated){
            words = (char **) realloc(words, sizeof(char *) * 
                wordAllocated * 2);
            assert(words);
            wordAllocated = wordAllocated * 2;
            /* Something has gone wrong if there's not sufficient word 
                space for another word. */
            assert(wordAllocated > wordCount);
        }

        words[wordCount] = word;
        wordCount++;
    }
    
    /* Done with dictText */
    if(dictText){
        free(dictText);
    }
    
    char *boardText = malloc((BOARD_CELLS + 1) * sizeof(char));
    assert(boardText);

    /* Now read in board */
    printf("Enter Board Here: ");
    assert(scanf("%s", boardText) == 1);

    while (strlen(boardText) != BOARD_CELLS) {
        printf("\e[31mInvalid Board size (Board must be of size 4x4)\e[0m\n");
        printf("Enter Board Here: ");
        assert(scanf("%s", boardText) == 1);
    }
    
    char **board = (char **) malloc(sizeof(char **) * BOARD_SIDE_LEN);
    assert(board);
    for (int i = 0; i < BOARD_SIDE_LEN; i++) {
        board[i] = malloc(sizeof(char *) * BOARD_SIDE_LEN);
        assert(board[i]);
    }
    
    for(int i = 0; i < BOARD_SIDE_LEN; i++){
        for(int j = 0; j < BOARD_SIDE_LEN; j++){
            board[i][j] = boardText[i*BOARD_SIDE_LEN + j];
        }
    }

    printf("Given board:\n");
    for(int i = 0; i < BOARD_SIDE_LEN; i++){
        for(int j = 0; j < BOARD_SIDE_LEN; j++){
            printf("%c ", toupper(board[i][j]));
        }
        printf("\n");
    }

    free(boardText);

    /* The number of words in the text. */
    p->wordCount = wordCount;
    /* The list of words in the dictionary. */
    p->words = words;

    /* The dimension of the board (number of rows) */
    p->dimension = BOARD_SIDE_LEN;

    /* The board, represented both as a 2-D list */
    p->board = board;

    p->part = VALID_WORDS;

    return p;
}

struct problem *readProblemWithPartialWord(FILE *dictFile){
    struct problem *p = readProblem(dictFile);

    char *partialString = malloc((BOARD_CELLS + 1) * sizeof(char));
    assert(partialString);

    /* String that is partially given - we assume it follows 
        word conventions for the %s specifier. */
    printf("Enter Partial Word Here: ");
    assert(scanf("%s", partialString) == 1);
    
    p->part = HINT;
    p->partialString = partialString;

    return p;
}

/*
    Outputs the given solution to the given file. If colourMode is 1, the
    sentence in the problem is coloured with the given solution colours.
*/
void outputProblem(struct problem *problem, struct solution *solution, 
    FILE *outfileName){
    assert(solution);
    switch(problem->part){
        case VALID_WORDS:
            fprintf(outfileName, "Valid words:\n");
            assert(solution->foundWordCount == 0 || solution->words);
            for(int i = 0; i < solution->foundWordCount; i++){
                fprintf(outfileName, "%s\n", solution->words[i]);
            }
            break;
        case HINT:
            printf("Hint(s):\n");
            assert(solution->foundLetterCount == 0 || solution->followLetters);
            if (!(solution->foundLetterCount)) {
                fprintf(outfileName, "\e[31mNo words can be made from: %s\e[0m\n", 
                    problem->partialString);
                break;
            }
            for(int i = 0; i < solution->foundLetterCount; i++){
                if(isalpha(solution->followLetters[i])){
                    fprintf(outfileName, "%c\n", solution->followLetters[i]);
                } else {
                    fprintf(outfileName, " \n");
                }
            }
            break;
    }
}

/*
    Frees the given solution and all memory allocated for it.
*/
void freeSolution(struct solution *solution, struct problem *problem){
    if(solution){
        if(solution->followLetters){
            free(solution->followLetters);
        }
        if(solution->words){
            for (int i = 0; i < solution->foundWordCount; i++) {
                if (solution->words[i]) {
                    free(solution->words[i]);
                }
            }
            free(solution->words);
        }
        free(solution);
    }
}

/*
    Frees the given problem and all memory allocated for it.
*/
void freeProblem(struct problem *problem){
    if(problem){
        if(problem->words){
            for(int i = 0; i < problem->wordCount; i++){
                if(problem->words[i]){
                    free(problem->words[i]);
                }
            }
            free(problem->words);
        }
        if(problem->board){
            free(problem->board);
        }
        if(problem->partialString){
            free(problem->partialString);
        }
        free(problem);
    }
}

/* Sets up a solution for the given problem */
struct solution *newSolution(struct problem *problem){
    struct solution *s = (struct solution *) malloc(sizeof(struct solution));
    assert(s);
    s->foundWordCount = 0;
    s->words = NULL;
    s->foundLetterCount = 0;
    s->followLetters = NULL;
    
    return s;
}

/*
    Solves the given problem according to Part A's definition
    and places the solution output into a returned solution value.
*/
struct solution *solveWords(struct problem *p){
    struct solution *s = newSolution(p);

    int dimension = p->dimension;

    struct prefixTree *tree = newPrefixTree();
    for (int i = 0; i < p->wordCount; i++) {
        addWordToTree(tree, p->words[i]);
    }

    /* Keeps track of the used characters in each word. Prevents re-use of 
        characters and re-visiting the same cell on the board 
    */
    int *usage = newCharUsage();
    /* Search starting from each cell to determine which words can be made */
    struct prefixTreeNode *node = tree->root;
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            int index = tolower(p->board[i][j]);
            if (node->children[index]) {
                /* There are valid words starting with the char at this cell */
                resetCharUsage(usage);
                searchFromCell(p->board, i, j, dimension, 
                    node->children[index], usage);            
            }
        }
    }
    freeCharUsage(usage);

    int word_count = 0;
    char **words_found = malloc(INITIALWORDSALLOCATION * sizeof(char *));
    assert(words_found);

    retrieveValidWords(tree, &words_found, &word_count);
    
    freePrefixTree(tree);
    s->foundWordCount = word_count;
    s->words = words_found;

    return s;
}

/*
    Solves the given problem according to Part B's definition
    and places the solution output into a returned solution value.
*/
struct solution *solveHint(struct problem *p){
    struct solution *s = newSolution(p);

    struct prefixTree *tree = newPrefixTree();
    for (int i = 0; i < p->wordCount; i++) {
        addWordToTree(tree, p->words[i]);
    }

    int dimension = p->dimension;

    /* Keeps track of the used characters in each word. Prevents re-use of 
        characters and re-visiting the same cell on the board 
    */
    int *usage = newCharUsage();

    /* Search starting from each cell to determine which words can be made */
    struct prefixTreeNode *node = tree->root;
    for (int i = 0; i < dimension; i++) {
        for (int j = 0; j < dimension; j++) {
            int index = tolower(p->board[i][j]);
            if (node->children[index]) {
                /* There are valid words starting with the char at this cell */
                resetCharUsage(usage);
                searchFromCell(p->board, i, j, dimension, node->children[index], 
                    usage);                
            }
        }
    }

    freeCharUsage(usage);

    int foundLetterCount = 0;
    char *followLetters = malloc(LETTERSALLOCATION * sizeof(char));
    assert(followLetters);

    retrieveNextChars(tree, p->partialString, followLetters, &foundLetterCount);

    freePrefixTree(tree);
    s->foundLetterCount = foundLetterCount;
    s->followLetters= followLetters;

    return s;
}

/*
    Creates a new 2D array to represent the visited states of cells on the
    Boggle board
*/
int **newVisitedMatrix(int dimension) {
    int **visited = malloc(sizeof(int *) * dimension);
    assert(visited);
    for(int i = 0; i < dimension; i++){
        visited[i] = malloc(sizeof(int) * dimension);
        assert(visited[i]);
    }
    return visited;
}

/*
    Resets the visited matrix by setting all the cells back to not visited state
*/
void resetVisitedMatrix(int **visited_matrix, int dimension) {
    for(int i = 0; i < dimension; i++){
        for (int j = 0; j < dimension; j++) {
            visited_matrix[i][j] = NOT_VISITED;
        }
    }
}

/*
    Frees the visited matrix from heap memory
*/
void freeVisitedMatrix(int **visited_matrix, int dimension) {
    if (visited_matrix) {
        for(int i = 0; i < dimension; i++){
            free(visited_matrix[i]);
        }
        free(visited_matrix);        
    }
}

int *newCharUsage() {
    int *usage = malloc(sizeof(int) * CHILD_COUNT);
    assert(usage);

    return usage;
}

/*
    Resets the visited matrix by setting all the cells back to not visited state
*/
void resetCharUsage(int *usage) {
    for(int i = 0; i < CHILD_COUNT; i++){
        usage[i] = UNUSED; 
    }
}

/*
    Frees the visited matrix from heap memory
*/
void freeCharUsage(int *usage) {
    if (usage) {
        free(usage);        
    }
}


