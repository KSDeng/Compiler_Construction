%{
    #include "parseTree.h"
    #include "lex.yy.c"
    Node* root;
    extern int n_error;    // number of errors
    extern bool debug;
    void printDebug2(char* str, int lineno){
        if(debug) printf("%s (%d)\n",str, lineno);
    }
    void printErrorTypeB(char* str, int lineno){
        fprintf(stderr, "Error type B at Line %d: %s\n", lineno, str);
        // printf("Error type B at line %d: %s\n", lineno, str);
    }
%}

/* Types */
%union {
    int type_int;
    float type_float;
    char* type_str;
    Node* type_node;
}

/* Tokens */
/* terminals */
%token <type_node> INT FLOAT ID SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV
%token <type_node> AND OR DOT NOT TYPE LP RP LB RB LC RC
%token <type_node> STRUCT RETURN IF ELSE WHILE
/* non-terminals */
%type <type_node> Program ExtDefList ExtDef ExtDecList
%type <type_node> Specifier StructSpecifier OptTag Tag
%type <type_node> VarDec FunDec VarList ParamDec
%type <type_node> CompSt StmtList Stmt
%type <type_node> DefList Def DecList Dec
%type <type_node> Exp Args

/* errors */
%token <type_node> INVALID_ID

/* Associative property and priority */
/* https://zh.cppreference.com/w/c/language/operator_precedence */
%left COMMA
%right ASSIGNOP
%left OR
%left AND
%left RELOP
%left PLUS MINUS
%left STAR DIV
%right NOT
%left LP RP LB RB DOT

%%

/* High-level Definitions */
Program : ExtDefList        { printDebug2("Program -> ExtDefList", @$.first_line);
                              $$ = createNode("Program", "", @$.first_line);
                              constructTree($$, 1, $1);
                              root = $$;                                }
    ;

ExtDefList : ExtDef ExtDefList  { printDebug2("ExtDefList -> ExtDef ExtDefList", @$.first_line);
                                  $$ = createNode("ExtDefList", "", @$.first_line);
                                  constructTree($$, 2, $1, $2);         }

    |   /*  empty   */          { printDebug2("ExtDefList -> Empty", @$.first_line);    
                                  $$ = NULL;     }
    ;

ExtDef : Specifier ExtDecList SEMI  { printDebug2("ExtDef -> Specifier ExtDecList SEMI", @$.first_line);
                                      $$ = createNode("ExtDef", "", @$.first_line);
                                      constructTree($$, 3, $1, $2, $3);                 }
    | Specifier SEMI                { printDebug2("ExtDef -> Specifier SEMI", @$.first_line);  
                                      $$ = createNode("ExtDef", "", @$.first_line);
                                      constructTree($$, 2, $1, $2);                     }
    | Specifier FunDec CompSt       { printDebug2("ExtDef -> Specifier FuncDec CompSt", @$.first_line);
                                      $$ = createNode("ExtDef", "", @$.first_line);
                                      constructTree($$, 3, $1, $2, $3);                 }
    | Specifier error               { printDebug2("ExtDef -> Specifier error", @$.first_line);
                                      n_error++;
                                      $$ = createNode("Error", "", @$.first_line);
                                      printErrorTypeB("Missing \";\"", @$.last_line);      }
    ;

ExtDecList : VarDec     { printDebug2("ExtDecList -> VarDec", @$.first_line);  
                          $$ = createNode("ExtDecList", "", @$.first_line);
                          constructTree($$, 1, $1);  }
    | VarDec COMMA ExtDecList   { printDebug2("ExtDecList -> VarDec COMMA ExtDecList", @$.first_line);
                                  $$ = createNode("ExtDecList", "",  @$.first_line);
                                  constructTree($$, 3, $1, $2, $3);   }
    ;

/* Specifiers */
Specifier : TYPE    { printDebug2("Specifier -> TYPE", @$.first_line); 
                      $$ = createNode("Specifier", "", @$.first_line);
                      constructTree($$, 1, $1);     }
    | StructSpecifier   { printDebug2("Specifier -> StructSpecifier", @$.first_line);
                          $$ = createNode("Specifier", "", @$.first_line);
                          constructTree($$, 1, $1);     }
    ;

StructSpecifier : STRUCT OptTag LC DefList RC   { printDebug2("StructSpecifier -> STRUCT OptTag LC DefList RC", @$.first_line);   
                                                  $$ = createNode("StructSpecifier", "", @$.first_line);
                                                  constructTree($$, 5, $1, $2, $3, $4, $5);     }
    | STRUCT Tag    { printDebug2("StructSpecifier -> STRUCT Tag", @$.first_line); 
                      $$ = createNode("StructSpecifier", "", @$.first_line);
                      constructTree($$, 2, $1, $2);  }
    ;

OptTag : ID     { printDebug2("OptTag -> ID", @$.first_line);
                  $$ = createNode("OptTag", "", @$.first_line);
                  constructTree($$, 1, $1);         }
    |   /*  empty  */   { printDebug2("OptTag -> Empty", @$.first_line);
                          $$ = NULL;     }
    ;

Tag : ID    { printDebug2("Tag -> ID", @$.first_line); 
              $$ = createNode("Tag", "", @$.first_line);
              constructTree($$, 1, $1);     }
    ;

/* Declarations */
VarDec : ID    { printDebug2("VarDec -> ID", @$.first_line);
                 $$ = createNode("VarDec", "", @$.first_line);
                 constructTree($$, 1, $1);  }
    | VarDec LB INT RB  { printDebug2("VarDec -> VarDec LB INT RB", @$.first_line);    
                          $$ = createNode("VarDec", "", @$.first_line);  
                          constructTree($$, 4, $1, $2, $3, $4);     }
    | VarDec LB ID RB   { printDebug2("VarDec -> VarDec LB ID RB", @$.first_line);
                          $$ = createNode("Error", "", @$.first_line);
                          n_error++;
                          printErrorTypeB("Invalid array declaration, dynamic space not allowed", @$.first_line);    }
    ;

FunDec : ID LP VarList RP   { printDebug2("FunDec -> ID LP VarList RP", @$.first_line);
                              $$ = createNode("FunDec", "", @$.first_line);
                              constructTree($$, 4, $1, $2, $3, $4);          }
    | ID LP RP  { printDebug2("FunDec -> ID LP RP", @$.first_line);    
                  $$ = createNode("FunDec", "", @$.first_line);
                  constructTree($$, 3, $1, $2, $3);     }
    | INVALID_ID LP RP       {  printDebug2("FunDec -> INVALID_ID LP RP", @$.first_line);
                                $$ = createNode("FunDec", "", @$.first_line);
                                constructTree($$, 3, $1, $2, $3);
                                n_error++;
                                printErrorTypeB("Invalid ID", @$.first_line);     }   
    ;

VarList : ParamDec COMMA VarList    { printDebug2("VarList -> ParamDec COMMA VarList", @$.first_line);
                                      $$ = createNode("VarList", "", @$.first_line);
                                      constructTree($$, 3, $1, $2, $3);         }
    | ParamDec  { printDebug2("VarList -> ParamDec", @$.first_line);
                  $$ = createNode("VarList", "", @$.first_line);
                  constructTree($$, 1, $1);         }
    ;

ParamDec : Specifier VarDec     { printDebug2("ParamDec -> Specifier VarDec", @$.first_line);
                                  $$ = createNode("ParamDec", "", @$.first_line);
                                  constructTree($$, 2, $1, $2);                 }
    ;

/* Statements */
CompSt : LC DefList StmtList RC     { printDebug2("CompSt -> LC DefList StmtList RC", @$.first_line);  
                                      $$ = createNode("CompSt", "", @$.first_line);
                                      constructTree($$, 4, $1, $2, $3, $4);     }
    ;

StmtList : Stmt StmtList    { printDebug2("StmtList -> Stmt StmtList", @$.first_line); 
                              $$ = createNode("StmtList", "", @$.first_line);
                              constructTree($$, 2, $1, $2);         }
    |   /*  empty   */      { printDebug2("StmtList -> Empty", @$.first_line); 
                              $$ = NULL;   }
    ;

Stmt : Exp SEMI     { printDebug2("Stmt -> Exp SEMI", @$.first_line);  
                      $$ = createNode("Stmt", "", @$.first_line);
                      constructTree($$, 2, $1, $2);     }
    | CompSt        { printDebug2("Stmt -> CompSt", @$.first_line);
                      $$ = createNode("Stmt", "", @$.first_line);
                      constructTree($$, 1, $1);                  }
    | RETURN Exp SEMI   { printDebug2("Stmt -> RETURN Exp SEMI", @$.first_line);
                          $$ = createNode("Stmt", "", @$.first_line);
                          constructTree($$, 3, $1, $2, $3);        }
    | IF LP Exp RP Stmt     { printDebug2("Stmt -> IF LP Exp RP Stmt", @$.first_line); 
                              $$ = createNode("Stmt", "", @$.first_line);
                              constructTree($$, 5, $1, $2, $3, $4, $5);     }
    | IF LP Exp RP Stmt ELSE Stmt   { printDebug2("Stmt -> IF LP Exp RP Stmt ELSE Stmt", @$.first_line);   
                                      $$ = createNode("Stmt","", @$.first_line);
                                      constructTree($$, 7, $1, $2, $3, $4, $5, $6, $7);      }
    | WHILE LP Exp RP Stmt      { printDebug2("Stmt -> WHILE LP Exp RP Stmt", @$.first_line);  
                                  $$ = createNode("Stmt", "", @$.first_line);
                                  constructTree($$, 5, $1, $2, $3, $4, $5);      }
    ;

/* Local Definitions */
DefList : Def DefList       { printDebug2("DefList -> Def DefList", @$.first_line);
                              $$ = createNode("DefList", "", @$.first_line);
                              constructTree($$, 2, $1, $2);          }
    |   /* empty */         { printDebug2("DefList -> Empty", @$.first_line);
                              $$ = NULL;  }
    ;

Def : Specifier DecList SEMI        { printDebug2("Def -> Specifier DecList SEMI", @$.first_line); 
                                      $$ = createNode("Def", "", @$.first_line);
                                      constructTree($$, 3, $1, $2, $3);       }
    ;

DecList : Dec       { printDebug2("DecList -> Dec", @$.first_line);    
                      $$ = createNode("DecList", "", @$.first_line);
                      constructTree($$, 1, $1);          }
    | Dec COMMA DecList     { printDebug2("DecList -> Dec COMMA DecList", @$.first_line);
                              $$ = createNode("DecList", "", @$.first_line);
                              constructTree($$, 3, $1, $2, $3);     }
    ;

Dec : VarDec        { printDebug2("Dec -> VarDec", @$.first_line); 
                      $$ = createNode("Dec", "", @$.first_line);
                      constructTree($$, 1, $1);              }
    | VarDec ASSIGNOP Exp       { printDebug2("Dec -> VarDec ASSIGNOP Exp", @$.first_line);
                                  $$ = createNode("Dec", "", @$.first_line);
                                  constructTree($$, 3, $1, $2, $3);              }
    ;

/* Expressions */
Exp : Exp ASSIGNOP Exp      { printDebug2("Exp -> Exp ASSIGNOP Exp", @$.first_line);
                              $$ = createNode("Exp", "", @$.first_line);    
                              constructTree($$, 3, $1, $2, $3);     }
    | Exp AND Exp           { printDebug2("Exp -> Exp AND Exp", @$.first_line);
                              $$ = createNode("Exp", "", @$.first_line);  
                              constructTree($$, 3, $1, $2, $3);     }
    | Exp OR Exp            { printDebug2("Exp -> Exp OR Exp", @$.first_line);
                              $$ = createNode("Exp", "", @$.first_line);
                              constructTree($$, 3, $1, $2, $3);     }
    | Exp RELOP Exp         { printDebug2("Exp -> Exp RELOP Exp", @$.first_line);
                              $$ = createNode("Exp", "", @$.first_line);
                              constructTree($$, 3, $1, $2, $3);     }
    | Exp PLUS Exp          { printDebug2("Exp -> Exp PLUS Exp", @$.first_line);
                              $$ = createNode("Exp", "", @$.first_line);
                              constructTree($$, 3, $1, $2, $3);     }
    | Exp MINUS Exp         { printDebug2("Exp -> Exp MINUS Exp", @$.first_line);
                              $$ = createNode("Exp", "", @$.first_line);
                              constructTree($$, 3, $1, $2, $3);     }
    | Exp STAR Exp          { printDebug2("Exp -> Exp STAR Exp", @$.first_line);
                              $$ = createNode("Exp", "", @$.first_line);
                              constructTree($$, 3, $1, $2, $3);     }
    | Exp DIV Exp           { printDebug2("Exp -> Exp DIV Exp", @$.first_line); 
                              $$ = createNode("Exp", "", @$.first_line);
                              constructTree($$, 3, $1, $2, $3);     }
    | LP Exp RP             { printDebug2("Exp -> LP Exp RP", @$.first_line);
                              $$ = createNode("Exp", "", @$.first_line);
                              constructTree($$, 3, $1, $2, $3);     }
    | MINUS Exp             { printDebug2("Exp -> MINUS Exp", @$.first_line);
                              $$ = createNode("Exp", "", @$.first_line);
                              constructTree($$, 2, $1, $2);     }
    | NOT Exp               { printDebug2("Exp -> NOT Exp", @$.first_line);
                              $$ = createNode("Exp", "", @$.first_line);
                              constructTree($$, 2, $1, $2);     }
    | ID LP Args RP         { printDebug2("Exp -> ID LP Args RP", @$.first_line);
                              $$ = createNode("Exp", "", @$.first_line);
                              constructTree($$, 4, $1, $2, $3, $4);     }
    | ID LP RP              { printDebug2("Exp -> ID LP RP", @$.first_line);
                              $$ = createNode("Exp", "", @$.first_line);
                              constructTree($$, 3, $1, $2, $3);     }
    | Exp LB Exp RB         { printDebug2("Exp -> Exp LB Exp RB", @$.first_line);
                              $$ = createNode("Exp", "", @$.first_line);
                              constructTree($$, 4, $1, $2, $3, $4);     }
    | Exp DOT ID            { printDebug2("Exp -> Exp DOT ID", @$.first_line);
                              $$ = createNode("Exp", "", @$.first_line);
                              constructTree($$, 3, $1, $2, $3);     }
    | ID                    { printDebug2("Exp -> ID", @$.first_line);
                              $$ = createNode("Exp", "", @$.first_line);
                              constructTree($$, 1, $1);     }
    | INT                   { printDebug2("Exp -> INT", @$.first_line);
                              $$ = createNode("Exp", "", @$.first_line);
                              constructTree($$, 1, $1);     }
    | FLOAT                 { printDebug2("Exp -> FLOAT", @$.first_line);
                              $$ = createNode("Exp", "", @$.first_line);
                              constructTree($$, 1, $1);     }
    | Exp ASSIGNOP error    { printDebug2("Exp -> Exp ASSIGNOP error", @$.first_line);
                              n_error++;
                              $$ = createNode("Error", "", @$.first_line);
                              printErrorTypeB("syntax error", @$.first_line);   }
    | Exp LB RB             { printDebug2("Exp -> Exp LB RB", @$.first_line);
                              n_error++;
                              $$ = createNode("Error", "", @$.first_line);
                              printErrorTypeB("Empty content inside \"[]\"", @$.first_line);   }
    | Exp LB error RB       { printDebug2("Exp -> Exp LB error RB", @$.first_line);
                              n_error++;
                              $$ = createNode("Error", "", @$.first_line);
                              printErrorTypeB("Syntax error inside \"[]\"", @$.first_line);
                              yyerrok;              }
    ;

Args : Exp COMMA Args       { printDebug2("Args -> Exp COMMA Args", @$.first_line);
                              $$ = createNode("Args", "", @$.first_line);
                              constructTree($$, 3, $1, $2, $3);     }
    | Exp                   { printDebug2("Args -> Exp", @$.first_line);
                              $$ = createNode("Args", "", @$.first_line);
                              constructTree($$, 1, $1);     }
    ;


%%

int main(int argc, char** argv){
    if(argc <= 1) return 1;
    FILE *f = fopen(argv[1], "r");
    if(!f){
        perror(argv[1]);
        return 1;
    }
    yyrestart(f);
    yyparse();
    if(n_error == 0) preOrderTraverse(root, 0);
    return 0;
}

int yyerror(char* str){
    /*  do nothing */
}
