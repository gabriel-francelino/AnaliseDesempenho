#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>

double uniforme() {
	double u = rand() / ((double) RAND_MAX + 1);
	//limitando entre (0,1]
	u = 1.0 - u;

	return (u);
}

double min(double d1, double d2){
    return (d1 < d2) ? d1 :  d2;
}

int main(){
    // iniciando a semente p/ a geracao dos numeros
    //pseudo-aleatorios
    int semente = time(NULL);
    // int semente = 10;
    srand(semente);

    //Na Exponencial, E[X] = 1/l
    // E[X] = 5
    // 1/l = 5
    // l = 1/5
    // l = 0.2
    double media_chegada;
    printf("Informe o tempo medio de chegada entre clientes (s): ");
    scanf("%lF", &media_chegada);
    media_chegada = 1.0/media_chegada;

    double media_servico;
    printf("Informe o tempo medio de serviço (s): ");
    scanf("%lF", &media_servico);
    media_servico = 1.0/media_servico;

    double tempo_simulacao;
    printf("Informe o tempo a ser simulado (s): ");
    scanf("%lF", &tempo_simulacao);
    tempo_simulacao = 1.0/tempo_simulacao;

    double tempo_decorrido = 0.0;

    double tempo_chegada = (-1.0/ media_chegada) * log(uniforme());
    double tempo_saida = DBL_MAX;
    // printf("%lF\n", tempo_saida);

    unsigned long int fila = 0;

    while (tempo_decorrido < tempo_simulacao)
    {
        tempo_decorrido = min(tempo_chegada, tempo_saida);
        // printf("tempo chegada: %lF\n", tempo_chegada);
        // printf("tempo saida: %lF\n", tempo_saida);
        // printf("tempo decorrido: %lF\n", tempo_decorrido);
        // printf("boolean: %d\n", tempo_decorrido == tempo_chegada);
        // getchar();
        // getchar();
        
        if (tempo_decorrido == tempo_chegada)
        {
            // chegada
            if (fila)
            {
                fila++;
            }
            else
            {
                tempo_saida = tempo_decorrido + (-1.0/ media_servico) * log(uniforme());
            }      

            tempo_chegada = tempo_decorrido + (-1.0/ media_chegada) * log(uniforme());      
            
        }
        else if (tempo_decorrido == tempo_saida)
        {
            // saida
            if (fila)
            {
                fila--;
                tempo_saida = tempo_decorrido + (-1.0/ media_servico) * log(uniforme());
            }
            else
            {
                tempo_saida = DBL_MAX;
            } 
        }
        else
        {
            printf("Evento inválido!\n");
            return(1);
        }    
    }
    

    // double valor = (-1.0/ l) * log(uniforme());

}
