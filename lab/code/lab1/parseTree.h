#ifndef _PARSE_TREE_H_
#define _PARSE_TREE_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// Bool type
typedef enum Boolean{
    true = 1, false = 0
} bool;

// Tree node type
typedef struct TreeNode{
    char* name;             // name of node, e.g. ID, LP, Stmt
    char* propertyValue;    // property value of node, can be empty, e.g. myVar, int, float
    int n_children;         // number of children node
    int first_line;         // first line number of current node
    struct TreeNode* parent;           // parent of current node
    struct TreeNode** children;        // children of current node
    bool if_leaf;           // note if current node is leaf node
} Node;

// Create a node
// Set first_line to -1 if not need to print out
Node* createNode(char* name, char* propertyValue, int first_line);
// Construct tree according to parent-children relationship
void constructTree(Node* parent, int n_children, ...);
// Pre order traverse the tree
void preOrderTraverse(Node* root, int depth);


#endif
