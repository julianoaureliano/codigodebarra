#ifndef CODIGO_BARRAS_H
#define CODIGO_BARRAS_H

// Estruturas
typedef struct
{
    char *Lcode[10];
    char *Rcode[10];
} CodigoBarraTabela;

typedef struct
{
    int espacamento_lateral;
    int altura_dos_pixels;
    char numero_codigo_de_barra[9];
    int digitos[8];
} ConfiguracaoCodigoBarra;

typedef struct
{
    int todos_os_digitos[200];
    int index_digitos;
    int pixels_tratados[1000];
    int index_pixels_tratados;
} CodigoBarraDados;

// Funções
void colocar_espacamento_lateral(int espacamento_lateral, CodigoBarraDados *dados);
void processar_digitos_codigo_de_barra(CodigoBarraTabela *tabela, CodigoBarraDados *dados, ConfiguracaoCodigoBarra *config);

#endif // CODIGO_BARRAS_H