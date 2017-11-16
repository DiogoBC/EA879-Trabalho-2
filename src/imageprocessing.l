
%{
#include <stdlib.h>
#include <string.h>
void yyerror(char *);

#include "y.tab.h"
%}



%%

[a-z]	{
	yylval.ival = yytext[0] - 'a';
    return VAR;
}

\"	{
	return ASPA; 
}

([0-9]*|[0-9].[0-9]+)	{
	for (int i = 0; i < yyleng; i++){
		yylval.strval[i] = yytext[i];
	}
	yylval.strval[yyleng] = '\0';
	printf ("Achei fator string: %s\n", yylval.strval);
	return FATOR;
}


[a-zA-Z0-9\.]+	{
	for (int i = 0; i < yyleng; i++){
		yylval.strval[i] = yytext[i];
	}
	yylval.strval[yyleng] = '\0';
    printf ("Achei nome: %s\n", yylval.strval);
    return STRING;
}

\[	{
	return ACOL;
}

\]	{
	return FCOL;
}

[*]	{
	return PRODUTO;
}

[/]	{
	return DIVISAO;
}

[=]	{
	return IGUAL;
}

\n 	{
	return EOL;
}

[ \t]+ ;

.	{
	printf("**%s**\n", yytext);
	yyerror("Token nao reconhecido!\n");
}

%%

int  yywrap() {
  return 1;
}
