%{
    #include <stdio.h>
    #include "lex.yy.c"
    void printDebug2(char* str){
        printf("%s\n", str);
    }
%}

/* Tokens */
%token INT FLOAT ID SEMI COMMA ASSIGNOP RELOP PLUS MINUS STAR DIV
%token AND OR DOT NOT TYPE LP RP LB RB LC RC
%token STRUCT RETURN IF ELSE WHILE

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
Program : ExtDefList        { printDebug2("Program -> ExtDefList");   }
    ;

ExtDefList : ExtDef ExtDefList  { printDebug2("ExtDefList -> ExtDef ExtDefList");    }
    |   /*  empty   */          { printDebug2("ExtDefList -> Empty");    }
    ;

ExtDef : Specifier ExtDecList SEMI  { printDebug2("ExtDef -> Specifier ExtDecList SEMI");  }
    | Specifier SEMI                { printDebug2("ExtDef -> Specifier SEMI");  }
    | Specifier FunDec CompSt       { printDebug2("ExtDef -> Specifier FuncDec CompSt");  }
    ;

ExtDecList : VarDec     { printDebug2("ExtDecList -> VarDec");  }
    | VarDec COMMA ExtDecList   { printDebug2("ExtDecList -> VarDec COMMA ExtDecList"); }
    ;

/* Specifiers */
Specifier : TYPE    { printDebug2("Specifier -> TYPE"); }
    | StructSpecifier   { printDebug2("Specifier -> StructSpecifier");  }
    ;

StructSpecifier : STRUCT OptTag LC DefList RC   { printDebug2("StructSpecifier -> STRUCT OptTag LC DefList RC");    }
    | STRUCT Tag    { printDebug2("StructSpecifier -> STRUCT Tag"); }
    ;

OptTag : ID     { printDebug2("OptTag -> ID");  }
    |   /*  empty  */   { printDebug2("OptTag -> Empty");   }
    ;

Tag : ID    { printDebug2("Tag -> ID"); }
    ;

/* Declarations */
VarDec : ID    { printDebug2("VarDec -> ID");   }
    | VarDec LB INT RB  { printDebug2("VarDec -> VarDec LB INT RB");    }
    ;

FunDec : ID LP VarList RP   { printDebug2("FunDec -> ID LP VarList RP");    }
    | ID LP RP  { printDebug2("FunDec -> ID LP RP");    }
    ;

VarList : ParamDec COMMA VarList    { printDebug2("VarList -> ParamDec COMMA VarList"); }
    | ParamDec  { printDebug2("VarList -> ParamDec");   }
    ;

ParamDec : Specifier VarDec     { printDebug2("ParamDec -> Specifier VarDec");  }
    ;

/* Statements */
CompSt : LC DefList StmtList RC     { printDebug2("CompSt -> LC DefList StmtList RC");  }
    ;

StmtList : Stmt StmtList    { printDebug2("StmtList -> Stmt StmtList"); }
    |   /*  empty   */      { printDebug2("StmtList -> Empty"); }
    ;

Stmt : Exp SEMI     { printDebug2("Stmt -> Exp SEMI");  }
    | CompSt        { printDebug2("Stmt -> CompSt");    }
    | RETURN Exp SEMI   { printDebug2("Stmt -> RETURN Exp SEMI");   }
    | IF LP Exp RP Stmt     { printDebug2("Stmt -> IF LP Exp RP Stmt"); }
    | IF LP Exp RP Stmt ELSE Stmt   { printDebug2("Stmt -> IF LP Exp RP Stmt ELSE Stmt");   }
    | WHILE LP Exp RP Stmt      { printDebug2("Stmt -> WHILE LP Exp RP Stmt");  }
    ;

/* Local Definitions */
DefList : Def DefList       { printDebug2("DefList -> Def DefList");    }
    |   /* empty */         { printDebug2("DefList -> Empty");  }
    ;

Def : Specifier DecList SEMI        { printDebug2("Def -> Specifier DecList SEMI"); }
    ;

DecList : Dec       { printDebug2("DecList -> Dec");    }
    | Dec COMMA DecList     { printDebug2("DecList -> Dec COMMA DecList");  }
    ;

Dec : VarDec        { printDebug2("Dec -> VarDec"); }
    | VarDec ASSIGNOP Exp       { printDebug2("Dec -> VarDec ASSIGNOP Exp");    }
    ;

/* Expressions */
Exp : Exp ASSIGNOP Exp      { printDebug2("Exp -> Exp ASSIGNOP Exp");   }
    | Exp AND Exp           { printDebug2("Exp -> Exp AND Exp");        }
    | Exp OR Exp            { printDebug2("Exp -> Exp OR Exp");         }
    | Exp RELOP Exp         { printDebug2("Exp -> Exp RELOP Exp");      }
    | Exp PLUS Exp          { printDebug2("Exp -> Exp PLUS Exp");       }
    | Exp MINUS Exp         { printDebug2("Exp -> Exp MINUS Exp");      }
    | Exp STAR Exp          { printDebug2("Exp -> Exp STAR Exp");       }
    | Exp DIV Exp           { printDebug2("Exp -> Exp DIV Exp");        }
    | LP Exp RP             { printDebug2("Exp -> LP Exp RP");          }
    | MINUS Exp             { printDebug2("Exp -> MINUS Exp");          }
    | NOT Exp               { printDebug2("Exp -> NOT Exp");            }
    | ID LP Args RP         { printDebug2("Exp -> ID LP Args RP");      }
    | ID LP RP              { printDebug2("Exp -> ID LP RP");           }
    | Exp LB Exp RB         { printDebug2("Exp -> Exp LB Exp RB");      }
    | Exp DOT ID            { printDebug2("Exp -> Exp DOT ID");         }
    | ID                    { printDebug2("Exp -> ID");                 }
    | INT                   { printDebug2("Exp -> INT");                }
    | FLOAT                 { printDebug2("Exp -> FLOAT");              }
    ;

Args : Exp COMMA Args       { printDebug2("Args -> Exp COMMA Args");    }
    | Exp                   { printDebug2("Args -> Exp");               }
    ;









