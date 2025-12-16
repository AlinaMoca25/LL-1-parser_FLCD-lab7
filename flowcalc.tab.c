/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "flowcalc.y"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "st.h"
#include "dfa.h"

extern int yylex(void);
extern int yylineno;
extern FILE *yyin;
void yyerror(const char *s);

static int production_count = 0;
static void print_production(const char *lhs, const char *rhs) {
    production_count++;
    printf("Production %d: %s -> %s\n", production_count, lhs, rhs);
}


#line 91 "flowcalc.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "flowcalc.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NL = 3,                         /* NL  */
  YYSYMBOL_BIND = 4,                       /* BIND  */
  YYSYMBOL_SET = 5,                        /* SET  */
  YYSYMBOL_DEF = 6,                        /* DEF  */
  YYSYMBOL_YIELD = 7,                      /* YIELD  */
  YYSYMBOL_WHEN = 8,                       /* WHEN  */
  YYSYMBOL_OTHERWISE = 9,                  /* OTHERWISE  */
  YYSYMBOL_EACH = 10,                      /* EACH  */
  YYSYMBOL_IN = 11,                        /* IN  */
  YYSYMBOL_DO = 12,                        /* DO  */
  YYSYMBOL_END = 13,                       /* END  */
  YYSYMBOL_AND = 14,                       /* AND  */
  YYSYMBOL_OR = 15,                        /* OR  */
  YYSYMBOL_NOT = 16,                       /* NOT  */
  YYSYMBOL_ASC = 17,                       /* ASC  */
  YYSYMBOL_DESC = 18,                      /* DESC  */
  YYSYMBOL_APPLY = 19,                     /* APPLY  */
  YYSYMBOL_KEEP = 20,                      /* KEEP  */
  YYSYMBOL_ORDER = 21,                     /* ORDER  */
  YYSYMBOL_DEDUPE = 22,                    /* DEDUPE  */
  YYSYMBOL_TAKE = 23,                      /* TAKE  */
  YYSYMBOL_SKIP = 24,                      /* SKIP  */
  YYSYMBOL_CONCAT = 25,                    /* CONCAT  */
  YYSYMBOL_JOINSTR = 26,                   /* JOINSTR  */
  YYSYMBOL_TOTAL = 27,                     /* TOTAL  */
  YYSYMBOL_COUNT = 28,                     /* COUNT  */
  YYSYMBOL_AVG = 29,                       /* AVG  */
  YYSYMBOL_ASSIGN = 30,                    /* ASSIGN  */
  YYSYMBOL_UPDATE = 31,                    /* UPDATE  */
  YYSYMBOL_LAMBDA = 32,                    /* LAMBDA  */
  YYSYMBOL_PIPELINE = 33,                  /* PIPELINE  */
  YYSYMBOL_PLUS = 34,                      /* PLUS  */
  YYSYMBOL_MINUS = 35,                     /* MINUS  */
  YYSYMBOL_MUL = 36,                       /* MUL  */
  YYSYMBOL_DIV = 37,                       /* DIV  */
  YYSYMBOL_MOD = 38,                       /* MOD  */
  YYSYMBOL_POW = 39,                       /* POW  */
  YYSYMBOL_EQ = 40,                        /* EQ  */
  YYSYMBOL_NE = 41,                        /* NE  */
  YYSYMBOL_LT = 42,                        /* LT  */
  YYSYMBOL_LE = 43,                        /* LE  */
  YYSYMBOL_GT = 44,                        /* GT  */
  YYSYMBOL_GE = 45,                        /* GE  */
  YYSYMBOL_RANGE_DOT = 46,                 /* RANGE_DOT  */
  YYSYMBOL_RANGE_DOT_LT = 47,              /* RANGE_DOT_LT  */
  YYSYMBOL_LPAREN = 48,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 49,                    /* RPAREN  */
  YYSYMBOL_LBRACKET = 50,                  /* LBRACKET  */
  YYSYMBOL_RBRACKET = 51,                  /* RBRACKET  */
  YYSYMBOL_COMMA = 52,                     /* COMMA  */
  YYSYMBOL_IDENTIFIER = 53,                /* IDENTIFIER  */
  YYSYMBOL_NUMBER = 54,                    /* NUMBER  */
  YYSYMBOL_STRING = 55,                    /* STRING  */
  YYSYMBOL_BOOL_LIT = 56,                  /* BOOL_LIT  */
  YYSYMBOL_NONE = 57,                      /* NONE  */
  YYSYMBOL_UMINUS = 58,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 59,                  /* $accept  */
  YYSYMBOL_program = 60,                   /* program  */
  YYSYMBOL_stmt_list = 61,                 /* stmt_list  */
  YYSYMBOL_stmt = 62,                      /* stmt  */
  YYSYMBOL_bind_stmt = 63,                 /* bind_stmt  */
  YYSYMBOL_set_stmt = 64,                  /* set_stmt  */
  YYSYMBOL_when_stmt = 65,                 /* when_stmt  */
  YYSYMBOL_otherwise_opt = 66,             /* otherwise_opt  */
  YYSYMBOL_each_stmt = 67,                 /* each_stmt  */
  YYSYMBOL_block = 68,                     /* block  */
  YYSYMBOL_expr = 69,                      /* expr  */
  YYSYMBOL_pipe_expr = 70,                 /* pipe_expr  */
  YYSYMBOL_stage = 71,                     /* stage  */
  YYSYMBOL_stage_keyword = 72,             /* stage_keyword  */
  YYSYMBOL_logic_or = 73,                  /* logic_or  */
  YYSYMBOL_logic_and = 74,                 /* logic_and  */
  YYSYMBOL_equality = 75,                  /* equality  */
  YYSYMBOL_relation = 76,                  /* relation  */
  YYSYMBOL_add = 77,                       /* add  */
  YYSYMBOL_mul = 78,                       /* mul  */
  YYSYMBOL_unary = 79,                     /* unary  */
  YYSYMBOL_power = 80,                     /* power  */
  YYSYMBOL_primary = 81,                   /* primary  */
  YYSYMBOL_lambda = 82,                    /* lambda  */
  YYSYMBOL_list_lit = 83,                  /* list_lit  */
  YYSYMBOL_expr_list_opt = 84,             /* expr_list_opt  */
  YYSYMBOL_expr_list = 85,                 /* expr_list  */
  YYSYMBOL_range_lit = 86,                 /* range_lit  */
  YYSYMBOL_id = 87,                        /* id  */
  YYSYMBOL_number = 88,                    /* number  */
  YYSYMBOL_string = 89,                    /* string  */
  YYSYMBOL_bool = 90                       /* bool  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

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


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
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

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  50
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   307

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  59
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  32
/* YYNRULES -- Number of rules.  */
#define YYNRULES  72
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  137

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   313


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    59,    59,    66,    70,    77,    81,    85,    89,    93,
     100,   107,   114,   121,   125,   132,   139,   143,   150,   157,
     161,   168,   175,   179,   183,   187,   191,   195,   202,   206,
     213,   217,   224,   228,   232,   239,   243,   247,   251,   255,
     262,   266,   270,   277,   281,   285,   289,   296,   300,   304,
     311,   315,   322,   326,   330,   334,   338,   342,   346,   350,
     354,   361,   368,   375,   379,   386,   390,   397,   401,   408,
     415,   422,   429
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NL", "BIND", "SET",
  "DEF", "YIELD", "WHEN", "OTHERWISE", "EACH", "IN", "DO", "END", "AND",
  "OR", "NOT", "ASC", "DESC", "APPLY", "KEEP", "ORDER", "DEDUPE", "TAKE",
  "SKIP", "CONCAT", "JOINSTR", "TOTAL", "COUNT", "AVG", "ASSIGN", "UPDATE",
  "LAMBDA", "PIPELINE", "PLUS", "MINUS", "MUL", "DIV", "MOD", "POW", "EQ",
  "NE", "LT", "LE", "GT", "GE", "RANGE_DOT", "RANGE_DOT_LT", "LPAREN",
  "RPAREN", "LBRACKET", "RBRACKET", "COMMA", "IDENTIFIER", "NUMBER",
  "STRING", "BOOL_LIT", "NONE", "UMINUS", "$accept", "program",
  "stmt_list", "stmt", "bind_stmt", "set_stmt", "when_stmt",
  "otherwise_opt", "each_stmt", "block", "expr", "pipe_expr", "stage",
  "stage_keyword", "logic_or", "logic_and", "equality", "relation", "add",
  "mul", "unary", "power", "primary", "lambda", "list_lit",
  "expr_list_opt", "expr_list", "range_lit", "id", "number", "string",
  "bool", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-108)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-52)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      30,   -48,   -48,   213,   -48,   213,   213,   213,   213,  -108,
    -108,  -108,  -108,  -108,    10,    18,  -108,  -108,  -108,  -108,
    -108,    -5,    17,    39,    19,    16,   -13,    32,   101,  -108,
    -108,     8,  -108,  -108,  -108,    36,  -108,  -108,  -108,    49,
      77,    -3,    96,    -5,   142,   147,   -10,    -5,     9,    62,
    -108,    30,   213,   213,    -1,   213,   213,   213,   213,   213,
     213,   213,   213,   213,   213,   213,   213,   213,   213,   213,
     213,   213,    30,   213,  -108,  -108,   213,  -108,    -5,    -5,
      68,    69,    78,    84,   104,   105,  -108,  -108,    12,   209,
     260,   260,   238,   238,   238,   238,    92,    97,   158,   163,
     192,   208,    -5,    -5,    -5,  -108,    14,     2,    -5,   -48,
     -48,   213,   213,   213,    75,    30,   128,   144,    30,   113,
      36,   115,    28,    47,    76,  -108,  -108,    30,  -108,     3,
    -108,  -108,  -108,  -108,  -108,   162,  -108
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,     0,     0,    64,    69,
      70,    71,    72,    55,     0,     2,     3,     5,     6,     7,
       8,     9,    18,    19,    28,    30,    32,    35,    40,    43,
      49,    50,    60,    58,    59,    56,    52,    53,    54,     0,
       0,     0,     0,     0,    43,    43,     0,    65,     0,    63,
       1,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    57,    62,     0,     4,    67,    68,
       0,     0,     0,     0,     0,     0,    20,    21,    29,    31,
      33,    34,    36,    37,    38,    39,    40,    40,    43,    43,
      43,    43,    61,    10,    11,    16,    14,     0,    66,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    27,    17,     0,    12,     0,
      22,    23,    24,    25,    26,    13,    15
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -108,  -108,  -108,     4,  -108,  -108,  -108,  -108,  -108,  -107,
       0,  -108,  -108,  -108,  -108,   114,   112,    24,    29,    35,
      -4,  -108,  -108,    -7,  -108,  -108,  -108,  -108,    11,  -108,
    -108,  -108
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    14,    15,   105,    17,    18,    19,   117,    20,   106,
      43,    22,    86,    87,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    48,    49,    34,    35,    36,
      37,    38
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      21,    44,    45,    41,    16,     9,   115,    46,    47,    72,
      50,   129,    39,    40,   118,    42,   136,   115,    80,    81,
     135,    51,    82,   116,    83,    84,    56,   -28,    85,    59,
      60,    61,    62,    56,     1,     2,    52,    53,     3,    74,
       4,    52,    53,    52,    53,   -28,     5,    68,    52,    53,
      54,    21,    78,    79,    55,    77,    57,    58,   -28,   -28,
      75,    98,    99,   100,   101,     6,    63,    64,    69,   102,
     103,   104,    21,   107,    52,    53,   108,   132,     7,    70,
       8,    90,    91,     9,    10,    11,    12,    13,    92,    93,
      94,    95,   -41,    52,    53,   -41,   133,   -42,    96,    97,
     -42,   -41,   119,   121,   -41,   -41,   -42,    73,    71,   -42,
     -42,   122,   123,   124,    76,    21,   109,   110,    21,   126,
     120,   120,    52,    53,   125,   134,   111,    21,    65,    66,
      67,   -41,   112,    65,    66,    67,   -42,    65,    66,    67,
     127,   -41,   -47,   -41,   -41,   -47,   -42,   -48,   -42,   -42,
     -48,   -47,   113,   114,   -47,   -47,   -48,   128,   -44,   -48,
     -48,   -44,   130,   -45,   131,   115,   -45,   -44,    89,    88,
     -44,   -44,   -45,     0,     0,   -45,   -45,     0,     0,     0,
       0,   -47,     0,     0,     0,     0,   -48,     0,     0,     0,
       0,   -47,   -46,   -47,   -47,   -46,   -48,   -44,   -48,   -48,
       0,   -46,   -45,     0,   -46,   -46,     0,   -44,   -51,   -44,
     -44,   -51,   -45,     0,   -45,   -45,     0,   -51,     0,     0,
     -51,   -51,     0,   -30,   -30,     0,     0,     0,     0,     5,
       0,   -46,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   -46,   -30,   -46,   -46,     0,     0,   -51,     6,    57,
      58,     0,   -35,   -35,     0,   -30,   -30,   -51,     0,   -51,
     -51,     7,     0,     8,     0,     0,     9,    10,    11,    12,
      13,   -35,    63,    64,   -32,   -32,     0,     0,   -35,   -35,
     -35,   -35,   -35,   -35,   -35,   -35,     0,     0,     0,     0,
       0,     0,     0,   -32,     0,     0,     0,     0,     0,     0,
     -32,   -32,    59,    60,    61,    62,   -32,   -32
};

static const yytype_int8 yycheck[] =
{
       0,     5,     6,     3,     0,    53,     3,     7,     8,    12,
       0,   118,     1,     2,    12,     4,    13,     3,    19,    20,
     127,     3,    23,     9,    25,    26,    14,    15,    29,    42,
      43,    44,    45,    14,     4,     5,    46,    47,     8,    49,
      10,    46,    47,    46,    47,    33,    16,    39,    46,    47,
      33,    51,    52,    53,    15,    51,    40,    41,    46,    47,
      51,    65,    66,    67,    68,    35,    34,    35,    32,    69,
      70,    71,    72,    73,    46,    47,    76,    49,    48,    30,
      50,    57,    58,    53,    54,    55,    56,    57,    59,    60,
      61,    62,     0,    46,    47,     3,    49,     0,    63,    64,
       3,     9,   109,   110,    12,    13,     9,    11,    31,    12,
      13,   111,   112,   113,    52,   115,    48,    48,   118,   115,
     109,   110,    46,    47,    49,    49,    48,   127,    36,    37,
      38,    39,    48,    36,    37,    38,    39,    36,    37,    38,
      12,    49,     0,    51,    52,     3,    49,     0,    51,    52,
       3,     9,    48,    48,    12,    13,     9,    13,     0,    12,
      13,     3,    49,     0,    49,     3,     3,     9,    56,    55,
      12,    13,     9,    -1,    -1,    12,    13,    -1,    -1,    -1,
      -1,    39,    -1,    -1,    -1,    -1,    39,    -1,    -1,    -1,
      -1,    49,     0,    51,    52,     3,    49,    39,    51,    52,
      -1,     9,    39,    -1,    12,    13,    -1,    49,     0,    51,
      52,     3,    49,    -1,    51,    52,    -1,     9,    -1,    -1,
      12,    13,    -1,    14,    15,    -1,    -1,    -1,    -1,    16,
      -1,    39,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    49,    33,    51,    52,    -1,    -1,    39,    35,    40,
      41,    -1,    14,    15,    -1,    46,    47,    49,    -1,    51,
      52,    48,    -1,    50,    -1,    -1,    53,    54,    55,    56,
      57,    33,    34,    35,    14,    15,    -1,    -1,    40,    41,
      42,    43,    44,    45,    46,    47,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    33,    -1,    -1,    -1,    -1,    -1,    -1,
      40,    41,    42,    43,    44,    45,    46,    47
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,     5,     8,    10,    16,    35,    48,    50,    53,
      54,    55,    56,    57,    60,    61,    62,    63,    64,    65,
      67,    69,    70,    73,    74,    75,    76,    77,    78,    79,
      80,    81,    82,    83,    86,    87,    88,    89,    90,    87,
      87,    69,    87,    69,    79,    79,    69,    69,    84,    85,
       0,     3,    46,    47,    33,    15,    14,    40,    41,    42,
      43,    44,    45,    34,    35,    36,    37,    38,    39,    32,
      30,    31,    12,    11,    49,    51,    52,    62,    69,    69,
      19,    20,    23,    25,    26,    29,    71,    72,    74,    75,
      76,    76,    77,    77,    77,    77,    78,    78,    79,    79,
      79,    79,    69,    69,    69,    62,    68,    69,    69,    48,
      48,    48,    48,    48,    48,     3,     9,    66,    12,    82,
      87,    82,    69,    69,    69,    49,    62,    12,    13,    68,
      49,    49,    49,    49,    49,    68,    13
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    59,    60,    61,    61,    62,    62,    62,    62,    62,
      63,    64,    65,    66,    66,    67,    68,    68,    69,    70,
      70,    71,    72,    72,    72,    72,    72,    72,    73,    73,
      74,    74,    75,    75,    75,    76,    76,    76,    76,    76,
      77,    77,    77,    78,    78,    78,    78,    79,    79,    79,
      80,    80,    81,    81,    81,    81,    81,    81,    81,    81,
      81,    82,    83,    84,    84,    85,    85,    86,    86,    87,
      88,    89,    90
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     3,     1,     1,     1,     1,     1,
       4,     4,     6,     3,     0,     7,     1,     3,     1,     1,
       3,     1,     4,     4,     4,     4,     4,     3,     1,     3,
       1,     3,     1,     3,     3,     1,     3,     3,     3,     3,
       1,     3,     3,     1,     3,     3,     3,     2,     2,     1,
       1,     3,     1,     1,     1,     1,     1,     3,     1,     1,
       1,     3,     3,     1,     0,     1,     3,     3,     3,     1,
       1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
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

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


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




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


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

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
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
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
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


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: stmt_list  */
#line 59 "flowcalc.y"
              {
        print_production("program", "stmt_list");
        (yyval.string_val) = "program";
    }
#line 1301 "flowcalc.tab.c"
    break;

  case 3: /* stmt_list: stmt  */
#line 66 "flowcalc.y"
         {
        print_production("stmt_list", "stmt");
        (yyval.string_val) = "stmt_list";
    }
#line 1310 "flowcalc.tab.c"
    break;

  case 4: /* stmt_list: stmt_list NL stmt  */
#line 70 "flowcalc.y"
                        {
        print_production("stmt_list", "stmt_list NL stmt");
        (yyval.string_val) = "stmt_list";
    }
#line 1319 "flowcalc.tab.c"
    break;

  case 5: /* stmt: bind_stmt  */
#line 77 "flowcalc.y"
              {
        print_production("stmt", "bind_stmt");
        (yyval.string_val) = "stmt";
    }
#line 1328 "flowcalc.tab.c"
    break;

  case 6: /* stmt: set_stmt  */
#line 81 "flowcalc.y"
               {
        print_production("stmt", "set_stmt");
        (yyval.string_val) = "stmt";
    }
#line 1337 "flowcalc.tab.c"
    break;

  case 7: /* stmt: when_stmt  */
#line 85 "flowcalc.y"
                {
        print_production("stmt", "when_stmt");
        (yyval.string_val) = "stmt";
    }
#line 1346 "flowcalc.tab.c"
    break;

  case 8: /* stmt: each_stmt  */
#line 89 "flowcalc.y"
                {
        print_production("stmt", "each_stmt");
        (yyval.string_val) = "stmt";
    }
#line 1355 "flowcalc.tab.c"
    break;

  case 9: /* stmt: expr  */
#line 93 "flowcalc.y"
           {
        print_production("stmt", "expr");
        (yyval.string_val) = "stmt";
    }
#line 1364 "flowcalc.tab.c"
    break;

  case 10: /* bind_stmt: BIND id ASSIGN expr  */
#line 100 "flowcalc.y"
                        {
        print_production("bind_stmt", "BIND id ASSIGN expr");
        (yyval.string_val) = "bind_stmt";
    }
#line 1373 "flowcalc.tab.c"
    break;

  case 11: /* set_stmt: SET id UPDATE expr  */
#line 107 "flowcalc.y"
                       {
        print_production("set_stmt", "SET id UPDATE expr");
        (yyval.string_val) = "set_stmt";
    }
#line 1382 "flowcalc.tab.c"
    break;

  case 12: /* when_stmt: WHEN expr DO block otherwise_opt END  */
#line 114 "flowcalc.y"
                                         {
        print_production("when_stmt", "WHEN expr DO block otherwise_opt END");
        (yyval.string_val) = "when_stmt";
    }
#line 1391 "flowcalc.tab.c"
    break;

  case 13: /* otherwise_opt: OTHERWISE DO block  */
#line 121 "flowcalc.y"
                       {
        print_production("otherwise_opt", "OTHERWISE DO block");
        (yyval.string_val) = "otherwise_opt";
    }
#line 1400 "flowcalc.tab.c"
    break;

  case 14: /* otherwise_opt: %empty  */
#line 125 "flowcalc.y"
      {
        print_production("otherwise_opt", "ε");
        (yyval.string_val) = "otherwise_opt";
    }
#line 1409 "flowcalc.tab.c"
    break;

  case 15: /* each_stmt: EACH id IN expr DO block END  */
#line 132 "flowcalc.y"
                                 {
        print_production("each_stmt", "EACH id IN expr DO block END");
        (yyval.string_val) = "each_stmt";
    }
#line 1418 "flowcalc.tab.c"
    break;

  case 16: /* block: stmt  */
#line 139 "flowcalc.y"
         {
        print_production("block", "stmt");
        (yyval.string_val) = "block";
    }
#line 1427 "flowcalc.tab.c"
    break;

  case 17: /* block: block NL stmt  */
#line 143 "flowcalc.y"
                    {
        print_production("block", "block NL stmt");
        (yyval.string_val) = "block";
    }
#line 1436 "flowcalc.tab.c"
    break;

  case 18: /* expr: pipe_expr  */
#line 150 "flowcalc.y"
              {
        print_production("expr", "pipe_expr");
        (yyval.string_val) = "expr";
    }
#line 1445 "flowcalc.tab.c"
    break;

  case 19: /* pipe_expr: logic_or  */
#line 157 "flowcalc.y"
             {
        print_production("pipe_expr", "logic_or");
        (yyval.string_val) = "pipe_expr";
    }
#line 1454 "flowcalc.tab.c"
    break;

  case 20: /* pipe_expr: pipe_expr PIPELINE stage  */
#line 161 "flowcalc.y"
                               {
        print_production("pipe_expr", "pipe_expr PIPELINE stage");
        (yyval.string_val) = "pipe_expr";
    }
#line 1463 "flowcalc.tab.c"
    break;

  case 21: /* stage: stage_keyword  */
#line 168 "flowcalc.y"
                  {
        print_production("stage", "stage_keyword");
        (yyval.string_val) = "stage";
    }
#line 1472 "flowcalc.tab.c"
    break;

  case 22: /* stage_keyword: APPLY LPAREN lambda RPAREN  */
#line 175 "flowcalc.y"
                               {
        print_production("stage_keyword", "APPLY LPAREN lambda RPAREN");
        (yyval.string_val) = "stage_keyword";
    }
#line 1481 "flowcalc.tab.c"
    break;

  case 23: /* stage_keyword: KEEP LPAREN lambda RPAREN  */
#line 179 "flowcalc.y"
                                {
        print_production("stage_keyword", "KEEP LPAREN lambda RPAREN");
        (yyval.string_val) = "stage_keyword";
    }
#line 1490 "flowcalc.tab.c"
    break;

  case 24: /* stage_keyword: TAKE LPAREN expr RPAREN  */
#line 183 "flowcalc.y"
                              {
        print_production("stage_keyword", "TAKE LPAREN expr RPAREN");
        (yyval.string_val) = "stage_keyword";
    }
#line 1499 "flowcalc.tab.c"
    break;

  case 25: /* stage_keyword: CONCAT LPAREN expr RPAREN  */
#line 187 "flowcalc.y"
                                {
        print_production("stage_keyword", "CONCAT LPAREN expr RPAREN");
        (yyval.string_val) = "stage_keyword";
    }
#line 1508 "flowcalc.tab.c"
    break;

  case 26: /* stage_keyword: JOINSTR LPAREN expr RPAREN  */
#line 191 "flowcalc.y"
                                 {
        print_production("stage_keyword", "JOINSTR LPAREN expr RPAREN");
        (yyval.string_val) = "stage_keyword";
    }
#line 1517 "flowcalc.tab.c"
    break;

  case 27: /* stage_keyword: AVG LPAREN RPAREN  */
#line 195 "flowcalc.y"
                        {
        print_production("stage_keyword", "AVG LPAREN RPAREN");
        (yyval.string_val) = "stage_keyword";
    }
#line 1526 "flowcalc.tab.c"
    break;

  case 28: /* logic_or: logic_and  */
#line 202 "flowcalc.y"
              {
        print_production("logic_or", "logic_and");
        (yyval.string_val) = "logic_or";
    }
#line 1535 "flowcalc.tab.c"
    break;

  case 29: /* logic_or: logic_or OR logic_and  */
#line 206 "flowcalc.y"
                            {
        print_production("logic_or", "logic_or OR logic_and");
        (yyval.string_val) = "logic_or";
    }
#line 1544 "flowcalc.tab.c"
    break;

  case 30: /* logic_and: equality  */
#line 213 "flowcalc.y"
             {
        print_production("logic_and", "equality");
        (yyval.string_val) = "logic_and";
    }
#line 1553 "flowcalc.tab.c"
    break;

  case 31: /* logic_and: logic_and AND equality  */
#line 217 "flowcalc.y"
                             {
        print_production("logic_and", "logic_and AND equality");
        (yyval.string_val) = "logic_and";
    }
#line 1562 "flowcalc.tab.c"
    break;

  case 32: /* equality: relation  */
#line 224 "flowcalc.y"
             {
        print_production("equality", "relation");
        (yyval.string_val) = "equality";
    }
#line 1571 "flowcalc.tab.c"
    break;

  case 33: /* equality: equality EQ relation  */
#line 228 "flowcalc.y"
                           {
        print_production("equality", "equality EQ relation");
        (yyval.string_val) = "equality";
    }
#line 1580 "flowcalc.tab.c"
    break;

  case 34: /* equality: equality NE relation  */
#line 232 "flowcalc.y"
                           {
        print_production("equality", "equality NE relation");
        (yyval.string_val) = "equality";
    }
#line 1589 "flowcalc.tab.c"
    break;

  case 35: /* relation: add  */
#line 239 "flowcalc.y"
        {
        print_production("relation", "add");
        (yyval.string_val) = "relation";
    }
#line 1598 "flowcalc.tab.c"
    break;

  case 36: /* relation: relation LT add  */
#line 243 "flowcalc.y"
                      {
        print_production("relation", "relation LT add");
        (yyval.string_val) = "relation";
    }
#line 1607 "flowcalc.tab.c"
    break;

  case 37: /* relation: relation LE add  */
#line 247 "flowcalc.y"
                      {
        print_production("relation", "relation LE add");
        (yyval.string_val) = "relation";
    }
#line 1616 "flowcalc.tab.c"
    break;

  case 38: /* relation: relation GT add  */
#line 251 "flowcalc.y"
                      {
        print_production("relation", "relation GT add");
        (yyval.string_val) = "relation";
    }
#line 1625 "flowcalc.tab.c"
    break;

  case 39: /* relation: relation GE add  */
#line 255 "flowcalc.y"
                      {
        print_production("relation", "relation GE add");
        (yyval.string_val) = "relation";
    }
#line 1634 "flowcalc.tab.c"
    break;

  case 40: /* add: mul  */
#line 262 "flowcalc.y"
        {
        print_production("add", "mul");
        (yyval.string_val) = "add";
    }
#line 1643 "flowcalc.tab.c"
    break;

  case 41: /* add: add PLUS mul  */
#line 266 "flowcalc.y"
                   {
        print_production("add", "add PLUS mul");
        (yyval.string_val) = "add";
    }
#line 1652 "flowcalc.tab.c"
    break;

  case 42: /* add: add MINUS mul  */
#line 270 "flowcalc.y"
                    {
        print_production("add", "add MINUS mul");
        (yyval.string_val) = "add";
    }
#line 1661 "flowcalc.tab.c"
    break;

  case 43: /* mul: unary  */
#line 277 "flowcalc.y"
          {
        print_production("mul", "unary");
        (yyval.string_val) = "mul";
    }
#line 1670 "flowcalc.tab.c"
    break;

  case 44: /* mul: mul MUL unary  */
#line 281 "flowcalc.y"
                    {
        print_production("mul", "mul MUL unary");
        (yyval.string_val) = "mul";
    }
#line 1679 "flowcalc.tab.c"
    break;

  case 45: /* mul: mul DIV unary  */
#line 285 "flowcalc.y"
                    {
        print_production("mul", "mul DIV unary");
        (yyval.string_val) = "mul";
    }
#line 1688 "flowcalc.tab.c"
    break;

  case 46: /* mul: mul MOD unary  */
#line 289 "flowcalc.y"
                    {
        print_production("mul", "mul MOD unary");
        (yyval.string_val) = "mul";
    }
#line 1697 "flowcalc.tab.c"
    break;

  case 47: /* unary: NOT unary  */
#line 296 "flowcalc.y"
              {
        print_production("unary", "NOT unary");
        (yyval.string_val) = "unary";
    }
#line 1706 "flowcalc.tab.c"
    break;

  case 48: /* unary: MINUS unary  */
#line 300 "flowcalc.y"
                               {
        print_production("unary", "MINUS unary");
        (yyval.string_val) = "unary";
    }
#line 1715 "flowcalc.tab.c"
    break;

  case 49: /* unary: power  */
#line 304 "flowcalc.y"
            {
        print_production("unary", "power");
        (yyval.string_val) = "unary";
    }
#line 1724 "flowcalc.tab.c"
    break;

  case 50: /* power: primary  */
#line 311 "flowcalc.y"
            {
        print_production("power", "primary");
        (yyval.string_val) = "power";
    }
#line 1733 "flowcalc.tab.c"
    break;

  case 51: /* power: primary POW unary  */
#line 315 "flowcalc.y"
                        {
        print_production("power", "primary POW unary");
        (yyval.string_val) = "power";
    }
#line 1742 "flowcalc.tab.c"
    break;

  case 52: /* primary: number  */
#line 322 "flowcalc.y"
           {
        print_production("primary", "number");
        (yyval.string_val) = "primary";
    }
#line 1751 "flowcalc.tab.c"
    break;

  case 53: /* primary: string  */
#line 326 "flowcalc.y"
             {
        print_production("primary", "string");
        (yyval.string_val) = "primary";
    }
#line 1760 "flowcalc.tab.c"
    break;

  case 54: /* primary: bool  */
#line 330 "flowcalc.y"
           {
        print_production("primary", "bool");
        (yyval.string_val) = "primary";
    }
#line 1769 "flowcalc.tab.c"
    break;

  case 55: /* primary: NONE  */
#line 334 "flowcalc.y"
           {
        print_production("primary", "NONE");
        (yyval.string_val) = "primary";
    }
#line 1778 "flowcalc.tab.c"
    break;

  case 56: /* primary: id  */
#line 338 "flowcalc.y"
         {
        print_production("primary", "id");
        (yyval.string_val) = "primary";
    }
#line 1787 "flowcalc.tab.c"
    break;

  case 57: /* primary: LPAREN expr RPAREN  */
#line 342 "flowcalc.y"
                         {
        print_production("primary", "LPAREN expr RPAREN");
        (yyval.string_val) = "primary";
    }
#line 1796 "flowcalc.tab.c"
    break;

  case 58: /* primary: list_lit  */
#line 346 "flowcalc.y"
               {
        print_production("primary", "list_lit");
        (yyval.string_val) = "primary";
    }
#line 1805 "flowcalc.tab.c"
    break;

  case 59: /* primary: range_lit  */
#line 350 "flowcalc.y"
                {
        print_production("primary", "range_lit");
        (yyval.string_val) = "primary";
    }
#line 1814 "flowcalc.tab.c"
    break;

  case 60: /* primary: lambda  */
#line 354 "flowcalc.y"
             {
        print_production("primary", "lambda");
        (yyval.string_val) = "primary";
    }
#line 1823 "flowcalc.tab.c"
    break;

  case 61: /* lambda: id LAMBDA expr  */
#line 361 "flowcalc.y"
                   {
        print_production("lambda", "id LAMBDA expr");
        (yyval.string_val) = "lambda";
    }
#line 1832 "flowcalc.tab.c"
    break;

  case 62: /* list_lit: LBRACKET expr_list_opt RBRACKET  */
#line 368 "flowcalc.y"
                                    {
        print_production("list_lit", "LBRACKET expr_list_opt RBRACKET");
        (yyval.string_val) = "list_lit";
    }
#line 1841 "flowcalc.tab.c"
    break;

  case 63: /* expr_list_opt: expr_list  */
#line 375 "flowcalc.y"
              {
        print_production("expr_list_opt", "expr_list");
        (yyval.string_val) = "expr_list_opt";
    }
#line 1850 "flowcalc.tab.c"
    break;

  case 64: /* expr_list_opt: %empty  */
#line 379 "flowcalc.y"
      {
        print_production("expr_list_opt", "ε");
        (yyval.string_val) = "expr_list_opt";
    }
#line 1859 "flowcalc.tab.c"
    break;

  case 65: /* expr_list: expr  */
#line 386 "flowcalc.y"
         {
        print_production("expr_list", "expr");
        (yyval.string_val) = "expr_list";
    }
#line 1868 "flowcalc.tab.c"
    break;

  case 66: /* expr_list: expr_list COMMA expr  */
#line 390 "flowcalc.y"
                           {
        print_production("expr_list", "expr_list COMMA expr");
        (yyval.string_val) = "expr_list";
    }
#line 1877 "flowcalc.tab.c"
    break;

  case 67: /* range_lit: expr RANGE_DOT expr  */
#line 397 "flowcalc.y"
                        {
        print_production("range_lit", "expr RANGE_DOT expr");
        (yyval.string_val) = "range_lit";
    }
#line 1886 "flowcalc.tab.c"
    break;

  case 68: /* range_lit: expr RANGE_DOT_LT expr  */
#line 401 "flowcalc.y"
                             {
        print_production("range_lit", "expr RANGE_DOT_LT expr");
        (yyval.string_val) = "range_lit";
    }
#line 1895 "flowcalc.tab.c"
    break;

  case 69: /* id: IDENTIFIER  */
#line 408 "flowcalc.y"
               {
        print_production("id", "IDENTIFIER");
        (yyval.string_val) = "id";
    }
#line 1904 "flowcalc.tab.c"
    break;

  case 70: /* number: NUMBER  */
#line 415 "flowcalc.y"
           {
        print_production("number", "NUMBER");
        (yyval.string_val) = "number";
    }
#line 1913 "flowcalc.tab.c"
    break;

  case 71: /* string: STRING  */
#line 422 "flowcalc.y"
           {
        print_production("string", "STRING");
        (yyval.string_val) = "string";
    }
#line 1922 "flowcalc.tab.c"
    break;

  case 72: /* bool: BOOL_LIT  */
#line 429 "flowcalc.y"
             {
        print_production("bool", "BOOL_LIT");
        (yyval.string_val) = "bool";
    }
#line 1931 "flowcalc.tab.c"
    break;


#line 1935 "flowcalc.tab.c"

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
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

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
                      yytoken, &yylval);
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
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

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

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 435 "flowcalc.y"


void yyerror(const char *s) {
    fprintf(stderr, "Parse error at line %d: %s\n", yylineno, s);
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <source.flowcalc>\n", argv[0]);
        return 1;
    }
    
    FILE *fp = fopen(argv[1], "r");
    if (!fp) {
        perror("open source");
        return 1;
    }
    yyin = fp;
    
    DFA ID, NUM;
    if (dfa_load("identifier.fa", &ID) != 0) {
        fprintf(stderr, "cannot load identifier.fa\n");
        fclose(fp);
        return 1;
    }
    if (dfa_load("number.fa", &NUM) != 0) {
        fprintf(stderr, "cannot load number.fa\n");
        fclose(fp);
        return 1;
    }
    
    SymbolTable ST;
    st_init(&ST, 199);
    
    extern void init_lexer(SymbolTable *st, DFA *id_dfa, DFA *num_dfa);
    init_lexer(&ST, &ID, &NUM);
    
    printf("===== Parsing Productions =====\n\n");
    
    int result = yyparse();
    
    fclose(fp);
    
    if (result == 0) {
        printf("\n===== Parsing Successful =====\n");
        printf("Total productions: %d\n", production_count);
        
        extern void dump_pif(void);
        st_dump(&ST);
        dump_pif();
        
        st_free(&ST);
        return 0;
    } else {
        printf("\n===== Parsing Failed =====\n");
        /* Even on parse failure, print symbol table and PIF for debugging */
        extern void dump_pif(void);
        st_dump(&ST);
        dump_pif();
        st_free(&ST);
        return 1;
    }
}

