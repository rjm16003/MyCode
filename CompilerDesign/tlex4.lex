%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

#include "test6.tab.h"

float maxint = INT_MAX;
%}


%%

\(   		{return LP;}

\)   		{return RP;}

:=		{return ASGN;}

;		{return SC;}

\*|div|mod	{yylval.sval = strdup(yytext); return OP2;}

\+|-		{yylval.sval = strdup(yytext); return(OP3);}

=|!=|<|>|<=|>=	{yylval.sval = strdup(yytext); return(OP4);}

if		{return(IF);}

then		{return THEN;}

else		{return ELSE;}

end		{return END;}

while		{return(WHILE);}

do		{return(DO);}

program		{return(PROGRAM);}

var		{return(VAR);}

as		{return(AS);}

int		{return(INT);}

bool		{return(BOOL);}

writeInt	{return(WRITEINT);}

readInt		{return(READINT);}

begin		{return START;}

([1-9][0-9]*)|0	{yylval.val = atoi(yytext);
		return(NUM);}

false|true	{yylval.sval = strdup(yytext);
		return(boollit);}

[A-Z][A-Z0-9]*	{yylval.sval = strdup(yytext);
		return(ident);}

.

%%

