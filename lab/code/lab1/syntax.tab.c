/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "./syntax.y" /* yacc.c:339  */

    #include "parseTree.h"
    #include "lex.yy.c"
    Node* root;
    extern int np, nb, nc;
    extern int line_p, line_b, line_c;
    extern int n_error;    // number of errors
    extern bool debug;
    void printDebug2(char* str, int lineno){
        if(debug) printf("%s (%d)\n",str, lineno);
    }

#line 79 "./syntax.tab.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "syntax.tab.h".  */
#ifndef YY_YY_SYNTAX_TAB_H_INCLUDED
# define YY_YY_SYNTAX_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    INT = 258,
    FLOAT = 259,
    ID = 260,
    SEMI = 261,
    COMMA = 262,
    ASSIGNOP = 263,
    RELOP = 264,
    PLUS = 265,
    MINUS = 266,
    STAR = 267,
    DIV = 268,
    AND = 269,
    OR = 270,
    DOT = 271,
    NOT = 272,
    TYPE = 273,
    LP = 274,
    RP = 275,
    LB = 276,
    RB = 277,
    LC = 278,
    RC = 279,
    STRUCT = 280,
    RETURN = 281,
    IF = 282,
    ELSE = 283,
    WHILE = 284,
    INVALID_ID = 285
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 15 "./syntax.y" /* yacc.c:355  */

    int type_int;
    float type_float;
    char* type_str;
    Node* type_node;

#line 157 "./syntax.tab.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;
int yyparse (void);

#endif /* !YY_YY_SYNTAX_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 188 "./syntax.tab.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE) + sizeof (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   386

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  31
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  75
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  144

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   285

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    53,    53,    59,    63,    67,    70,    73,    76,    82,
      90,    93,    99,   102,   107,   110,   113,   120,   123,   127,
     133,   136,   139,   145,   151,   154,   157,   163,   166,   171,
     177,   182,   185,   189,   192,   195,   198,   201,   204,   207,
     212,   218,   227,   230,   234,   237,   244,   247,   252,   255,
     261,   264,   267,   270,   273,   276,   279,   282,   285,   288,
     291,   294,   297,   300,   303,   306,   309,   312,   315,   319,
     323,   328,   333,   338,   345,   348
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "INT", "FLOAT", "ID", "SEMI", "COMMA",
  "ASSIGNOP", "RELOP", "PLUS", "MINUS", "STAR", "DIV", "AND", "OR", "DOT",
  "NOT", "TYPE", "LP", "RP", "LB", "RB", "LC", "RC", "STRUCT", "RETURN",
  "IF", "ELSE", "WHILE", "INVALID_ID", "$accept", "Program", "ExtDefList",
  "ExtDef", "ExtDecList", "Specifier", "StructSpecifier", "OptTag", "Tag",
  "VarDec", "FunDec", "VarList", "ParamDec", "CompSt", "StmtList", "Stmt",
  "DefList", "Def", "DecList", "Dec", "Exp", "Args", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285
};
# endif

#define YYPACT_NINF -62

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-62)))

#define YYTABLE_NINF -44

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      22,     5,   -62,     7,    37,   -62,    22,    13,   -62,   -62,
     -62,     3,    23,   -62,   -62,   -62,   -62,    54,   -62,    56,
      47,    -1,    58,    26,    41,    25,   -62,   -62,    86,    59,
      53,   -62,    88,    77,    53,   -62,    86,    78,    98,   -62,
     -62,   -62,    89,    93,   111,   146,   114,    20,   132,   133,
     -62,   -62,   120,   -62,    67,   -62,   -62,   -62,   -62,   -62,
     128,   202,   202,   202,   202,   113,   134,   135,   -62,   -62,
     131,   146,   239,   -62,   202,   -62,    86,   -62,   174,    -5,
      74,   283,   297,   150,   225,   202,   202,   -62,   -62,   -62,
     202,   118,   179,   202,   202,   202,   202,   202,   154,   141,
     339,   -62,   -62,   254,   144,   -62,   -62,   -62,   -62,   -62,
     311,   325,   339,   202,   115,   -62,    -5,    -5,    74,    74,
     365,   352,   -62,   139,    48,   268,   202,   -62,    83,   146,
     339,   -62,   161,   -62,   -62,   140,   142,   -62,   152,   146,
     146,   -62,   -62,   -62
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,     0,    12,    18,     0,     2,     0,     0,    13,    19,
      16,    19,     0,    15,     1,     3,     8,    20,     6,     0,
       0,    10,     0,     0,     0,     0,     9,     5,     0,     0,
       0,     7,     0,     0,     0,    25,     0,     0,    28,    26,
      20,    11,     0,     0,     0,    32,     0,    48,     0,    46,
      14,    42,    29,    24,     0,    23,    21,    22,    66,    67,
      65,     0,     0,     0,     0,     0,     0,     0,    72,    34,
       0,    32,     0,    45,     0,    44,     0,    27,     0,    59,
      60,     0,     0,     0,     0,     0,     0,    30,    31,    33,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      49,    47,    62,    75,     0,    58,    71,    39,    40,    35,
       0,     0,    50,     0,    53,    69,    54,    55,    56,    57,
      51,    52,    64,     0,    65,     0,     0,    61,     0,     0,
      73,    68,     0,    63,    74,     0,    36,    38,     0,     0,
       0,    70,    41,    37
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -62,   -62,   175,   -62,   158,    15,   -62,   -62,   177,   -27,
     -62,   138,   -62,   165,   117,   -32,    69,   -62,   121,   -62,
     -61,    73
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     4,     5,     6,    20,    32,     8,    12,    10,    21,
      22,    37,    38,    69,    70,    71,    33,    34,    48,    49,
      72,   104
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      79,    80,    81,    82,    84,    47,    28,    94,    95,    52,
       9,    98,    11,   100,    16,     7,    99,   103,    17,    18,
      29,     7,    -4,     1,   110,   111,   -17,     1,    74,   112,
     114,   116,   117,   118,   119,   120,   121,    14,   125,    36,
       2,    29,     1,    19,     2,    39,    23,     3,    26,    47,
     -43,     3,   130,    27,     1,   132,   -43,   -43,   -43,     2,
      42,    35,    43,    44,   -43,   103,     3,    78,     1,    36,
     -43,     2,   -43,    24,   -43,    25,   -43,   -43,     3,   -43,
     -43,    30,   -43,   -43,   135,     2,    58,    59,    60,    46,
      98,    40,     3,    40,    61,    99,   136,   137,    53,    45,
      62,    50,    63,    51,    64,    54,    30,   142,   143,    65,
      66,    55,    67,    68,    83,    56,    58,    59,    60,   113,
      73,    58,    59,    60,    61,    92,    93,    94,    95,    61,
      62,    98,    63,    57,    64,    62,    99,    63,    75,    64,
      76,    29,   123,    68,    58,    59,   124,    78,    68,    58,
      59,    60,    61,    85,    86,    87,   107,    61,    62,   122,
      63,   131,    64,    62,   127,    63,   138,    64,   139,    30,
     140,    68,    65,    66,   141,    67,    68,    58,    59,    60,
      13,    15,    58,    59,    60,    61,    41,    31,    88,   115,
      61,    62,    77,    63,   102,    64,    62,   101,    63,   134,
      64,     0,     0,     0,    68,    58,    59,    60,     0,    68,
       0,     0,     0,    61,     0,     0,     0,     0,     0,    62,
       0,    63,     0,    64,     0,     0,   108,     0,     0,     0,
       0,   109,    68,    90,    91,    92,    93,    94,    95,    96,
      97,    98,     0,     0,     0,    89,    99,    90,    91,    92,
      93,    94,    95,    96,    97,    98,     0,     0,     0,     0,
      99,   126,    90,    91,    92,    93,    94,    95,    96,    97,
      98,     0,     0,     0,     0,    99,    90,    91,    92,    93,
      94,    95,    96,    97,    98,     0,     0,     0,     0,    99,
     133,    90,    91,    92,    93,    94,    95,    96,    97,    98,
       0,     0,     0,   105,    99,    90,    91,    92,    93,    94,
      95,    96,    97,    98,     0,     0,     0,   106,    99,    90,
      91,    92,    93,    94,    95,    96,    97,    98,     0,     0,
       0,   128,    99,    90,    91,    92,    93,    94,    95,    96,
      97,    98,     0,     0,     0,   129,    99,    90,    91,    92,
      93,    94,    95,    96,    97,    98,     0,     0,     0,     0,
      99,    91,    92,    93,    94,    95,    96,     0,    98,     0,
       0,     0,     0,    99,    91,    92,    93,    94,    95,     0,
       0,    98,     0,     0,     0,     0,    99
};

static const yytype_int16 yycheck[] =
{
      61,    62,    63,    64,    65,    32,     7,    12,    13,    36,
       5,    16,     5,    74,     1,     0,    21,    78,     5,     6,
      21,     6,     0,     1,    85,    86,    23,     1,     8,    90,
      91,    92,    93,    94,    95,    96,    97,     0,    99,    24,
      18,    21,     1,    30,    18,    20,    23,    25,     1,    76,
      24,    25,   113,     6,     1,     7,     3,     4,     5,    18,
       1,    20,     3,     4,    11,   126,    25,    19,     1,    54,
      17,    18,    19,    19,    21,    19,    23,    24,    25,    26,
      27,    23,    29,    30,     1,    18,     3,     4,     5,     1,
      16,     5,    25,     5,    11,    21,   128,   129,    20,    30,
      17,    24,    19,    34,    21,     7,    23,   139,   140,    26,
      27,    22,    29,    30,     1,    22,     3,     4,     5,     1,
       6,     3,     4,     5,    11,    10,    11,    12,    13,    11,
      17,    16,    19,    22,    21,    17,    21,    19,     6,    21,
       7,    21,     1,    30,     3,     4,     5,    19,    30,     3,
       4,     5,    11,    19,    19,    24,     6,    11,    17,     5,
      19,    22,    21,    17,    20,    19,     5,    21,    28,    23,
      28,    30,    26,    27,    22,    29,    30,     3,     4,     5,
       3,     6,     3,     4,     5,    11,    28,    22,    71,    10,
      11,    17,    54,    19,    20,    21,    17,    76,    19,   126,
      21,    -1,    -1,    -1,    30,     3,     4,     5,    -1,    30,
      -1,    -1,    -1,    11,    -1,    -1,    -1,    -1,    -1,    17,
      -1,    19,    -1,    21,    -1,    -1,     1,    -1,    -1,    -1,
      -1,     6,    30,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    -1,    -1,    -1,     6,    21,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,
      21,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    -1,    -1,    -1,    -1,    21,     8,     9,    10,    11,
      12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,    21,
      22,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      -1,    -1,    -1,    20,    21,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    -1,    -1,    -1,    20,    21,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    -1,    -1,
      -1,    20,    21,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    -1,    -1,    -1,    20,    21,     8,     9,    10,
      11,    12,    13,    14,    15,    16,    -1,    -1,    -1,    -1,
      21,     9,    10,    11,    12,    13,    14,    -1,    16,    -1,
      -1,    -1,    -1,    21,     9,    10,    11,    12,    13,    -1,
      -1,    16,    -1,    -1,    -1,    -1,    21
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,    18,    25,    32,    33,    34,    36,    37,     5,
      39,     5,    38,    39,     0,    33,     1,     5,     6,    30,
      35,    40,    41,    23,    19,    19,     1,     6,     7,    21,
      23,    44,    36,    47,    48,    20,    36,    42,    43,    20,
       5,    35,     1,     3,     4,    47,     1,    40,    49,    50,
      24,    47,    40,    20,     7,    22,    22,    22,     3,     4,
       5,    11,    17,    19,    21,    26,    27,    29,    30,    44,
      45,    46,    51,     6,     8,     6,     7,    42,    19,    51,
      51,    51,    51,     1,    51,    19,    19,    24,    45,     6,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    21,
      51,    49,    20,    51,    52,    20,    20,     6,     1,     6,
      51,    51,    51,     1,    51,    10,    51,    51,    51,    51,
      51,    51,     5,     1,     5,    51,     7,    20,    20,    20,
      51,    22,     7,    22,    52,     1,    46,    46,     5,    28,
      28,    22,    46,    46
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    31,    32,    33,    33,    34,    34,    34,    34,    34,
      35,    35,    36,    36,    37,    37,    37,    38,    38,    39,
      40,    40,    40,    40,    41,    41,    41,    42,    42,    43,
      44,    45,    45,    46,    46,    46,    46,    46,    46,    46,
      46,    46,    47,    47,    48,    48,    49,    49,    50,    50,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    51,    51,    51,    51,    51,    51,
      51,    51,    51,    51,    52,    52
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     0,     3,     2,     3,     2,     3,
       1,     3,     1,     1,     5,     2,     2,     1,     0,     1,
       1,     4,     4,     4,     4,     3,     3,     3,     1,     2,
       4,     2,     0,     2,     1,     3,     5,     7,     5,     3,
       3,     7,     2,     0,     3,     3,     1,     3,     1,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     4,     3,     4,     3,     1,     1,     1,     4,     3,
       6,     3,     1,     4,     3,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static unsigned
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  unsigned res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#  define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  YY_LOCATION_PRINT (yyoutput, *yylocationp);
  YYFPRINTF (yyoutput, ": ");
  yy_symbol_value_print (yyoutput, yytype, yyvaluep, yylocationp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                       , &(yylsp[(yyi + 1) - (yynrhs)])                       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.
       'yyls': related to locations.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    /* The location stack.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls;
    YYLTYPE *yylsp;

    /* The locations where the error started and ended.  */
    YYLTYPE yyerror_range[3];

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yylsp = yyls = yylsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yyls1, yysize * sizeof (*yylsp),
                    &yystacksize);

        yyls = yyls1;
        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location.  */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 53 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Program -> ExtDefList", (yyloc).first_line);
                              (yyval.type_node) = createNode("Program", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 1, (yyvsp[0].type_node));
                              root = (yyval.type_node);                                }
#line 1505 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 59 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("ExtDefList -> ExtDef ExtDefList", (yyloc).first_line);
                                  (yyval.type_node) = createNode("ExtDefList", "", (yyloc).first_line);
                                  constructTree((yyval.type_node), 2, (yyvsp[-1].type_node), (yyvsp[0].type_node));         }
#line 1513 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 63 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("ExtDefList -> Empty", (yyloc).first_line);    
                                  (yyval.type_node) = NULL;     }
#line 1520 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 67 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("ExtDef -> Specifier ExtDecList SEMI", (yyloc).first_line);
                                      (yyval.type_node) = createNode("ExtDef", "", (yyloc).first_line);
                                      constructTree((yyval.type_node), 3, (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));                 }
#line 1528 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 70 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("ExtDef -> Specifier SEMI", (yyloc).first_line);  
                                      (yyval.type_node) = createNode("ExtDef", "", (yyloc).first_line);
                                      constructTree((yyval.type_node), 2, (yyvsp[-1].type_node), (yyvsp[0].type_node));                     }
#line 1536 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 73 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("ExtDef -> Specifier FuncDec CompSt", (yyloc).first_line);
                                      (yyval.type_node) = createNode("ExtDef", "", (yyloc).first_line);
                                      constructTree((yyval.type_node), 3, (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));                 }
#line 1544 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 77 "./syntax.y" /* yacc.c:1646  */
    {
                              printErrorTypeB("Missing \";\"", (yyloc).last_line);
                              n_error++;
                              yyerrok;
                            }
#line 1554 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 83 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("ExtDef -> Specifier ExtDecList error", (yyloc).first_line);
                                      printErrorTypeB("Missing \";\"", (yyloc).first_line);
                                      n_error++;
                                      yyerrok;
                                    }
#line 1564 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 90 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("ExtDecList -> VarDec", (yyloc).first_line);  
                          (yyval.type_node) = createNode("ExtDecList", "", (yyloc).first_line);
                          constructTree((yyval.type_node), 1, (yyvsp[0].type_node));  }
#line 1572 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 93 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("ExtDecList -> VarDec COMMA ExtDecList", (yyloc).first_line);
                                  (yyval.type_node) = createNode("ExtDecList", "",  (yyloc).first_line);
                                  constructTree((yyval.type_node), 3, (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));   }
#line 1580 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 99 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Specifier -> TYPE", (yyloc).first_line); 
                      (yyval.type_node) = createNode("Specifier", "", (yyloc).first_line);
                      constructTree((yyval.type_node), 1, (yyvsp[0].type_node));     }
#line 1588 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 102 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Specifier -> StructSpecifier", (yyloc).first_line);
                          (yyval.type_node) = createNode("Specifier", "", (yyloc).first_line);
                          constructTree((yyval.type_node), 1, (yyvsp[0].type_node));     }
#line 1596 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 107 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("StructSpecifier -> STRUCT OptTag LC DefList RC", (yyloc).first_line);   
                                                  (yyval.type_node) = createNode("StructSpecifier", "", (yyloc).first_line);
                                                  constructTree((yyval.type_node), 5, (yyvsp[-4].type_node), (yyvsp[-3].type_node), (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 1604 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 110 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("StructSpecifier -> STRUCT Tag", (yyloc).first_line); 
                      (yyval.type_node) = createNode("StructSpecifier", "", (yyloc).first_line);
                      constructTree((yyval.type_node), 2, (yyvsp[-1].type_node), (yyvsp[0].type_node));  }
#line 1612 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 114 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("StructSpecifier -> error Tag", (yyloc).first_line);
                      printErrorTypeB("Syntax error, unexpected ID", (yyloc).last_line);
                      n_error++;
                    }
#line 1621 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 120 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("OptTag -> ID", (yyloc).first_line);
                  (yyval.type_node) = createNode("OptTag", "", (yyloc).first_line);
                  constructTree((yyval.type_node), 1, (yyvsp[0].type_node));         }
#line 1629 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 123 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("OptTag -> Empty", (yyloc).first_line);
                          (yyval.type_node) = NULL;     }
#line 1636 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 127 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Tag -> ID", (yyloc).first_line); 
              (yyval.type_node) = createNode("Tag", "", (yyloc).first_line);
              constructTree((yyval.type_node), 1, (yyvsp[0].type_node));     }
#line 1644 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 133 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("VarDec -> ID", (yyloc).first_line);
                 (yyval.type_node) = createNode("VarDec", "", (yyloc).first_line);
                 constructTree((yyval.type_node), 1, (yyvsp[0].type_node));  }
#line 1652 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 136 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("VarDec -> VarDec LB INT RB", (yyloc).first_line);    
                          (yyval.type_node) = createNode("VarDec", "", (yyloc).first_line);  
                          constructTree((yyval.type_node), 4, (yyvsp[-3].type_node), (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 1660 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 140 "./syntax.y" /* yacc.c:1646  */
    {
                          printDebug2("VarDec -> VarDec LB FLOAT RB", (yyloc).first_line);
                          printErrorTypeB("Unexpected float, expecting int", (yyloc).first_line);
                          n_error++;
                        }
#line 1670 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 145 "./syntax.y" /* yacc.c:1646  */
    {
                                printErrorTypeB("Syntax error between []", (yyloc).first_line);
                                n_error++;
                            }
#line 1679 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 151 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("FunDec -> ID LP VarList RP", (yyloc).first_line);
                              (yyval.type_node) = createNode("FunDec", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 4, (yyvsp[-3].type_node), (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));          }
#line 1687 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 154 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("FunDec -> ID LP RP", (yyloc).first_line);    
                  (yyval.type_node) = createNode("FunDec", "", (yyloc).first_line);
                  constructTree((yyval.type_node), 3, (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 1695 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 157 "./syntax.y" /* yacc.c:1646  */
    { 
                              n_error++;
                              printErrorTypeB("Illegal function ID", (yyloc).first_line);                  
                            }
#line 1704 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 163 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("VarList -> ParamDec COMMA VarList", (yyloc).first_line);
                                      (yyval.type_node) = createNode("VarList", "", (yyloc).first_line);
                                      constructTree((yyval.type_node), 3, (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));         }
#line 1712 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 166 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("VarList -> ParamDec", (yyloc).first_line);
                  (yyval.type_node) = createNode("VarList", "", (yyloc).first_line);
                  constructTree((yyval.type_node), 1, (yyvsp[0].type_node));         }
#line 1720 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 171 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("ParamDec -> Specifier VarDec", (yyloc).first_line);
                                  (yyval.type_node) = createNode("ParamDec", "", (yyloc).first_line);
                                  constructTree((yyval.type_node), 2, (yyvsp[-1].type_node), (yyvsp[0].type_node));                 }
#line 1728 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 177 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("CompSt -> LC DefList StmtList RC", (yyloc).first_line);  
                                      (yyval.type_node) = createNode("CompSt", "", (yyloc).first_line);
                                      constructTree((yyval.type_node), 4, (yyvsp[-3].type_node), (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 1736 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 182 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("StmtList -> Stmt StmtList", (yyloc).first_line); 
                              (yyval.type_node) = createNode("StmtList", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 2, (yyvsp[-1].type_node), (yyvsp[0].type_node));         }
#line 1744 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 185 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("StmtList -> Empty", (yyloc).first_line); 
                              (yyval.type_node) = NULL;   }
#line 1751 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 189 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Stmt -> Exp SEMI", (yyloc).first_line);  
                      (yyval.type_node) = createNode("Stmt", "", (yyloc).first_line);
                      constructTree((yyval.type_node), 2, (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 1759 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 192 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Stmt -> CompSt", (yyloc).first_line);
                      (yyval.type_node) = createNode("Stmt", "", (yyloc).first_line);
                      constructTree((yyval.type_node), 1, (yyvsp[0].type_node));                  }
#line 1767 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 195 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Stmt -> RETURN Exp SEMI", (yyloc).first_line);
                          (yyval.type_node) = createNode("Stmt", "", (yyloc).first_line);
                          constructTree((yyval.type_node), 3, (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));        }
#line 1775 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 198 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Stmt -> IF LP Exp RP Stmt", (yyloc).first_line); 
                              (yyval.type_node) = createNode("Stmt", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 5, (yyvsp[-4].type_node), (yyvsp[-3].type_node), (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 1783 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 201 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Stmt -> IF LP Exp RP Stmt ELSE Stmt", (yyloc).first_line);   
                                      (yyval.type_node) = createNode("Stmt","", (yyloc).first_line);
                                      constructTree((yyval.type_node), 7, (yyvsp[-6].type_node), (yyvsp[-5].type_node), (yyvsp[-4].type_node), (yyvsp[-3].type_node), (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));      }
#line 1791 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 204 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Stmt -> WHILE LP Exp RP Stmt", (yyloc).first_line);  
                                  (yyval.type_node) = createNode("Stmt", "", (yyloc).first_line);
                                  constructTree((yyval.type_node), 5, (yyvsp[-4].type_node), (yyvsp[-3].type_node), (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));      }
#line 1799 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 207 "./syntax.y" /* yacc.c:1646  */
    {
                              printDebug2("Stmt -> RETURN error SEMI", (yyloc).first_line);
                              printErrorTypeB("Return expression syntax error", (yyloc).first_line);
                              n_error++;
                            }
#line 1809 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 213 "./syntax.y" /* yacc.c:1646  */
    {
                              printErrorTypeB("Missing \";\"", (yyloc).first_line);
                              n_error++;
                              yyerrok;
                            }
#line 1819 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 219 "./syntax.y" /* yacc.c:1646  */
    {
                                printDebug2("Stmt -> IF LP Exp RP error ELSE Stmt", (yyloc).first_line);
                                printErrorTypeB("Syntax error", (yyloc).first_line);
                                n_error++;
                            }
#line 1829 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 227 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("DefList -> Def DefList", (yyloc).first_line);
                              (yyval.type_node) = createNode("DefList", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 2, (yyvsp[-1].type_node), (yyvsp[0].type_node));          }
#line 1837 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 230 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("DefList -> Empty", (yyloc).first_line);
                              (yyval.type_node) = NULL;  }
#line 1844 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 234 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Def -> Specifier DecList SEMI", (yyloc).first_line); 
                                      (yyval.type_node) = createNode("Def", "", (yyloc).first_line);
                                      constructTree((yyval.type_node), 3, (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));       }
#line 1852 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 237 "./syntax.y" /* yacc.c:1646  */
    {
                                      printDebug2("Def -> Specifier error SEMI", (yyloc).first_line);
                                      printErrorTypeB("Syntax error when defining variable", (yyloc).first_line);
                                      n_error++;
                                                    }
#line 1862 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 244 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("DecList -> Dec", (yyloc).first_line);    
                      (yyval.type_node) = createNode("DecList", "", (yyloc).first_line);
                      constructTree((yyval.type_node), 1, (yyvsp[0].type_node));          }
#line 1870 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 247 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("DecList -> Dec COMMA DecList", (yyloc).first_line);
                              (yyval.type_node) = createNode("DecList", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 3, (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 1878 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 252 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Dec -> VarDec", (yyloc).first_line); 
                      (yyval.type_node) = createNode("Dec", "", (yyloc).first_line);
                      constructTree((yyval.type_node), 1, (yyvsp[0].type_node));              }
#line 1886 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 255 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Dec -> VarDec ASSIGNOP Exp", (yyloc).first_line);
                                  (yyval.type_node) = createNode("Dec", "", (yyloc).first_line);
                                  constructTree((yyval.type_node), 3, (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));              }
#line 1894 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 261 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> Exp ASSIGNOP Exp", (yyloc).first_line);
                              (yyval.type_node) = createNode("Exp", "", (yyloc).first_line);    
                              constructTree((yyval.type_node), 3, (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 1902 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 264 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> Exp AND Exp", (yyloc).first_line);
                              (yyval.type_node) = createNode("Exp", "", (yyloc).first_line);  
                              constructTree((yyval.type_node), 3, (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 1910 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 267 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> Exp OR Exp", (yyloc).first_line);
                              (yyval.type_node) = createNode("Exp", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 3, (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 1918 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 270 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> Exp RELOP Exp", (yyloc).first_line);
                              (yyval.type_node) = createNode("Exp", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 3, (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 1926 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 273 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> Exp PLUS Exp", (yyloc).first_line);
                              (yyval.type_node) = createNode("Exp", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 3, (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 1934 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 276 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> Exp MINUS Exp", (yyloc).first_line);
                              (yyval.type_node) = createNode("Exp", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 3, (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 1942 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 279 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> Exp STAR Exp", (yyloc).first_line);
                              (yyval.type_node) = createNode("Exp", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 3, (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 1950 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 282 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> Exp DIV Exp", (yyloc).first_line); 
                              (yyval.type_node) = createNode("Exp", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 3, (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 1958 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 285 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> LP Exp RP", (yyloc).first_line);
                              (yyval.type_node) = createNode("Exp", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 3, (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 1966 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 288 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> MINUS Exp", (yyloc).first_line);
                              (yyval.type_node) = createNode("Exp", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 2, (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 1974 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 291 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> NOT Exp", (yyloc).first_line);
                              (yyval.type_node) = createNode("Exp", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 2, (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 1982 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 294 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> ID LP Args RP", (yyloc).first_line);
                              (yyval.type_node) = createNode("Exp", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 4, (yyvsp[-3].type_node), (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 1990 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 297 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> ID LP RP", (yyloc).first_line);
                              (yyval.type_node) = createNode("Exp", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 3, (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 1998 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 300 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> Exp LB Exp RB", (yyloc).first_line);
                              (yyval.type_node) = createNode("Exp", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 4, (yyvsp[-3].type_node), (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 2006 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 303 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> Exp DOT ID", (yyloc).first_line);
                              (yyval.type_node) = createNode("Exp", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 3, (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 2014 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 306 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> ID", (yyloc).first_line);
                              (yyval.type_node) = createNode("Exp", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 1, (yyvsp[0].type_node));     }
#line 2022 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 309 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> INT", (yyloc).first_line);
                              (yyval.type_node) = createNode("Exp", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 1, (yyvsp[0].type_node));     }
#line 2030 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 312 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> FLOAT", (yyloc).first_line);
                              (yyval.type_node) = createNode("Exp", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 1, (yyvsp[0].type_node));     }
#line 2038 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 315 "./syntax.y" /* yacc.c:1646  */
    {
                              printErrorTypeB("Syntax error between []", (yyloc).first_line);
                              n_error++;
                            }
#line 2047 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 320 "./syntax.y" /* yacc.c:1646  */
    { printErrorTypeB("Illegal expression", (yyloc).first_line);
                              n_error++;
                            }
#line 2055 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 324 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> Exp LB ID COMMA ID RB", (yyloc).first_line);
                              printErrorTypeB("Syntax error between []", (yyloc).first_line);
                              n_error++;
                            }
#line 2064 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 329 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> LB Exp RP", (yyloc).first_line);
                              printErrorTypeB("Syntax error, parentheses don't match", (yyloc).first_line);
                              n_error++;
                            }
#line 2073 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 334 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> INVALID_ID", (yyloc).first_line);
                              printErrorTypeB("Illegal ID", (yyloc).first_line);
                              n_error++;
                            }
#line 2082 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 339 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Exp -> Exp RELOP error Exp", (yyloc).first_line);
                              printErrorTypeB("Unexpected RELOP", (yyloc).first_line);
                              n_error++;
                            }
#line 2091 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 345 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Args -> Exp COMMA Args", (yyloc).first_line);
                              (yyval.type_node) = createNode("Args", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 3, (yyvsp[-2].type_node), (yyvsp[-1].type_node), (yyvsp[0].type_node));     }
#line 2099 "./syntax.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 348 "./syntax.y" /* yacc.c:1646  */
    { printDebug2("Args -> Exp", (yyloc).first_line);
                              (yyval.type_node) = createNode("Args", "", (yyloc).first_line);
                              constructTree((yyval.type_node), 1, (yyvsp[0].type_node));     }
#line 2107 "./syntax.tab.c" /* yacc.c:1646  */
    break;


#line 2111 "./syntax.tab.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }

  yyerror_range[1] = yylloc;

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  yyerror_range[1] = yylsp[1-yylen];
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  /* Using YYLLOC is tempting, but would change the location of
     the lookahead.  YYLOC is available though.  */
  YYLLOC_DEFAULT (yyloc, yyerror_range, 2);
  *++yylsp = yyloc;

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
#line 354 "./syntax.y" /* yacc.c:1906  */


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

    if(debug) preOrderTraverse(root, 0);
    else{
        if(n_error == 0) preOrderTraverse(root, 0);
    }
    return 0;
}

int yyerror(char* str){
    /* do nothing */
}
