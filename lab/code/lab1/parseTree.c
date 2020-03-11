#include "parseTree.h"
bool debug = false;      // set true to enter debug mode
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
    n->if_leaf = false;     // not leaf node by default
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
    if(!debug){
        if(root -> if_leaf){
            if( strcmp(root->name, "TYPE") == 0 ||
                strcmp(root->name, "ID") == 0   ||
                strcmp(root->name, "INT") == 0  ||
                strcmp(root->name, "FLOAT") == 0)
                    printf(": %s", root->propertyValue);
        }else{
            printf("  (%d)", root->first_line);
        }
        printf("\n");
    }else{
        printf(": %s [%d]\n", root->propertyValue, root->first_line);
    }

    for(int i = 0; i < root->n_children; ++i){
        preOrderTraverse(root->children[i], depth + 1);
    }
    return;
}

// Function to print error type A
void printErrorTypeA(char* msg, int lineno){
    printf("Error type A at Line %d: Mysterious character \"%s\".\n", lineno, msg);
}
// Function to print error type B
void printErrorTypeB(char* msg, int lineno){
    fprintf(stderr, "Error type B at Line %d: %s\n", lineno, msg);
    // printf("Error type B at line %d: %s\n", lineno, str);
}

