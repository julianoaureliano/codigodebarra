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

void colocar_espacamento_lateral(int *espacamento_lateral, int *todososdigitos, int *index_digitos)
{
    for (int i = 0; i < *espacamento_lateral; i++)
    {
        todososdigitos[*index_digitos] = 0;
        (*index_digitos)++;
    }
}

void processar_digitos_codigo_de_barra(int *todososdigitos, int *index_digitos, char *Lcode[], char *Rcode[], int digitos[])
{
    // "101" inicial
    todososdigitos[*index_digitos] = 1;
    (*index_digitos)++;
    todososdigitos[*index_digitos] = 0;
    (*index_digitos)++;
    todososdigitos[*index_digitos] = 1;
    (*index_digitos)++;

    // processa os 8 dígitos do código de barras
    for (int i = 0; i < 8; i++)
    {
        if (i < 4)
        {
            // lado esquerdo usa Lcode
            for (int j = 0; j < 7; j++)
            {
                todososdigitos[*index_digitos] = Lcode[digitos[i]][j] - '0'; // guarda cada dígito do Lcode convertendo de string para int
                (*index_digitos)++;
            }
        }
        else
        {
            if (i == 4)
            {
                // "01010" no meio
                todososdigitos[*index_digitos] = 0;
                (*index_digitos)++;
                todososdigitos[*index_digitos] = 1;
                (*index_digitos)++;
                todososdigitos[*index_digitos] = 0;
                (*index_digitos)++;
                todososdigitos[*index_digitos] = 1;
                (*index_digitos)++;
                todososdigitos[*index_digitos] = 0;
                (*index_digitos)++;
            }

            // lado direito usa Rcode
            for (int j = 0; j < 7; j++)
            {
                todososdigitos[*index_digitos] = Rcode[digitos[i]][j] - '0'; // guarda cada dígito do Rcode convertendo de string para int
                (*index_digitos)++;
            }
        }
    }

    // "101" final
    todososdigitos[*index_digitos] = 1;
    (*index_digitos)++;
    todososdigitos[*index_digitos] = 0;
    (*index_digitos)++;
    todososdigitos[*index_digitos] = 1;
    (*index_digitos)++;
}

int main()
{
    // tabela Lcode e Rcode
    char *Rcode[10] = {
        "1110010", "1100110", "1101100", "1000010",
        "1011100", "1001110", "1010000", "1000100",
        "1001000", "1110100"};
    char *Lcode[10] = {
        "0001101", "0011001", "0010011", "0111101",
        "0100011", "0110001", "0101111", "0111011",
        "0110111", "0001011"};

    int digitos[8];
    int todososdigitos[200];
    int index_digitos = 0;
    int espacamento_lateral;
    char numero_codigo_de_barra[9];
    int altura_dos_pixels;

    printf("Digite um número: ");
    scanf("%8s", numero_codigo_de_barra);

    if (strlen(numero_codigo_de_barra) != 8)
    {
        printf("O número do código de barras não tem 8 dígitos\n");
        return 1;
    }

    printf("Digite o espaçamento lateral: ");
    scanf("%d", &espacamento_lateral);

    printf("Digite a altura dos pixels: ");
    scanf("%d", &altura_dos_pixels);

    colocar_espacamento_lateral(&espacamento_lateral, todososdigitos, &index_digitos);

    for (int i = 0; i < 8; i++)
    {
        digitos[i] = numero_codigo_de_barra[i] - '0'; // converte a string para int
    }

    processar_digitos_codigo_de_barra(todososdigitos, &index_digitos, Lcode, Rcode, digitos);

    colocar_espacamento_lateral(&espacamento_lateral, todososdigitos, &index_digitos);

    int pixel = 3;
    int pixels_tratados[1000];
    char alteracao_pixel[1];
    // pixel--;

    printf("Deseja alterar o padrao dos pixels? (Padrao eh 3) y ou n : ");
    scanf("%1s", alteracao_pixel);
    if (alteracao_pixel[0] == 'y')
    {
        printf("Diga o padrao dos pixels: ");
        scanf("%d", &pixel);
    }

    int index_dos_pixels_tratados = 0;

    // printf("Dígitos do código de barras: ");
    for (int i = 0; i < index_digitos; i++)
    {
        // começa a multiplicar os pixels depos do espaçamento lateral esquerdo e antes do direito e guardando num novo vetor
        if (i >= (espacamento_lateral) && i < ((index_digitos) - (espacamento_lateral)))
        {
            for (int j = 1; j < pixel; j++)
            {
                // printf("%d", todososdigitos[i]);
                pixels_tratados[index_dos_pixels_tratados++] = todososdigitos[i];
            }
        }
        /// printf("%d", todososdigitos[i]);
        pixels_tratados[index_dos_pixels_tratados++] = todososdigitos[i];
    }
    // printf("\nTotal de dígitos: %d\n", index_digitos);

    printf("linha e altura: %d %d\n", index_dos_pixels_tratados, (altura_dos_pixels + (espacamento_lateral * 2)));

    printf("Dígitos do código de barras duplicado: \n");

    for (int j = 0; j < (altura_dos_pixels + (espacamento_lateral * 2)); j++)
    {
        for (int i = 0; i < index_dos_pixels_tratados; i++)
        {
            if (j < espacamento_lateral || j >= (altura_dos_pixels + espacamento_lateral))
            {
                printf("%d", pixels_tratados[i] * 0);
            }
            else
            {
                printf("%d", pixels_tratados[i]);
            }
        }
        printf("\n");
    }

    return 0;
}
