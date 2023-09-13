%{


/* This interpreter evaluates arithmetic expressions and assigns
   them to the specified variable names. The grammar is:

   pgm -> stmtlist
   stmtlist -> stmt | stmtlist ; stmt
   stmt -> id = exp 
        |  print id 
        | if cond then stmt else stmt
        | while cond do stmt

   exp -> exp + mulexp | exp - mulexp 
   mulexp -> mulexp * primexp | mulexp / primexp | mulexp % primexp
   primexp ->  ( exp ) | ( exp ) | - primexp | id | number | id ( exp ) 

   cond -> cond "||" multcond | mulcond
   mulcond -> multcond "&&" primcond | primcond 
   primcond -> exp relop exp | "!" primcond   
 
*/

#include <iostream>
#include <fstream>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <list>
#include "imp.h"



using namespace std;

extern FILE *yyin;

// the root of the abstract syntax tree
 statement *root;

// for keeping track of line numbers in the program we are parsing
  int line_num = 1;

// function prototypes, we need the yylex return prototype so C++ won't complain
int yylex();
void yyerror(const char * s);

%}

%start program

%union {
  double fNum;
  int       iNum;
  unsigned id;
  operation oper;
  exp_node *exp_node_ptr;
  cond_node *cond_node_ptr;
  statement *st;
}

%error-verbose

%token <iNum> iNUMBER
%token <fNum> fNUMBER
%token <id> ID
%token <oper> RELOP
%token SEMICOLON  ASSIGN PRINT  PLUS MINUS TIMES DIVIDE MODULO LPAREN RPAREN LBRACE RBRACE IF THEN ELSE READ WHILE DO NEG AND OR

%type <exp_node_ptr> exp
%type <exp_node_ptr> mulexp
%type <exp_node_ptr> primexp 
%type <cond_node_ptr> cond
%type <cond_node_ptr> mulcond
%type <cond_node_ptr> primcond
%type <st> stmtlist
%type <st> stmt
%type <st> program

%%

program : stmtlist { root = $$; }
;

stmtlist : stmtlist SEMICOLON stmt
            { // copy up the list and add the stmt to it
              if ($3->isEmpty()) $$ = $1; 
              else $$ = new sequence_stmt($1,$3);
            }
         | stmtlist SEMICOLON error
	   { // just copy up the stmtlist when an error occurs
             $$ = $1;
             yyclearin; } 
         |  stmt 
	 { $$ = $1;   }
;

stmt: ID ASSIGN exp { 
  $$ = new assignment_stmt($1, $3);
	   }
       
| PRINT exp {
  $$ = new print_stmt($2);
 }
| READ ID { $$ = new input_stmt($2);
 }
|
{ $$ = new skip_stmt();
}
| LBRACE stmtlist RBRACE { $$=$2; }

| IF cond THEN stmt ELSE stmt { $$ = new ife_stmt($2,$4,$6);} 
| WHILE cond DO stmt {$$ = new while_stmt($2,$4);}
 ;


exp:	exp PLUS mulexp { $$ = new operator_node(ADD,$1, $3); }

|	exp MINUS mulexp { $$ = new operator_node(SUB,$1, $3); }

      |	mulexp {  $$ = $1; }
;



mulexp:	mulexp TIMES primexp { $$ = new operator_node(MUL, $1, $3); }
| mulexp DIVIDE primexp { $$ = new operator_node(DIV, $1, $3); }
| mulexp MODULO primexp { $$ = new operator_node(MOD, $1, $3); }
| primexp { $$=$1;  }
;



primexp:	MINUS primexp { $$ = new unary_minus_node($2); }

| LPAREN exp RPAREN  {  $$ = $2; }
| fNUMBER { $$ = new fNumber_node($1); }
| iNUMBER { $$ = new iNumber_node($1); }
| ID { $$ = new id_node($1); }
| ID  LPAREN exp RPAREN   {  $$ = new call_node($1, $3);  }
;

cond:	cond OR mulcond { $$ = new or_node($1, $3); }
      |	mulcond {  $$ = $1; }
;

mulcond:	mulcond AND primcond {
	  $$ = new and_node($1, $3); }
      | primcond { $$=$1;  }
;

primcond:	NEG primcond  { $$ = new neg_node($2); }

      |	LPAREN cond RPAREN  {  $$ = $2; }

      |	exp RELOP exp { $$ = new compare_node($2, $1,$3); }
;


 
%%
int main(int argc, char **argv)
{ 
  init_state();

  if (argc>1) 
  { yyin=fopen(argv[1],"r");
   
     ofstream FCDOutput(string(argv[1]) + ".FCD.tex"); 
     ofstream CFAOutput(string(argv[1]) + ".CFA.tex"); 
     ofstream ASTOutput(string(argv[1]) + ".AST.tex"); 


  //  yydebug = 1;

  yyparse();

  cout << "---------- list of input program------------" << endl << endl;

  root -> print(0); 

  cout << endl << endl; 

  cout << "---------- begin exeuction of input program------------" << endl << endl;

  root->evaluate();

  cout << "---------- end exeuction of input program------------" << endl << endl;



  ASTOutput << root->ASTgen().TikZCode(); 

  FCD fcdstart("[start]{}");
  FCD fcdend("[stop]{}");

  FCDOutput << fcdstart.seqComp(root->FCDgen()).seqComp(fcdend).TikZCode(); 


  CFA cfastart("");
  CFA cfaend("");

  CFAOutput << cfastart.seqComp(root->CFAgen()).seqComp(cfaend).TikZCode(); 

  FCDOutput.close(); 
  CFAOutput.close();
  ASTOutput.close();
  };

}

void yyerror(const char * s)
{
  fprintf(stderr, "line %d: %s\n", line_num, s);
}

