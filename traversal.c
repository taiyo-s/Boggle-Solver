/*
    Implementation for a graph traversel algorithm, Depth First Search.
 
    Code Written by Taiyo Siew-Adachi - 1461593, with reference to pseudocode 
    from lecture slides by Lars Kulik. Modified to a variation for traversing 
    a 2D array, assumin they are connected
*/
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>

#include "problem.h"
#include "prefixTree.h"
#include "traversal.h"

/* Number of words to allocate space for initially. */
#define INITIALWORDSALLOCATION 64

/* Denotes that the dimension has not yet been set. */
#define DIMENSION_UNSET (-1)
#define VISITED 1
#define NOT_VISITED 0
#define USED 1
#define UNUSED 0
#define PATH_TO_VALID_WORD 1
#define PATH_TO_INVALID_WORD 0

/* Traverses the trie through marked nodes in search for words that can be made
    using a dfs-like traversal technique. Updates the word list to be assigned 
    to solution struct
 */
void dfsExplore(struct prefixTreeNode *node, char ***words, int *num) {
    for (int i = 0; i < CHILD_COUNT; i++) {
        if (node->children[i] != NULL) {
            if (i == '\0') {
                if (*num >= INITIALWORDSALLOCATION) {
                    *words = realloc(*words, sizeof(char *) * 2*(*num));
                    assert(*words);
                }
                (*words)[*num] = malloc(sizeof(char)*(strlen(node->prefix)+1));
                assert((*words)[*num]);
                strcpy((*words)[*num], node->prefix);
                (*num)++;
            }
            else if (node->children[i]->marked) {
                node->children[i]->marked = UNMARKED;
                dfsExplore(node->children[i], words, num); 
            }
        }
    } 
}

/* Retrieves valid words in from the trie by calling dfsExplore() for each 
    starting character
 */
void retrieveValidWords(struct prefixTree *tree, char ***words, int *word_num) {
    for (int i = 0; i < CHILD_COUNT; i++) {
        if (tree->root->children[i]) {
            if (tree->root->children[i]->marked) {
                tree->root->children[i]->marked = UNMARKED;
                dfsExplore(tree->root->children[i], words, word_num);
            }
        }
    } 
}

/* Retrieves next characters allowed, given a word is currently in the making */
void retrieveNextChars(struct prefixTree *tree, char *partialString, 
    char *chars, int *char_num) {
    struct prefixTreeNode *node = tree->root;
    for (int i = 0; partialString[i]; i++) {
        if (node->children[tolower(partialString[i])]) {
                /* There are valid words starting with the char at this cell */
                node = node->children[tolower(partialString[i])];        
        }
        else {
            return;
        }
    }
    /* At last letter in partial string */
    for (int i = 0; i < CHILD_COUNT; i++) {
        if (node->children[i]) {
            if (isalpha(i)) {
                chars[*char_num] = i;
                (*char_num)++; 
            }
            else {
                chars[*char_num] = ' ';
                (*char_num)++; 
            }
        }
    }
}

/* Returns whether the it is allowed to traverse to the cell we want to for Part 
    D, meaning there is an extra condition of not allowing re-use of characters 
*/
int isValidCell(int row, int col, int dimension, int *usage, char **board) {
    return row >= 0 && col >= 0 && row < dimension && col < dimension &&
           !usage[tolower(board[row][col])];
}

/* Explore valid adjacent board cells, which considers the prevention of re-use 
    of characters, while simulataneosuly travering the trie and updating it 
    accordingly. Utilises dfs-like traversal, and backtracking to keep track of 
    visited board cells and whether a specific node in the trie is on the right 
    path to a valid word
*/
int explore(char **board, int row, int col, int dimension,
    struct prefixTreeNode *node, int *usage) {

    usage[tolower(board[row][col])] = USED;
    int path_state = PATH_TO_INVALID_WORD;

    int checked_all_children = CHECKED;
    if (node->children['\0'] != NULL) {
        path_state = PATH_TO_VALID_WORD;
        node->children['\0']->checked_all_children = CHECKED;
    }
    for (int i = 0; i < CHILD_COUNT; i++) {
        if (node->children[i]) {
            if (!node->children[i]->checked_all_children) {
                checked_all_children = UNCHECKED;
            }
        }
    }
    if (checked_all_children) {
        node->checked_all_children = CHECKED;
    }

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            }
            if (isValidCell(row + i, col + j, dimension, usage, board) 
                && !node->checked_all_children && 
                node->children[tolower(board[row + i][col + j])]) {
                if (explore(board, row + i, col + j, dimension, 
                    node->children[tolower(board[row + i][col + j])], usage)) {
                    path_state = PATH_TO_VALID_WORD;
                }
            }
        }
    }

    if (path_state) {
        node->marked = MARKED;
    }

    usage[tolower(board[row][col])] = UNUSED;
    return path_state;
}

/* Search the board from a starting cell. Calling function for the rest of the 
    traversing of the board
 */
void searchFromCell(char **board, int row, int col, int dimension, 
    struct prefixTreeNode *node, int *usage) {

    usage[tolower(board[row][col])] = USED;

    for (int i = -1; i <= 1; i++) {
        for (int j = -1; j <= 1; j++) {
            if (i == 0 && j == 0) {
                continue;
            } else if (isValidCell(row + i, col + j, dimension, usage, board) 
                && node->children[tolower(board[row + i][col + j])]) {
                if (explore(board, row + i, col + j, dimension, 
                    node->children[tolower(board[row + i][col + j])], usage)) {
                    node->marked = MARKED;
                }
            }
        }
    }

    usage[tolower(board[row][col])] = UNUSED;
}