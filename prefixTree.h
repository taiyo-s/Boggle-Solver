/*
    Written by Taiyo Siew-Adachi
    
    Header for module which contains Prefix Trie 
        data structures and functions.
*/
struct prefixTree;
struct prefixTreeNode;

#ifndef PREFIXTREESTRUCT
#define PREFIXTREESTRUCT
#define CHILD_COUNT (1 << 8)
#define UNMARKED (0)
#define MARKED (1)
#define UNCHECKED (0)
#define CHECKED (1)

struct prefixTree {
    struct prefixTreeNode *root;
};

struct prefixTreeNode {
    int marked; // Output traversal guide
    int checked_all_children; // All nodes below have been visited
    char *prefix; // Current build up of a word
    struct prefixTreeNode *children[CHILD_COUNT]; // Each possible ASCII char 
};

#endif

struct prefixTree *newPrefixTree();

void addWordToTree(struct prefixTree *pt, char *word);

void freePrefixTree(struct prefixTree *tree);
