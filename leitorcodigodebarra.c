#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include "codigo_barras.h"

void tirar_espacamento_lateral(CodigoBarraDados *dados, ConfiguracaoCodigoBarra *config, int *quantidade_de_pixels)
{ // vejo a quantidade do espaçamento lateral
    for (int i = 0; i < dados->index_pixels_tratados; i++)
    {
        if (dados->pixels_tratados[i] == 0)
        {
            config->espacamento_lateral++;
        }
        else
        {
            break;
        }
    }
    // vejo a resoluçao dos pixels
    for (int i = config->espacamento_lateral; i < dados->index_pixels_tratados; i++)
    {
        if (dados->pixels_tratados[i] == 1)
        {
            config->altura_dos_pixels++;
        }
        else
        {
            break;
        }
    }
    // com base na resolução dos pixels faço o for encrementado por ele e pego os 67 digitos originais
    for (int i = config->espacamento_lateral; i < dados->index_pixels_tratados - config->espacamento_lateral; i += config->altura_dos_pixels)
    {
        printf("%d", dados->pixels_tratados[i]);
        dados->todos_os_digitos[dados->index_digitos] = dados->pixels_tratados[i];
        dados->index_digitos++;
    }
}

int main()
{
    setlocale(LC_ALL, "Portuguese_Brazil");
    CodigoBarraDados dados = {.index_digitos = 0, .index_pixels_tratados = 0};
    ConfiguracaoCodigoBarra config = {.espacamento_lateral = 0, .altura_dos_pixels = 0};
    int quantidade_de_pixels = 0;

    FILE *file = fopen("codigo_de_barras.pbm", "r");
    if (!file)
    {
        perror("Erro ao abrir o arquivo");
        return 1;
    }

    // Ignorando o cabeçalho
    char tipo[3];
    int largura, altura;
    fscanf(file, "%s", tipo); // Tipo da imagem P1
    fscanf(file, "%d %d", &largura, &altura);

    // A linha do meio
    int meio = altura / 2;
    int totalPixels = largura * altura;
    int *pixels = (int *)malloc(totalPixels * sizeof(int));

    // Lendo os dados dos pixels
    for (int i = 0; i < totalPixels; i++)
    {
        fscanf(file, "%d", &pixels[i]);
    }

    // pegando a linha do meio e guardando no vetor
    for (int i = 0; i < largura; i++)
    {
        printf("%d ", pixels[meio * largura + i]);
        dados.pixels_tratados[dados.index_pixels_tratados] = pixels[meio * largura + i];
        dados.index_pixels_tratados++;
    }

    printf("\n");
    printf("%d", largura);

    for (int i = 0; i < dados.index_pixels_tratados; i++)
    {
        printf("%d", dados.pixels_tratados[i]);
    }

    printf("\n");
    tirar_espacamento_lateral(&dados, &config, &quantidade_de_pixels);

    printf("\n");
    printf("Espacamento lateral do codigo eh %d e a resolucao dos pixels eh %d tamanho da linha configurada %d", config.espacamento_lateral, config.altura_dos_pixels, dados.index_digitos);
    // Liberando memória e fechando o arquivo
    free(pixels);
    fclose(file);

    return 0;
}
