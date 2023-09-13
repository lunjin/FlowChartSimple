%option interactive
%{
/* * * * * * * * * * * *
 * * * DEFINITIONS * * *
 * * * * * * * * * * * */
%}

%{

// y.tab.h contains the token number values produced by the parser
#include <string.h>
  //#include <stdlib.h>

#include "imp.h"
#include "y.tab.h"
#include "string_table.h"

extern int line_num;
extern string_table nametable;

%}


%option noyywrap
DIGIT [0-9]
LETTER [a-zA-Z]

%{ 
  /* * * * * * * * * *
   * * * STATES  * * *
   * * * * * * * * * */
%}

%x ERROR

%%

%{
/* * * * * * * * * 
 * * * RULES * * *
 * * * * * * * * */
%}

print   { return PRINT; }
{DIGIT}+ {
  yylval.iNum = atoi(yytext); return iNUMBER;
	}

{DIGIT}+.{DIGIT}+ {
  yylval.fNum = atof(yytext); return fNUMBER;
	}

"if" { return IF;}

"then" { return THEN;}

"else" { return ELSE;}

"read" { return READ;}

"while" { return WHILE;}

"do" { return DO;}


{LETTER}[0-9a-zA-Z]* {
        yylval.id = nametable.put(yytext); return ID;
	}

[ \t\f\r]	;		 // ignore white space 

\n      { line_num++; }
"-"	{ return MINUS;  }
"+"	{ return PLUS;   }
"*"	{ return TIMES;  }
"/"	{ return DIVIDE; }
"%"	{ return MODULO; }
"("	{ return LPAREN; }
")"	{ return RPAREN; }
"{"	{ return LBRACE; }
"}"	{ return RBRACE; }
":="     { return ASSIGN; }
";"    {return SEMICOLON;}
"!"    {return NEG;}
"&&"    {return AND;}
"||"    {return OR;}
"=" {yylval.oper = EQ; return RELOP;}
">" {yylval.oper = GE; return RELOP;}



. { BEGIN(ERROR); yymore(); }
<ERROR>[^{DIGIT}{LETTER}+\-/*(){}= \t\n\f\r] { yymore(); }
<ERROR>(.|\n) { yyless(yyleng-1); printf("error token: %s on line %d\n", yytext, line_num); 
           BEGIN(INITIAL); }

%%

