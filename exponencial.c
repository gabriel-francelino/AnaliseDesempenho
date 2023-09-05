#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

double uniforme(){
    double u = rand() / ((double)RAND_MAX + 1.0);
    // limitando entre 0 e 1
    u = 1.0 - u;

    return (u);
}

int main(int argc, char const *argv[])
{
    // Na exponencial, E[x] = 1/l
    // E[X] = 5
    // 1/l = 5
    // l = 1/5
    // l = 0.2
    double l;
    double soma;
    printf("Informe o tempo medio em segundos: ");
    scanf("%lF", &l);
    l = 1.0/l;

    // variaveis para calculo da media final
    double some = 0.0;
    double qtd_valores_gerados = 1000000000.0;

    // iniciando a semente para geração de numeros pseudoaleatorios
    int semente = time(NULL);
    // semente = 10;
    srand(semente);

    int i;
    for (i = 0; i < qtd_valores_gerados; i++)
    {
        double valor = (-1.0/l) * log(uniforme());
        //printf("%lF\n", valor);
        //getchar();
        soma += valor;

    }
    printf("Media: %lF\n", (soma/qtd_valores_gerados));

    return 0;
}
