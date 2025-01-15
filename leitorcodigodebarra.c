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
// remover o 101 inicial e final do vetor
void remover_marcadores(CodigoBarraDados *dados)
{
    int inicial = 3;
    int final = 3;
    // desloca os elementos pra esquerda, ignorando os primeiros
    for (int i = 0; i < dados->index_digitos - inicial - final; i++)
    {
        dados->todos_os_digitos[i] = dados->todos_os_digitos[i + inicial];
    }
    dados->index_digitos -= (inicial + final);
}
// remover o 01010 no meio do vetor
void remover_marcador_central(CodigoBarraDados *dados)
{
    int inicioRemocao = 28; // sao 28 digitos antes do caracter central pois 7*4=28
    int quantidade = 5;     // tamanho do 01010
    // desloca os elementos do vetor para preencher a lacuna
    for (int i = inicioRemocao; i < dados->index_digitos - quantidade; i++)
    {
        dados->todos_os_digitos[i] = dados->todos_os_digitos[i + quantidade];
    }
    dados->index_digitos -= (quantidade);
}

void agrupar_em_strings(CodigoBarraDados *dados, char resultadoEmString[][8], int *tamanhoResultado)
{
    int grupos = dados->index_digitos / 7; // grupos de 7
    *tamanhoResultado = grupos;

    for (int i = 0; i < grupos; i++)
    {
        // Inicializa a string para o grupo atual
        for (int j = 0; j < 7; j++)
        {
            resultadoEmString[i][j] = '0' + dados->todos_os_digitos[i * 7 + j]; // Converte 0 ou 1 para caractere
        }
        resultadoEmString[i][7] = '\0'; // Termina a string com '\0'
    }
}
// vou comparar os grupos de string que fiz com o lcode e rcode, e guardo a posição que na verdade ja é o numero
void transformar_verificador(CodigoBarraTabela *tabela, ConfiguracaoCodigoBarra *config, char resultadoEmString[][8], int tamanhoResultado)
{
    for (int i = 0; i < tamanhoResultado; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            if (i < 5)
            {
                if (strcmp(resultadoEmString[i], tabela->Lcode[j]) == 0)
                {
                    config->digitos[i] = j;
                    break;
                }
            }
            else
            {
                if (strcmp(resultadoEmString[i], tabela->Rcode[j]) == 0)
                {
                    config->digitos[i] = j;
                    break;
                }
            }
        }
    }
}

int main()
{
    setlocale(LC_ALL, "Portuguese_Brazil");
    CodigoBarraDados dados = {.index_digitos = 0, .index_pixels_tratados = 0};
    ConfiguracaoCodigoBarra config = {.espacamento_lateral = 0, .altura_dos_pixels = 0};
    int quantidade_de_pixels = 0;
    CodigoBarraTabela tabela = {
        .Lcode = {"0001101", "0011001", "0010011", "0111101", "0100011", "0110001", "0101111", "0111011", "0110111", "0001011"},
        .Rcode = {"1110010", "1100110", "1101100", "1000010", "1011100", "1001110", "1010000", "1000100", "1001000", "1110100"}};

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
    remover_marcadores(&dados);
    remover_marcador_central(&dados);
    char resultadoEmString[dados.index_digitos / 7][8];
    int tamanhoResultado;
    agrupar_em_strings(&dados, resultadoEmString, &tamanhoResultado);
    printf("\n");

    for (int i = 0; i < dados.index_digitos; i++)
    {
        printf("%d", dados.todos_os_digitos[i]);
    }

    printf("\n");
    printf("Espacamento lateral do codigo eh %d e a resolucao dos pixels eh %d tamanho da linha configurada %d", config.espacamento_lateral, config.altura_dos_pixels, dados.index_digitos);
    printf("\n");
    // Imprime o resultado
    for (int i = 0; i < tamanhoResultado; i++)
    {
        printf("%s\n", resultadoEmString[i]);
    }
    transformar_verificador(&tabela, &config, resultadoEmString, tamanhoResultado);

    printf("Identificador do codigo de barras: ");
    for (int i = 0; i < 8; i++)
    {
        printf("%d", config.digitos[i]);
    }
    // liberando memória e fechando o arquivo
    free(pixels);
    fclose(file);

    return 0;
}
