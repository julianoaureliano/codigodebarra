// compila o arquivo shell : gcc arquivo.c -o nomedoarquivo -g(>sequiserdebugar)
// executa o arquivo : ./nomedoarquivo
// executa o arquivo com entradas : ./nomedoarquivo < entrada.txt
// executa o arquivo com entradas e saida: ./nomedoarquivo < entrada > saida
// (Git) git remote -v = para ver qual link do repositorio está linkado
// (Git) git status = para ver as atualizaçoes e mudancas dos arquivos
// *salve o arquivo antes*(Git) git add . = para carregar todos os arquivos para o commit
// (Git) git commit -m "colocarmensagem" = carregar os arquivos para enviar com a mensagem
// (Git) git push origin main = enviar os arquivos atualizados
// (Git) git fetch = "sincroniza" os repositorios, para saber se tem alguma atualização no repositorio
// (Git) git pull origin main = para baixar/atualizar os arquivos locais com o do repositorio do github
#include <stdio.h>
#include <math.h>
#include <string.h>

// adcicionando as structs
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

// adiciona os 0 do espaçamento lateral
void colocar_espacamento_lateral(int espacamento_lateral, CodigoBarraDados *dados)
{
    for (int i = 0; i < espacamento_lateral; i++)
    {
        dados->todos_os_digitos[dados->index_digitos] = 0;
        dados->index_digitos++;
    }
}

void processar_digitos_codigo_de_barra(CodigoBarraTabela *tabela, CodigoBarraDados *dados, ConfiguracaoCodigoBarra *config)
{
    // "101" inicial
    dados->todos_os_digitos[dados->index_digitos] = 1;
    dados->index_digitos++;
    dados->todos_os_digitos[dados->index_digitos] = 0;
    dados->index_digitos++;
    dados->todos_os_digitos[dados->index_digitos] = 1;
    dados->index_digitos++;

    // Processa os 8 dígitos do código de barras
    for (int i = 0; i < 8; i++)
    {
        if (i < 4)
        {
            // Lado esquerdo usa Lcode
            for (int j = 0; j < 7; j++)
            {
                dados->todos_os_digitos[dados->index_digitos] = tabela->Lcode[config->digitos[i]][j] - '0';
                dados->index_digitos++;
            }
        }
        else
        {
            if (i == 4)
            {
                // "01010" no meio
                int meio[] = {0, 1, 0, 1, 0};
                for (int j = 0; j < 5; j++)
                {
                    dados->todos_os_digitos[dados->index_digitos] = meio[j];
                    dados->index_digitos++;
                }
            }
            // Lado direito usa Rcode
            for (int j = 0; j < 7; j++)
            {
                dados->todos_os_digitos[dados->index_digitos] = tabela->Rcode[config->digitos[i]][j] - '0';
                dados->index_digitos++;
            }
        }
    }

    // "101" final
    dados->todos_os_digitos[dados->index_digitos] = 1;
    dados->index_digitos++;
    dados->todos_os_digitos[dados->index_digitos] = 0;
    dados->index_digitos++;
    dados->todos_os_digitos[dados->index_digitos] = 1;
    dados->index_digitos++;
}

int main()
{
    // Inicialização das tabelas
    CodigoBarraTabela tabela = {
        .Lcode = {"0001101", "0011001", "0010011", "0111101", "0100011", "0110001", "0101111", "0111011", "0110111", "0001011"},
        .Rcode = {"1110010", "1100110", "1101100", "1000010", "1011100", "1001110", "1010000", "1000100", "1001000", "1110100"}};

    // Inicialização da configuração
    ConfiguracaoCodigoBarra config;
    CodigoBarraDados dados = {.index_digitos = 0, .index_pixels_tratados = 0};

    printf("Digite um número: ");
    scanf("%8s", config.numero_codigo_de_barra);

    if (strlen(config.numero_codigo_de_barra) != 8)
    {
        printf("O número do código de barras não tem 8 dígitos\n");
        return 1;
    }

    printf("Digite o espaçamento lateral: ");
    scanf("%d", &config.espacamento_lateral);

    printf("Digite a altura dos pixels: ");
    scanf("%d", &config.altura_dos_pixels);

    colocar_espacamento_lateral(config.espacamento_lateral, &dados);

    for (int i = 0; i < 8; i++)
    {
        config.digitos[i] = config.numero_codigo_de_barra[i] - '0'; // converte a string para int
    }

    processar_digitos_codigo_de_barra(&tabela, &dados, &config);

    colocar_espacamento_lateral(config.espacamento_lateral, &dados);

    int pixel = 3; // valor padrao
    char alteracao_pixel[2];

    printf("Deseja alterar o padrão dos pixels? (Padrão eh 3) y ou n : ");
    scanf("%1s", alteracao_pixel);
    if (alteracao_pixel[0] == 'y')
    {
        printf("Diga o padrão dos pixels: ");
        scanf("%d", &pixel);
        if (pixel < 1) // garatir o pixel valido
        {
            printf("O valor do padrão de pixels deve ser maior ou igual a 1. Usando o padrão 3.\n");
            pixel = 3;
        }
    }

    printf("Valor final de pixel: %d\n", pixel);

    for (int i = 0; i < dados.index_digitos; i++)
    {
        // começa a multiplicar os pixels depos do espaçamento lateral esquerdo e antes do direito. E guardando num novo vetor
        if (i >= (config.espacamento_lateral) && i < ((dados.index_digitos) - (config.espacamento_lateral)))
        {
            for (int j = 1; j < pixel; j++)
            {
                dados.pixels_tratados[dados.index_pixels_tratados] = dados.todos_os_digitos[i];
                dados.index_pixels_tratados++;
            }
        }
        dados.pixels_tratados[dados.index_pixels_tratados] = dados.todos_os_digitos[i];
        dados.index_pixels_tratados++;
    }

    printf("Linha e altura: %d %d\n", dados.index_pixels_tratados, (config.altura_dos_pixels + (config.espacamento_lateral * 2)));

    printf("Dígitos do código de barras duplicado: \n");

    for (int j = 0; j < (config.altura_dos_pixels + (config.espacamento_lateral * 2)); j++)
    {
        for (int i = 0; i < dados.index_pixels_tratados; i++)
        {
            // faz as linhas de 0 pela quantidade do espaçamento lateral
            if (j < config.espacamento_lateral || j >= (config.altura_dos_pixels + config.espacamento_lateral))
            {
                printf("%d", dados.pixels_tratados[i] * 0);
            }
            else
            {
                printf("%d", dados.pixels_tratados[i]);
            }
        }
        printf("\n");
    }

    return 0;
}
