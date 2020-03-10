%{
    #include "parseTree.h"
    #include "syntax.tab.h"
    // Bison location information
    int yycolumn = 1;
    #define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;

    #define NEW_NODE(token_name) \
        yylval.node = createNode(#token_name, yytext, yylloc.first_line)
    // Funciton to debug
    void printDebug(char* str){
        // printf("[%d] %s: %s\n", yylineno, str, yytext);
    }
    // Function to print error type A
    void printErrorTypeA(char* str){
        printf("Error type A at Line %d: Mysterious character \"%s\".\n", yylineno, str);
    }
%}
%option yylineno

DIGIT       [0-9]
INT         {DIGIT}+
FLOAT       {INT}\.{DIGIT}+
LETTER_     [a-zA-Z_]
ID          {LETTER_}({LETTER_}|{DIGIT})*
SEMI        ";"
COMMA       ","
ASSIGNOP    "="
RELOP       ">"|"<"|">="|"<="|"=="|"!="
PLUS        "+"
MINUS       "-"
STAR        "*"
DIV         "/"
AND         "&&"
OR          "||"
DOT         "."
NOT         "!"
TYPE        int|float
LP          "("
RP          ")"
LB          "["
RB          "]"
LC          "{"
RC          "}"
STRUCT      "struct"
RETURN      "return"
IF          "if"
ELSE        "else"
WHILE       "while"
MAIN        "main"
WHITESPACE  [ \t\r\n]+

%%

\n              {   yycolumn = 1; }

{INT}           {   printDebug("INT");
                    NEW_NODE(INT);
                    return INT;         }
{FLOAT}         {   printDebug("FLOAT");
                    NEW_NODE(FLOAT);
                    return FLOAT;       }
{SEMI}          {   printDebug("SEMI");
                    NEW_NODE(SEMI);
                    return SEMI;        }
{COMMA}         {   printDebug("COMMA");
                    NEW_NODE(COMMA);
                    return COMMA;       }
{ASSIGNOP}      {   printDebug("ASSIGNOP");
                    NEW_NODE(ASSIGNOP);
                    return ASSIGNOP;    }
{RELOP}         {   printDebug("RELOP");
                    NEW_NODE(RELOP);
                    return RELOP;       }
{PLUS}          {   printDebug("PLUS");
                    NEW_NODE(PLUS);
                    return PLUS;        }
{MINUS}         {   printDebug("MINUS");
                    NEW_NODE(MINUS);
                    return MINUS;       }
{STAR}          {   printDebug("STAR");
                    NEW_NODE(STAR);
                    return STAR;        }
{DIV}           {   printDebug("DIV");
                    NEW_NODE(DIV);
                    return DIV;         }
{AND}           {   printDebug("AND");
                    NEW_NODE(AND);
                    return AND;         }
{OR}            {   printDebug("OR");
                    NEW_NODE(OR);
                    return OR;          }
{DOT}           {   printDebug("DOT");
                    NEW_NODE(DOT);
                    return DOT;         }
{NOT}           {   printDebug("NOT");
                    NEW_NODE(NOT);
                    return NOT;         }
{TYPE}          {   printDebug("TYPE");
                    NEW_NODE(TYPE);
                    return TYPE;        }
{LP}            {   printDebug("LP");
                    NEW_NODE(LP);
                    return LP;          }
{RP}            {   printDebug("RP");
                    NEW_NODE(RP);
                    return RP;          }
{LB}            {   printDebug("LB");
                    NEW_NODE(LB);
                    return LB;          }
{RB}            {   printDebug("RB");
                    NEW_NODE(RB);
                    return RB;          }
{LC}            {   printDebug("LC");
                    NEW_NODE(LC);
                    return LC;          }
{RC}            {   printDebug("RC");
                    NEW_NODE(RC);
                    return RC;          }
{STRUCT}        {   printDebug("STRUCT");
                    NEW_NODE(STRUCT);
                    return STRUCT;      }
{RETURN}        {   printDebug("RETURN");
                    NEW_NODE(RETURN);
                    return RETURN;      }
{IF}            {   printDebug("IF");
                    NEW_NODE(IF);
                    return IF;          }
{ELSE}          {   printDebug("ELSE");
                    NEW_NODE(ELSE);
                    return ELSE;        }
{WHILE}         {   printDebug("WHILE");
                    NEW_NODE(WHILE);
                    return WHILE;       }
{ID}            {   printDebug("ID");
                    NEW_NODE(ID);
                    return ID;          }
{WHITESPACE}+   {   /* Do nothing */    }

.               {   printErrorTypeA(yytext);    }

%%





