#include <stdio.h>
#include <stdlib.h>
int part = 444;

typedef struct
{
    int status;
    int comeco;
    int setores;
    int fim;
    unsigned int tamanho;
} Particao;

Particao particoes[4];

void openFile(unsigned char buff[], int sz)
{
    char retorno, retorno2;
    int ativos = 0;

    FILE *arq = fopen("mbr.bin", "rb");
    if (arq)
    {
        fread(buff, 512, 1, arq);
        unsigned int signature = buff[part + 66] | buff[part + 67] << 8;

        if (signature == 0xaa55)
        {
            printf("\n\nMBR Signature: %x\n\n", signature);

            // hexdump
            for (int j = 0; j < 512; j++)
            {
                printf("%02hx", (unsigned char)buff[j]);
                printf("%02hx\t", (unsigned char)buff[j + 1]);
                if (j == part)
                {
                    printf("\n\n------- Particao 1 -------\n\n");
                    if ((unsigned char)buff[part + 2] == 0x80)
                    {
                        particoes[0].status = 1;
                    }
                    unsigned int start = buff[part + 10] | buff[part + 11] << 8;
                    unsigned int setores_hex = buff[part + 17] << 24 | buff[part + 16] << 16 | buff[part + 15] << 8 | buff[part + 14];
                    unsigned int fim = setores_hex + (start - 1);
                    particoes[0].comeco = (int)start;
                    particoes[0].setores = (int)setores_hex;
                    particoes[0].fim = (int)fim;
                }
                if (j == part + 16)
                {
                    printf("\n\n------- Particao 2 -------\n\n");
                    if ((unsigned char)buff[part + 18] == 0x80)
                    {
                        ativos += 1;
                    }
                }
                if (j == part + 32)
                {
                    printf("\n\n------- Particao 3 -------\n\n");
                    if ((unsigned char)buff[part + 34] == 0x80)
                    {
                        ativos += 1;
                    }
                }
                if (j == part + 48)
                {
                    printf("\n\n------- Particao 4 -------\n\n");
                    if ((unsigned char)buff[part + 50] == 0x80)
                    {
                        ativos += 1;
                    }
                }
                if (j == part + 64)
                {
                    printf("\n\n----- Boot Signature -----\n\n");
                }

                j++;
            }
        }
        else
        {
            printf("Non MBR Signature");
        }
        // variaveis do disco
        unsigned int id = buff[part - 1] << 24 | buff[part - 2] << 16 | buff[part - 3] << 8 | buff[part - 4];
        printf("\n\nIdentificador do disco: 0x%0x\n", id);
    }
    else
    {
        printf("\nErro na Leitura do arquivo");
    }
}

int main(void)
{
    unsigned char buff[512];
    openFile(buff, 512);

    printf("\nDispositivo Inicializar   Início      Fim  Setores Tamanho Id Tipo");
    printf("\n/dev/sda1   *        %d          %d   %d 32G 83 Linux", particoes[0].comeco, particoes[0].fim, particoes[0].setores);

    // mbr signature
}