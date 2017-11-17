
#include <stdlib.h>
#include <stdio.h>
#include "imageprocessing.h"
#include <FreeImage.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>
#include <pthread.h>
#include <sys/time.h>
#include <math.h>

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

void brilhoDireto (imagem *I, float fator){

  int y, x, idx;
  float r, g, b;
  struct timeval rt0, rt1, drt;

  gettimeofday(&rt0, NULL);
  
  for (y=0; y<I->height; y++) {
    for (x=0; x<I->width; x++){
      idx = x + (y*I->width);
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
  gettimeofday(&rt1, NULL);
  timersub(&rt1, &rt0, &drt);
  printf("Tempo de multiplicação direta: %ld.%06ld segundos\n", drt.tv_sec, drt.tv_usec);
}

void multiplicaLinha (int i, imagem *I, float *r, float *g, float *b, float fator){
  int x, idx;
  float rPix, gPix, bPix;
  for (x=0; x<I->width; x++){
    idx = i*(I->width) + x;
    rPix = I->r[idx]*fator;
    gPix = I->g[idx]*fator;
    bPix = I->b[idx]*fator;
    if (rPix > 255){
      r[idx] = 255;        
    }
    else {
      r[idx] = rPix;
    }
    if (gPix > 255){
      g[idx] = 255;
    }
    else {
      g[idx] = gPix;
    }
    if (bPix > 255){
      b[idx] = 255;
    }
    else {
      b[idx] = bPix;
    }
  }
}

void atualizaImagem (imagem *I, float *r, float *g, float *b){

  int x, y, idx;

  for (y=0; y<I->height; y++){
    for (x=0; x<I->width; x++){
      idx = x + y*(I->width);
      I->r[idx] = r[idx];
      I->g[idx] = g[idx];
      I->b[idx] = b[idx];
    }
  }
}

void brilhoProcesso (imagem *I, float fator){

  int n = ((int)((I->height)/pow(2,10)) > 0) ? (int)((I->height)/pow(2,10)) : 1; //Número de processos = 2*n   
  pid_t *pids = malloc(sizeof(pid_t)*n);
  int i, j, k, status;
  int protection = PROT_READ | PROT_WRITE;
  int visibility = MAP_SHARED | MAP_ANON;
  float *r = (float*) mmap(NULL, sizeof(float)*(I->height)*(I->width), protection, visibility, 0, 0);
  float *g = (float*) mmap(NULL, sizeof(float)*(I->height)*(I->width), protection, visibility, 0, 0);
  float *b = (float*) mmap(NULL, sizeof(float)*(I->height)*(I->width), protection, visibility, 0, 0);
  struct timeval rt0, rt1, drt;

  gettimeofday(&rt0, NULL);

  for (i=0; i<n; i++){
    pids[i] = fork();
    if (pids[i] < 0){
      printf ("Erro ao fazer fork\n");
      exit (EXIT_FAILURE);
    }
    else if (pids[i] == 0){
      for (k=2*i; k<I->height; k+=2*n){
        multiplicaLinha (k, I, r, g, b, fator);
      }            
      exit (EXIT_SUCCESS);
    }
    else {
      for (k=(2*i)+1; k<I->height; k+=2*n){
        multiplicaLinha (k, I, r, g, b, fator);
      }           
    }
  }  

  i=0;

  while (i<n){
    waitpid (pids[i], &status, 0);
    i++;
  }

  gettimeofday(&rt1, NULL);
  timersub(&rt1, &rt0, &drt);
  atualizaImagem (I, r, g, b);
  printf("Tempo de multiplicação por %d processos: %ld.%06ld segundos\n", 2*n, drt.tv_sec, drt.tv_usec);
}

void brilho_imagem (imagem *I, float fator){
  brilhoDireto (I, 1/(fator*fator));
  brilhoProcesso (I, fator);
}