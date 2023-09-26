#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <float.h>
#include <time.h>

// Define a estrutura de dados para armazenar os parâmetros da simulação.
typedef struct {
    double media_chegada;
    double media_servico;
    double tempo_simulacao;
} parametros;

// Define a estrutura de dados para calcular as métricas usando o método de Little.
typedef struct {
    unsigned long int no_eventos;
    double tempo_anterior;
    double soma_areas;
} little;

// Inicializa uma estrutura "little" com valores iniciais.
void inicia_little(little * l){
    l->no_eventos = 0;
    l->tempo_anterior = 0.0;
    l->soma_areas = 0.0;
}

// Lê os parâmetros da simulação da entrada do usuário.
void le_parametros(parametros * params){
    printf("Informe o tempo médio entre clientes (s): ");
    scanf("%lF", &params->media_chegada);
    params->media_chegada = 1.0 / params->media_chegada;

    printf("Informe o tempo médio de serviço (s): ");
    scanf("%lF", &params->media_servico);
    params->media_servico = 1.0 / params->media_servico;

    printf("Informe o tempo a ser simulado (s): ");
    scanf("%lF", &params->tempo_simulacao);
}

// Gera um número aleatório uniforme no intervalo (0, 1] usando a função rand.
double uniforme() {
    double u = rand() / ((double) RAND_MAX + 1);
    // Limitando entre (0,1]
    u = 1.0 - u;
    return u;
}

// Retorna o menor entre dois números.
double min(double d1, double d2){
    if(d1 < d2) return d1;
    return d2;
}

int main(){
    int semente = time(NULL);
    srand(semente);

    // Lê os parâmetros da simulação da entrada do usuário.
    parametros params;
    le_parametros(&params);

    // Variáveis de controle da simulação.
    double tempo_decorrido = 0.0;
    double tempo_chegada = (-1.0 / params.media_chegada) * log(uniforme());
    double tempo_saida = DBL_MAX;
    unsigned long int fila = 0;
    unsigned long int max_fila = 0;

    // Variáveis de medidas de interesse.
    double soma_ocupacao = 0.0;

    /**
     * Little
     */
    little e_n;
    little e_w_chegada;
    little e_w_saida;

    // Inicializa as estruturas "little" com valores iniciais.
    inicia_little(&e_n);
    inicia_little(&e_w_chegada);
    inicia_little(&e_w_saida);

    /**
     * Little -- fim
     */

    // Loop principal da simulação.
while(tempo_decorrido < params.tempo_simulacao){

    // Determina qual evento ocorre primeiro: a chegada de um cliente ou a saída de um cliente.
    tempo_decorrido = min(tempo_chegada, tempo_saida);

    // Se um cliente chega, atualiza as variáveis de controle e medidas de interesse.
    if(tempo_decorrido == tempo_chegada){

        // Calcula o tempo de saída do cliente.
        if(!fila){
            double tempo_servico = (-1.0 / params.media_servico) * log(uniforme());
            tempo_saida = tempo_decorrido + tempo_servico;
            soma_ocupacao += tempo_servico;
        }

        // Atualiza a variável de tamanho da fila.
        fila++;

        // Atualiza a variável de tamanho máximo da fila.
        max_fila = fila > max_fila ? fila : max_fila;

        // Atualiza a variável de tempo de chegada do próximo cliente.
        tempo_chegada = tempo_decorrido + (-1.0 / params.media_chegada) * log(uniforme());

        // Atualiza as áreas sob as curvas de E[N] e E[W] para o método de Little.
        e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
        e_n.no_eventos++;
        e_n.tempo_anterior = tempo_decorrido;

        e_w_chegada.soma_areas += (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
        e_w_chegada.no_eventos++;
        e_w_chegada.tempo_anterior = tempo_decorrido;

    // Se um cliente sai, atualiza as variáveis de controle e medidas de interesse.
    } else if(tempo_decorrido == tempo_saida){

        // Atualiza a variável de tamanho da fila.
        fila--;

        // Se ainda houver clientes na fila, atualiza a variável de tempo de saída do próximo cliente.
        if(fila){
            double tempo_servico = (-1.0 / params.media_servico) * log(uniforme());
            tempo_saida = tempo_decorrido + tempo_servico;
            soma_ocupacao += tempo_servico;
        } else {
            tempo_saida = DBL_MAX;
        }

        // Atualiza as áreas sob as curvas de E[N] e E[W] para o método de Little.
        e_n.soma_areas += (tempo_decorrido - e_n.tempo_anterior) * e_n.no_eventos;
        e_n.no_eventos--;
        e_n.tempo_anterior = tempo_decorrido;

        e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;
        e_w_saida.no_eventos++;
        e_w_saida.tempo_anterior = tempo_decorrido;

    // Se um evento inválido ocorrer, imprime uma mensagem de erro e retorna um erro.
    } else{
        printf("Evento inválido!\n");
        return(1);
    }
}

// Atualiza as áreas para o cálculo final do método de Little.
//
// A área sob a curva de E[W] é calculada como a soma das áreas de todos os retângulos que representam o tempo de espera dos clientes.
// Cada retângulo tem um comprimento igual ao tempo de espera do cliente e uma altura igual à taxa de chegada.

e_w_chegada.soma_areas += (tempo_decorrido - e_w_chegada.tempo_anterior) * e_w_chegada.no_eventos;
e_w_saida.soma_areas += (tempo_decorrido - e_w_saida.tempo_anterior) * e_w_saida.no_eventos;

// Calcula e exibe as métricas finais.
//
// A ocupação média da fila é calculada como a soma do tempo de serviço de todos os clientes dividido pelo tempo total de simulação.
// O tamanho máximo da fila é o maior número de clientes que já estiveram na fila ao mesmo tempo.
//
// O número médio de clientes na fila é calculado usando o método de Little. O método de Little afirma que o número médio de clientes na fila é igual à taxa de chegada vezes o tempo médio de espera.
// O tempo médio de espera é calculado usando o método de Little. O método de Little afirma que o tempo médio de espera é igual à área sob a curva de E[W] dividida pela taxa de chegada.
// O erro de Little é a diferença entre o número médio de clientes na fila calculado usando o método de Little e o número médio de clientes na fila calculado usando a fórmula tradicional.

printf("Ocupação: %lF\n", (soma_ocupacao / tempo_decorrido));
printf("Tamanho máximo da fila: %ld\n", max_fila);

double e_n_calculo = e_n.soma_areas / tempo_decorrido;
double e_w_calculo = (e_w_chegada.soma_areas - e_w_saida.soma_areas) / e_w_chegada.no_eventos;
double lambda = e_w_chegada.no_eventos / tempo_decorrido;

printf("E[N]: %lF\n", e_n_calculo);
printf("E[W]: %lF\n", e_w_calculo);
printf("Erro de Little: %.20lF\n", e_n_calculo - lambda * e_w_calculo);

    return 0;
}
