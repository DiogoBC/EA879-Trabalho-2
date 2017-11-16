%{
#include <stdio.h>
#include <stdlib.h>
#include "imageprocessing.h"
#include <FreeImage.h>

void yyerror(char *c);
int yylex(void);
float vmax[3];

%}
%union {
  char    strval[50];
  int     ival;   
}
%token <strval> STRING FATOR
%token <ival> VAR IGUAL EOL ASPA ACOL FCOL 
%left SOMA PRODUTO DIVISAO

%%

PROGRAMA:
        PROGRAMA EXPRESSAO EOL
        |
        ;

EXPRESSAO:
    | STRING IGUAL STRING {
        printf("Copiando %s para %s\n", $3, $1);
        imagem I = abrir_imagem($3);
        printf("Li imagem %d por %d\n", I.width, I.height);
        salvar_imagem($1, &I);
        liberar_imagem(&I);
    }
    | STRING IGUAL STRING PRODUTO FATOR {
    	imagem I = abrir_imagem ($3);
    	printf ("Fator float = %f\n", atof($5));
    	brilho_imagem (&I, atof($5));     
    	salvar_imagem ($1, &I);    	
    	liberar_imagem (&I);    	
    }
    | STRING IGUAL STRING DIVISAO FATOR {
    	imagem I = abrir_imagem ($3);
    	printf ("Fator float = %f\n", 1/atof($5));    	
    	brilho_imagem (&I, 1/atof($5));    	
    	salvar_imagem ($1, &I);    	
    	liberar_imagem (&I);
    }    
    | ACOL STRING FCOL {
    	imagem I = abrir_imagem ($2);
    	vmax_imagem (&I, &vmax[0]);
    	printf ("Valor máximo r: %.2f\nValor máximo g: %.2f\nValor máximo b: %.2f\n", vmax[0], vmax[1], vmax[2]);
    } 

    ;

%%

void yyerror(char *s) {
    fprintf(stderr, "%s\n", s);
}

int main() {
  FreeImage_Initialise(0);
  yyparse();
  return 0;
}