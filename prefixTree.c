/*
    Written by Taiyo Siew-Adachi
    
    Implementation for module which contains Prefix Trie 
        data structures and functions.
*/
#include "prefixTree.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Creational function for a node in prefix trie, returns created new node */
struct prefixTreeNode *newPrefixTreeNode() {
    struct prefixTreeNode *node = malloc(sizeof(struct prefixTreeNode));
    assert(node);
    
    node->marked = UNMARKED;
    node->checked_all_children = UNCHECKED;
    node->prefix = NULL;
    for (int i = 0; i < CHILD_COUNT; i++) {
        node->children[i] = NULL;
    }
    return node;
}

/* Creational function for a prefix trie, returns the new trie */
struct prefixTree *newPrefixTree(){
    struct prefixTree *trie = malloc(sizeof(struct prefixTree));
    assert(trie);

    trie->root = newPrefixTreeNode();
    return trie;
}

/* Builds the trie passed into the function by adding words into it in a 
    character by character manner, delimiting with null terminator character 
*/
void addWordToTree(struct prefixTree *pt, char *word){
    if (pt == NULL || word == NULL) {
        return;
    }

    if (strlen(word) < 3) {
        return;
    }

    struct prefixTreeNode *cur_node = pt->root;
    for (int i = 0; word[i]; i++) {
        int index = (int)word[i];
        if (cur_node->children[index] == NULL) {
            cur_node->children[index] = newPrefixTreeNode();

            int prefix_len = 0;
            if (cur_node->prefix != NULL) {
                prefix_len = strlen(cur_node->prefix);
            }

            char *next_prefix = malloc(sizeof(char) * (prefix_len + 2));
            assert(next_prefix);
            if (cur_node->prefix) {
                strcpy(next_prefix, cur_node->prefix);
            }
            next_prefix[prefix_len] = word[i];
            next_prefix[prefix_len + 1] = '\0';

            cur_node->children[index]->prefix = next_prefix;
        }
        cur_node = cur_node->children[index];
    }
    cur_node->children['\0'] = newPrefixTreeNode(); // not null anymore

}

/* Free prefix trie nodes using recursive dfs */
void freePrefixTreeNode(struct prefixTreeNode *node) {
    if (node == NULL) {
        return;
    }

    for (int i = 0; i < CHILD_COUNT; i++) {
        if (node->children[i] != NULL) {
            freePrefixTreeNode(node->children[i]);
        }
    }

    if (node->prefix != NULL) {
        free(node->prefix);
    }

    free(node);
}

/* Free struct representing the whole trie */
void freePrefixTree(struct prefixTree *tree) {
    if (tree == NULL) {
        return;
    }

    freePrefixTreeNode(tree->root);

    free(tree);
}
