
#include <stdlib.h>
#include <stdio.h>

#include "imageprocessing.h"

#include <FreeImage.h>

/*
imagem abrir_imagem(char *nome_do_arquivo);
void salvar_imagem(char *nome_do_arquivo);
void liberar_imagem(imagem *i);
 */

imagem abrir_imagem(char *nome_do_arquivo) {
  FIBITMAP *bitmapIn;
  int x, y;
  RGBQUAD color;
  imagem I;

  bitmapIn = FreeImage_Load(FIF_JPEG, nome_do_arquivo, 0);

  if (bitmapIn == 0) {
    printf("Erro! Nao achei arquivo - %s\n", nome_do_arquivo);
  } else {
    printf("Arquivo lido corretamente!\n");
   }

  x = FreeImage_GetWidth(bitmapIn);
  y = FreeImage_GetHeight(bitmapIn);

  I.width = x;
  I.height = y;
  I.r = malloc(sizeof(float) * x * y);
  I.g = malloc(sizeof(float) * x * y);
  I.b = malloc(sizeof(float) * x * y);

   for (int i=0; i<x; i++) {
     for (int j=0; j <y; j++) {
      int idx;
      FreeImage_GetPixelColor(bitmapIn, i, j, &color);

      idx = i + (j*x);

      I.r[idx] = (float)color.rgbRed;
      I.g[idx] = (float)color.rgbGreen;
      I.b[idx] = (float)color.rgbBlue;
    }
   }
  return I;

}

void liberar_imagem(imagem *I) {
  free(I->r);
  free(I->g);
  free(I->b);
}

void salvar_imagem(char *nome_do_arquivo, imagem *I) {
  FIBITMAP *bitmapOut;
  RGBQUAD color;

  printf("Salvando imagem %d por %d...\n", I->width, I->height);
  bitmapOut = FreeImage_Allocate(I->width, I->height, 24, 0, 0, 0);

   for (int i=0; i<I->width; i++) {
     for (int j=0; j<I->height; j++) {
      int idx;

      idx = i + (j*I->width);
      color.rgbRed = (float)I->r[idx];
      color.rgbGreen = (float)I->g[idx];
      color.rgbBlue = (float)I->b[idx];

      FreeImage_SetPixelColor(bitmapOut, i, j, &color);
    }
  }

  FreeImage_Save(FIF_JPEG, bitmapOut, nome_do_arquivo, JPEG_DEFAULT);
}

void brilho_imagem (imagem *I, float fator) {

	int i, j, idx;
	float r, g, b;
	
	for (i=0; i<I->width; i++) {
		for (j=0; j<I->height; j++){
			idx = i + (j*I->width);
			r = I->r[idx] * fator;
			g = I->g[idx] * fator;
			b = I->b[idx] * fator;			
			if  (r > 255){
				I->r[idx] = 255;				
			}
			else {
				I->r[idx] = r;
			}
			if (g > 255){
				I->g[idx] =  255;
			}
			else {
				I->g[idx] = g;
			}
			if (b > 255){
				I->b[idx] = 255;
			}
			else {
				I->b[idx] = b;
			}		
						
		}
	}
}

void vmax_imagem (imagem *I, float vmax[3]){

	int i, j, idx;
	vmax[0] = 0.0; 
	vmax[1] = 0.0;
	vmax[2] = 0.0;
	
	for (i=0; i<I->width; i++) {
		for (j=0; j<I->height; j++){
			idx = i + (j*I->width);			
			if (I->r[idx] >= vmax[0]){
				vmax[0] = I->r[idx];
			}
			if (I->g[idx] >= vmax[1]){
				vmax[1] = I->g[idx];
			}
			if (I->b[idx] >= vmax[2]){
				vmax[2] = I->b[idx];
			}			
		}
	}	
}

void imprime (imagem *I){

	int i, j, idx;

	for (i=0; i<I->width; i++) {
		for (j=0; j<I->height; j++){
			idx = i + (j*I->width);			
			if (I->r[idx] - (int)I->r[idx] != 0 | I->g[idx] - (int)I->g[idx] != 0 | I->b[idx] - (int)I->b[idx] != 0){
				printf ("%f %f %f\n", I->r[idx], I->g[idx], I->b[idx]);
			}			
		}
	}
	printf ("fim de impressão\n");
}