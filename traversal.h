/*
    Written by Taiyo Siew-Adachi

    Header for module which contains tree traversal algorithm functions all 
    using a dfs approach
*/
#ifndef TRAVERSAL_H
#define TRAVERSAL_H

/* Retrieves words available to made from the board, given the allowed words */
void retrieveValidWords(struct prefixTree *tree, char ***words, int *word_num);
/* Retrieves next characters allowed, given a word is currently in the making */
void retrieveNextChars(struct prefixTree *tree, char *partialString, 
    char *chars, int *char_num);
/* Searches adjacent cells for valid prefixes to an allowed word with 
    resitriction of any character can only be used once */
void searchFromCell(char **board, int row, int col, int dimension,
                    struct prefixTreeNode *node, int *usage);

#endif
