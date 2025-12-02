/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_FLOWCALC_TAB_H_INCLUDED
# define YY_YY_FLOWCALC_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NL = 258,                      /* NL  */
    BIND = 259,                    /* BIND  */
    SET = 260,                     /* SET  */
    DEF = 261,                     /* DEF  */
    YIELD = 262,                   /* YIELD  */
    WHEN = 263,                    /* WHEN  */
    OTHERWISE = 264,               /* OTHERWISE  */
    EACH = 265,                    /* EACH  */
    IN = 266,                      /* IN  */
    DO = 267,                      /* DO  */
    END = 268,                     /* END  */
    AND = 269,                     /* AND  */
    OR = 270,                      /* OR  */
    NOT = 271,                     /* NOT  */
    ASC = 272,                     /* ASC  */
    DESC = 273,                    /* DESC  */
    APPLY = 274,                   /* APPLY  */
    KEEP = 275,                    /* KEEP  */
    ORDER = 276,                   /* ORDER  */
    DEDUPE = 277,                  /* DEDUPE  */
    TAKE = 278,                    /* TAKE  */
    SKIP = 279,                    /* SKIP  */
    CONCAT = 280,                  /* CONCAT  */
    JOINSTR = 281,                 /* JOINSTR  */
    TOTAL = 282,                   /* TOTAL  */
    COUNT = 283,                   /* COUNT  */
    AVG = 284,                     /* AVG  */
    ASSIGN = 285,                  /* ASSIGN  */
    UPDATE = 286,                  /* UPDATE  */
    LAMBDA = 287,                  /* LAMBDA  */
    PIPELINE = 288,                /* PIPELINE  */
    PLUS = 289,                    /* PLUS  */
    MINUS = 290,                   /* MINUS  */
    MUL = 291,                     /* MUL  */
    DIV = 292,                     /* DIV  */
    MOD = 293,                     /* MOD  */
    POW = 294,                     /* POW  */
    EQ = 295,                      /* EQ  */
    NE = 296,                      /* NE  */
    LT = 297,                      /* LT  */
    LE = 298,                      /* LE  */
    GT = 299,                      /* GT  */
    GE = 300,                      /* GE  */
    RANGE_DOT = 301,               /* RANGE_DOT  */
    RANGE_DOT_LT = 302,            /* RANGE_DOT_LT  */
    LPAREN = 303,                  /* LPAREN  */
    RPAREN = 304,                  /* RPAREN  */
    LBRACKET = 305,                /* LBRACKET  */
    RBRACKET = 306,                /* RBRACKET  */
    COMMA = 307,                   /* COMMA  */
    IDENTIFIER = 308,              /* IDENTIFIER  */
    NUMBER = 309,                  /* NUMBER  */
    STRING = 310,                  /* STRING  */
    BOOL_LIT = 311,                /* BOOL_LIT  */
    NONE = 312,                    /* NONE  */
    UMINUS = 313                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 21 "flowcalc.y"

    char *string_val;
    int bool_val;

#line 127 "flowcalc.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_FLOWCALC_TAB_H_INCLUDED  */
