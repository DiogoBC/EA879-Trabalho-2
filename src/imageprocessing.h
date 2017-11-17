
#ifndef IMAGEPROCESSING_H
#define IMAGEPROCESSING_H


typedef struct {
  unsigned int width, height;
  float *r, *g, *b;
} imagem;

imagem abrir_imagem(char *nome_do_arquivo);
void salvar_imagem(char *nome_do_arquivo, imagem *I);
void liberar_imagem(imagem *i);
void vmax_imagem (imagem *I, float vmax[3]);
void brilhoDireto (imagem *I, float fator);
void brilhoProcesso (imagem *I, float fator);
void multiplicaLinha (int i, imagem *I, float *r, float *g, float *b, float fator);
void atualizaImagem (imagem *I, float *r, float *g, float *b);
void brilho_imagem (imagem *I, float fator);

#endif
