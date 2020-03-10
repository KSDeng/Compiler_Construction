#include "parseTree.h"

// Create a node
// Set first_line to -1 if not need to print out
Node* createNode(char* name, char* propertyValue, int first_line){
    Node* n = (Node*)malloc(sizeof(Node));    // allocate space in memory
    n->name = (char*)malloc(strlen(name) + 1);
    strcpy(n->name, name);
    n->propertyValue = (char*)malloc(strlen(propertyValue) + 1);
    strcpy(n->propertyValue, propertyValue);
    n->first_line = first_line;
    n->n_children = 0;
    n->parent = NULL;
    n->children = NULL;
    return n;
}

// Construct tree according to parent-children relationship
void constructTree(Node* parent, int n_children, ...){
    va_list p;
    va_start(p, n_children);
    parent->children = (Node**)malloc(sizeof(Node*) * n_children);
    parent->n_children = n_children;
    for(int i = 0; i < n_children; ++i){
        parent->children[i] = (Node*)va_arg(p, Node*);
        if(parent->children[i]) parent->children[i]->parent = parent;
    }
    va_end(p);
}

// Pre order traverse the tree
void preOrderTraverse(Node* root, int depth){
//    printf("Pre order traverse, depth %d\n", depth);
    if(!root) return;
    for(int i = 0; i < depth; ++i){
        printf("  ");
    }
    printf("%s", root->name);
    if(strlen(root->propertyValue) > 0)
        printf(": %s", root->propertyValue);
    // cases to show line number
    if(root->first_line != -1)
        printf("  (%d)", root->first_line);
    printf("\n");

    for(int i = 0; i < root->n_children; ++i){
        preOrderTraverse(root->children[i], depth + 1);
    }
    return;
}

