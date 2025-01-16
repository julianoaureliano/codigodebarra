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
#include <string.h>
#include "codigo_barras.h"
#include <locale.h>

int calcular_digito_verificador(int *digitos)
{
    int soma = 0;
    for (int i = 0; i < 7; i++)
    {
        if (i % 2 == 0)
        {
            soma += digitos[i] * 3; // indice impar multiplica por 3
        }
        else
        {
            soma += digitos[i]; // indice par
        }
    }
    int digito_verificador = (10 - (soma % 10)) % 10;
    return digito_verificador;
}

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
    setlocale(LC_ALL, "Portuguese_Brazil");
    CodigoBarraTabela tabela = {
        .Lcode = {"0001101", "0011001", "0010011", "0111101", "0100011", "0110001", "0101111", "0111011", "0110111", "0001011"},
        .Rcode = {"1110010", "1100110", "1101100", "1000010", "1011100", "1001110", "1010000", "1000100", "1001000", "1110100"}};

    ConfiguracaoCodigoBarra config;
    CodigoBarraDados dados = {.index_digitos = 0, .index_pixels_tratados = 0};

    printf("Digite o codigo: ");
    scanf("%8s", config.numero_codigo_de_barra);

    if (strlen(config.numero_codigo_de_barra) != 8)
    {
        printf("O número do código de barras não tem 8 dígitos\n");
        return 1;
    }

    char alteracao_espacamento[2];
    config.espacamento_lateral = 3;
    printf("Deseja alterar o padrão do espacamento lateral? (Padrão é 3) y ou n: ");
    scanf("%1s", alteracao_espacamento);
    if (alteracao_espacamento[0] == 'y')
    {
        printf("Diga o espacamento lateral: ");
        scanf("%d", &config.espacamento_lateral);
        if (config.espacamento_lateral < 1)
        {
            printf("O valor do espacamento lateral deve ser maior ou igual a 1. Usando o padrão 3.\n");
            config.espacamento_lateral = 3;
        }
    }

    char alteracao_altura[2];
    config.altura_dos_pixels = 10;
    printf("Deseja alterar da altura dos pixels? (Padrão é 10) y ou n: ");
    scanf("%1s", alteracao_altura);
    if (alteracao_altura[0] == 'y')
    {
        printf("Diga o espacamento lateral: ");
        scanf("%d", &config.altura_dos_pixels);
        if (config.espacamento_lateral < 1)
        {
            printf("O valor da altura dos pixels deve ser maior ou igual a 1. Usando o padrão 10.\n");
            config.espacamento_lateral = 3;
        }
    }

    int pixel = 3;
    char alteracao_pixel[2];

    printf("Deseja alterar o padrão dos pixels? (Padrão é 3) y ou n: ");
    scanf("%1s", alteracao_pixel);
    if (alteracao_pixel[0] == 'y')
    {
        printf("Diga o padrão dos pixels: ");
        scanf("%d", &pixel);
        if (pixel < 1)
        {
            printf("O valor do padrão de pixels deve ser maior ou igual a 1. Usando o padrão 3.\n");
            pixel = 3;
        }
    }

    for (int i = 0; i < 8; i++)
    {
        config.digitos[i] = config.numero_codigo_de_barra[i] - '0'; // converte a string para inteiro
    }

    int digito_verificador_calculado = calcular_digito_verificador(config.digitos);

    // erifica o digito verificador
    if (config.digitos[7] != digito_verificador_calculado)
    {
        printf("Erro: o dígito verificador informado (%d) não corresponde ao esperado (%d).\n",
               config.digitos[7], digito_verificador_calculado);
        return 1;
    }

    colocar_espacamento_lateral(config.espacamento_lateral, &dados);
    processar_digitos_codigo_de_barra(&tabela, &dados, &config);
    colocar_espacamento_lateral(config.espacamento_lateral, &dados);

    // começa a multiplicar os pixels depos do espaçamento lateral esquerdo e antes do direito. E guardando num novo vetor
    for (int i = 0; i < dados.index_digitos; i++)
    {
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

    char nomeArquivo[256] = "codigo_de_barras.pbm";
    char alteracao_nome[2];
    printf("Deseja digitar o nome do arquivo?(padrao eh: codigo_de_barras.pbm ) y ou n: ");
    scanf("%1s", alteracao_nome);
    if (alteracao_nome[0] == 'y')
    {
        printf("Diga o nome do arquivo: ");
        scanf("%255s", nomeArquivo);
    }

    // ver se o arquivo ja existe:
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo)
    {
        fclose(arquivo); // fecha o arquivo que foi aberto para leitura
        printf("O arquivo '%s' já existe. Deseja sobrescrevê-lo? (y/n): ", nomeArquivo);
        char resposta;
        scanf(" %c", &resposta); // O espaço antes de %c ignora caracteres pendentes no buffer
        if (resposta == 'n' || resposta != 'y')
        {
            printf("Operação cancelada.\n");
            return 0;
        }
    }

    arquivo = fopen(nomeArquivo, "w");
    if (!arquivo)
    {
        perror("Erro ao criar o arquivo");
        return 1;
    }

    fprintf(arquivo, "P1\n");
    fprintf(arquivo, "%d %d\n", dados.index_pixels_tratados, config.altura_dos_pixels + (config.espacamento_lateral * 2));

    // imprime o codigo de barra completo com todas as linhas
    for (int j = 0; j < (config.altura_dos_pixels + (config.espacamento_lateral * 2)); j++)
    {
        for (int i = 0; i < dados.index_pixels_tratados; i++)
        {
            // faz as linhas de 0 pela quantidade do espaçamento lateral
            if (j < config.espacamento_lateral || j >= (config.altura_dos_pixels + config.espacamento_lateral))
            {
                fprintf(arquivo, "0 ");
            }
            else
            {
                fprintf(arquivo, "%d ", dados.pixels_tratados[i]);
            }
        }
        fprintf(arquivo, "\n");
    }

    fclose(arquivo);
    printf("Arquivo PBM gerado com sucesso: %s\n", nomeArquivo);
    return 0;
}
