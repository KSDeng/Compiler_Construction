%{
    #include "parseTree.h"
    #include "semantic.h"
    #include "intercodegen.h"
    #include "objectcodegen.h"
    #include "lex.yy.c"
    Node* root;
    extern int np, nb, nc;
    extern int line_p, line_b, line_c;
    extern int n_error;    // number of errors
    extern bool debug;
    void printDebug2(char* str, int lineno){
        if(debug) printf("%s (%d)\n",str, lineno);
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
    | Specifier error
                            {
                              printErrorTypeB("Missing \";\"", @$.last_line);
                              n_error++;
                              yyerrok;
                            }
    | Specifier ExtDecList error
                                    { printDebug2("ExtDef -> Specifier ExtDecList error", @$.first_line);
                                      printErrorTypeB("Missing \";\"", @$.first_line);
                                      n_error++;
                                      yyerrok;
                                    }
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
    | error Tag
                    { printDebug2("StructSpecifier -> error Tag", @$.first_line);
                      printErrorTypeB("Syntax error, unexpected ID", @$.last_line);
                      n_error++;
                    }
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
    | VarDec LB FLOAT RB
                        {
                          printDebug2("VarDec -> VarDec LB FLOAT RB", @$.first_line);
                          printErrorTypeB("Unexpected float, expecting int", @$.first_line);
                          n_error++;
                        }
    | VarDec LB error RB    {
                                printErrorTypeB("Syntax error between []", @$.first_line);
                                n_error++;
                            }
    ;

FunDec : ID LP VarList RP   { printDebug2("FunDec -> ID LP VarList RP", @$.first_line);
                              $$ = createNode("FunDec", "", @$.first_line);
                              constructTree($$, 4, $1, $2, $3, $4);          }
    | ID LP RP  { printDebug2("FunDec -> ID LP RP", @$.first_line);    
                  $$ = createNode("FunDec", "", @$.first_line);
                  constructTree($$, 3, $1, $2, $3);     }
    | INVALID_ID LP RP      { 
                              n_error++;
                              printErrorTypeB("Illegal function ID", @$.first_line);                  
                            }
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
    | RETURN error SEMI     {
                              printDebug2("Stmt -> RETURN error SEMI", @$.first_line);
                              printErrorTypeB("Return expression syntax error", @$.first_line);
                              n_error++;
                            }
    | RETURN Exp error      
                            {
                              printErrorTypeB("Missing \";\"", @$.first_line);
                              n_error++;
                              yyerrok;
                            }
    | IF LP Exp RP error ELSE Stmt
                            {
                                printDebug2("Stmt -> IF LP Exp RP error ELSE Stmt", @$.first_line);
                                printErrorTypeB("Syntax error", @$.first_line);
                                n_error++;
                            }
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
    | Specifier error SEMI          {
                                      printDebug2("Def -> Specifier error SEMI", @$.first_line);
                                      printErrorTypeB("Syntax error when defining variable", @$.first_line);
                                      n_error++;
                                                    }
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
    | Exp LB error RB       {
                              printErrorTypeB("Syntax error between []", @$.first_line);
                              n_error++;
                            }
    | Exp PLUS PLUS         
                            { printErrorTypeB("Illegal expression", @$.first_line);
                              n_error++;
                            }
    | Exp LB ID COMMA ID RB
                            { printDebug2("Exp -> Exp LB ID COMMA ID RB", @$.first_line);
                              printErrorTypeB("Syntax error between []", @$.first_line);
                              n_error++;
                            }
    | LB Exp RP             
                            { printDebug2("Exp -> LB Exp RP", @$.first_line);
                              printErrorTypeB("Syntax error, parentheses don't match", @$.first_line);
                              n_error++;
                            }
    | INVALID_ID
                            { printDebug2("Exp -> INVALID_ID", @$.first_line);
                              printErrorTypeB("Illegal ID", @$.first_line);
                              n_error++;
                            }
    | Exp RELOP error Exp   
                            { printDebug2("Exp -> Exp RELOP error Exp", @$.first_line);
                              printErrorTypeB("Unexpected RELOP", @$.first_line);
                              n_error++;
                            }
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

    // if(debug) printf("np = %d  nb = %d  nc = %d\n", np, nb, nc);
    if(np > 0) printErrorTypeB("Parentheses not match, missing \")\"", line_p);
    if(nb > 0) printErrorTypeB("Brackets not match, missing \"]\"", line_b);
    if(nc > 0) printErrorTypeB("Brackets not match, missing \"}\"", line_c);

    //if(debug)
    //    preOrderTraverse(root, 0);
    //else{
        if(n_error == 0) {
            // preOrderTraverse(root, 0);
            Program(root);
            translate_Program(root);
            if(argc == 2) {
                //writeInterCode("stdout");
                writeAssemblyCode("stdout");
            }else if(argc == 3) {
                //writeInterCode(argv[2]);
                //writeAssemblyCode("stdout");
                writeAssemblyCode(argv[2]);
            }
            /*
            else if(argc == 4){
                writeInterCode(argv[2]);
                writeAssemblyCode(argv[3]);
            }else printf("Wrong number of params for main().\n");
            */
        }
    //}
    return 0;
}

int yyerror(char* str){
    /* do nothing */
}
