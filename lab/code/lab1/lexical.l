%{
    #include <stdio.h>
    #include "syntax.tab.h"

    // Bison location information
    int yycolumn = 1;
    #define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;

    // Funciton to debug
    void printDebug(char* str){
        printf("[%d] %s: %s\n", yylineno, str, yytext);
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
                    return INT;         }
{FLOAT}         {   printDebug("FLOAT");
                    return FLOAT;       }
{SEMI}          {   printDebug("SEMI");
                    return SEMI;        }
{COMMA}         {   printDebug("COMMA");
                    return COMMA;       }
{ASSIGNOP}      {   printDebug("ASSIGNOP");
                    return ASSIGNOP;    }
{RELOP}         {   printDebug("RELOP");
                    return RELOP;       }
{PLUS}          {   printDebug("PLUS");
                    return PLUS;        }
{MINUS}         {   printDebug("MINUS");
                    return MINUS;       }
{STAR}          {   printDebug("STAR");
                    return STAR;        }
{DIV}           {   printDebug("DIV");
                    return DIV;         }
{AND}           {   printDebug("AND");
                    return AND;         }
{OR}            {   printDebug("OR");
                    return OR;          }
{DOT}           {   printDebug("DOT");
                    return DOT;         }
{NOT}           {   printDebug("NOT");
                    return NOT;         }
{TYPE}          {   printDebug("TYPE");
                    return TYPE;        }
{LP}            {   printDebug("LP");
                    return LP;          }
{RP}            {   printDebug("RP");
                    return RP;          }
{LB}            {   printDebug("LB");
                    return LB;          }
{RB}            {   printDebug("RB");
                    return RB;          }
{LC}            {   printDebug("LC");
                    return LC;          }
{RC}            {   printDebug("RC");
                    return RC;          }
{STRUCT}        {   printDebug("STRUCT");
                    return STRUCT;      }
{RETURN}        {   printDebug("RETURN");
                    return RETURN;      }
{IF}            {   printDebug("IF");
                    return IF;          }
{ELSE}          {   printDebug("ELSE");
                    return ELSE;        }
{WHILE}         {   printDebug("WHILE");
                    return WHILE;       }
{ID}            {   printDebug("ID");
                    return ID;          }
{WHITESPACE}+   {   /* Do nothing */    }

.               {   printErrorTypeA(yytext);    }

%%





