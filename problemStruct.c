/*
    Written by Grady Fitzpatrick for 
    COMP20007 Assignment 2 2024 Semester 1
    
    Implementation for data structure used
        in storing the problem and its
        information.
*/

#ifndef PROBLEMPARTENUM_DEF
#define PROBLEMPARTENUM_DEF 1
enum problemPart {
    VALID_WORDS = 0,
    HINT = 1,
};
#endif

struct problem {
    /* The number of words in the text. */
    int wordCount;
    /* The list of words in the dictionary. */
    char **words;

    /* The dimension of the board (number of rows) */
    int dimension;

    /* The board, represented both as a 2-D list */
    char **board;

    /* The partial string. */
    char *partialString;

    /* Which problem part is being solved. */
    enum problemPart part;
};


