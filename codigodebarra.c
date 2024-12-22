// compila o arquivo shell : gcc arquivo.c -o nomedoarquivo -g(>sequiserdebugar)
// executa o arquivo : ./nomedoarquivo
// executa o arquivo com entradas : ./nomedoarquivo < entrada.txt
// executa o arquivo com entradas e saida: ./nomedoarquivo < entrada > saida
// (Git) git status = para ver as atualizaçoes e mudancas dos arquivos
// *salve o arquivo antes*(Git) git add . = para carregar todos os arquivos para o commit
// (Git) git commit -m "colocarmensagem" = carregar os arquivos para enviar com a mensagem
// (Git) git push origin main = enviar os arquivos atualizados
// (Git) git fetch = "sincroniza" os repositorios, para saber se tem alguma atualização no repositorio
// (Git) git pull origin main = para baixar/atualizar os arquivos locais com o do repositorio do github
#include <stdio.h>
#include <math.h>
#include <string.h>

// separar cada digito por digito em um vetor para comparar com as tabelas dos valores

int main()
{
    // Armazenando os valores binários como strings
    char *Rcode[10] = {
        "1110010", "1100110", "1101100", "1000010",
        "1011100", "1001110", "1010000", "1000100",
        "1001000", "1110100"};
    char *Lcode[10] = {
        "0001101", "0011001", "0010011", "0111101",
        "0100011", "0110001", "0101111", "0111011",
        "0110111", "0001011"};

    int digitos[8];

    char numero[9];

    printf("Digite um número: ");
    scanf("%8s", numero);
    if (strlen(numero) != 8)
    {
        printf("O numero nao tem 8 digitos");
        return 1;
    }
    for (int i = 0; i < 8; i++)
    {
        digitos[i] = numero[i] - '0'; // converte string para numero e guarda no vetor
    }

    printf("101 ");

    for (int i = 0; i < 9; i++)
    {
        if (i < 4)
        {
            printf("%s ", Lcode[digitos[i]]);
        }
        if (i >= 4)
        {
            if (i == 4)
            {
                printf("01010 ");
            }
            printf("%s ", Rcode[digitos[i]]);
        }
    }
    printf("101 ");

    return 0;
}
