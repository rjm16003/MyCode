%{
#include <stdio.h>
#include "uthash.h"
#include "ntest5.h"
#include "t5_functions.h"

int yylex();

//int count=0;

%}

%union
{
   char *sval;
   int val;
   struct tree *node;
};

%token <sval> ident
%token <val> NUM
%token <sval> boollit
%token <val> INT BOOL


%token <node> LP RP SC
%token <sval> OP2 OP3 OP4

%token <node> THEN ELSE END
%token <node> DO PROGRAM VAR AS

%token <node> READINT


%token<node> WRITEINT
%token<node> WHILE
%token<node> IF
%token<node> ASGN

%token <node> START

%type <node> program declarations statementSequence statement
%type <node> assignment ifStatement whileStatement writeInt
%type <node> elseClause expression simpleExpression term factor
%type <val> type


%start program
%%

program:
	PROGRAM declarations START statementSequence END{
	$$ = make_node($2, $4);
        gen_code($2, $4);
	};

declarations:
	VAR ident AS type SC declarations{
	   $$ = make_decl($2, $4, $6);
	 //  add_sym(count++, $2, $4);
	}
	| /* empty */ {$$ = NULL;};

type:
	INT {$$ = 0;}
	| BOOL{$$ = 1;};

statementSequence:
	statement SC statementSequence{ $$ = make_stmt_seq($1, $3);}
	| /* empty */ {$$ = NULL;};

statement:
	assignment { $$ = make_stmt($1);}
	| ifStatement{$$ = make_stmt($1); }
	| whileStatement { $$ = make_stmt($1);}
	| writeInt { $$ = make_stmt($1);}
	;

assignment:
	ident ASGN expression { $$ = make_asgn($1, $3);}
	| ident ASGN READINT { $$ = make_asgn_rd($1);}
	;


ifStatement:
	IF expression THEN statementSequence elseClause END { $$ = make_if($2, $4, $5); }
	;

elseClause:
	ELSE statementSequence {$$ = make_else($2); }
	| { $$ = NULL; }
	;

whileStatement:
	WHILE expression DO statementSequence END { $$ = make_while($2, $4); }
	;

writeInt:
	WRITEINT expression { $$ = make_write_int($2); }
	;

expression:
	simpleExpression { $$ = make_expr($1); }
	| simpleExpression OP4 simpleExpression { $$ = make_expr2($1, $2, $3);}
	;

simpleExpression:
	term OP3 term {$$ = make_expr_s2($1, $2, $3);}
	| term {$$ = make_expr_s($1); }
	;

term:
	factor OP2 factor {$$ = make_term2($1, $2, $3); }
	| factor {$$ = make_term($1); }
	;

factor:
	ident {$$ = make_factor_ident($1); }
	| NUM {$$ = make_factor_num($1); }
	| boollit { $$ = make_factor_boollit("false"); printf("%s", $1); }
	| LP expression RP { $$ = make_factor_expr($2); }
	;
%%

int yyerror(char *s){
   printf("yyerror: %s\n",s);
}

int main(void){
   yyparse();
   printf("SUCCESS!\n");

}

