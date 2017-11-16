
#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H


typedef struct {
  unsigned int width, height;
  float *r, *g, *b;
} imagem;

imagem abrir_imagem(char *nome_do_arquivo);
void salvar_imagem(char *nome_do_arquivo, imagem *I);
void liberar_imagem(imagem *i);
void brilho_imagem (imagem *I, float fator);
void vmax_imagem (imagem *I, float vmax[3]);
void imprime (imagem *I);

#endif
