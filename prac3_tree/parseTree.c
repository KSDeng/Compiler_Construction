#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

// Tree node type
typedef struct TreeNode{
    char* name;             // name of node, e.g. ID, LP, Stmt
    char* propertyValue;    // property value of node, can be empty, e.g. myVar, int, float
    int n_children;         // number of children node
    int first_line;         // first line number of current node
    struct TreeNode* parent;           // parent of current node
    struct TreeNode** children;        // children of current node
} Node;


// Create a node
// Set first_line to -1 if not need to print out
Node* createNode(char* name, char* propertyValue, int first_line){
    Node* n;
    n = (Node*)malloc(sizeof(Node));    // allocate space in memory
    n->name = (char*)malloc(strlen(name) + 1);
    strcpy(n->name, name);
    n->propertyValue = (char*)malloc(strlen(propertyValue) + 1);
    strcpy(n->propertyValue, propertyValue);
    n->first_line = first_line;
    n->n_children = 0;
    return n;
}

// Construct tree according to parent-children relationship
void constructTree(Node* parent, int n_children, ...){
    va_list p;
    va_start(p, n_children);
    parent->children = (Node**)malloc(sizeof(Node*) * n_children);
    parent->n_children = n_children;
    for(int i = 0; i < n_children; ++i){
        parent->children[i] = va_arg(p, Node*);
        parent->children[i]->parent = parent;
    }
    va_end(p);
}

// Pre order traverse the tree
void preOrderTraverse(Node* root, int depth){
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

int main(){
    Node* root = createNode("1", "", 1);
    Node* child1 = createNode("2", "", 2);
    Node* child2 = createNode("3", "", 3);
    Node* child3 = createNode("4", "", 4);
    Node* child1child1 = createNode("5", "", 5);
    Node* child3child1 = createNode("6", "int", 6); 
    constructTree(root, 3, child1, child2, child3);
    constructTree(child1, 1, child1child1);
    constructTree(child3, 1, child3child1);
    preOrderTraverse(root, 0);
    return 0;
}
