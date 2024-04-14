/* iaed-23 - ist1106326 - project2 */
/* Funções relacionadas com os comandos p (e i).*/

#include "dados.h"
#include "memoria.h"

/* Cria uma head para a lista de carreiras. Usada no início do programa
ou quando todas as carreiras são apagadas. */
paragem_t* inicializa_paragens(void) {
    paragem_t* paragens = malloc(sizeof(paragem_t));

    paragens->nome = NULL;
    paragens->num_carreiras = 0;
    paragens->latitude = 0;
    paragens->longitude = 0;
    paragens->anterior = NULL;
    paragens->seguinte = NULL;
    paragens->carreiras = NULL;

    return paragens;
}

/* Apresenta no stdout o nome das paragens onde passam mais do que uma
carreira, e apresenta o nome dessas carreiras por ordem alfabética. */
void print_intersecoes(paragem_t* paragens) {
    paragem_t* paragem = paragens;
    carreira_que_passa_t* carreira_que_passa;

    while(paragem != NULL) {
        if(paragem->num_carreiras > 1) {
            printf("%s %d:", paragem->nome, paragem->num_carreiras);
            carreira_que_passa = paragem->carreiras;
            while(carreira_que_passa != NULL) {
                printf(" %s",carreira_que_passa->carreira_passante->nome);
                carreira_que_passa = carreira_que_passa->seguinte;
            }
            printf("\n");
        }
        paragem = paragem->seguinte;
    }
}

/* Devolve no stdout todas as paragens existentes, por ordem de criação.*/
void print_paragens(paragem_t* paragens) {
    paragem_t* paragem = paragens;

    while(paragem != NULL) {
        if (paragem->nome != NULL) { /* No caso de não existirem paragens, isto verifica-se.  */
            printf("%s: ", paragem->nome);
            printf("%16.12f %16.12f %d\n", paragem->latitude, paragem->longitude, paragem->num_carreiras);
        }
        paragem = paragem->seguinte;
    }
}

/* Verifica a existência de uma paragem com o nome indicado.*/
int existe_paragem(char nome_paragem[], paragem_t* paragens) {
    paragem_t* paragem = paragens;
    if (paragem->nome != NULL)
        while(paragem != NULL) {
            if (!strcmp(paragem->nome, nome_paragem))
                return 1;
            paragem = paragem->seguinte;
        }
    return 0;
}

/* Devolve no stdout as coordenadas da paragem indicada. */
void print_coordenadas(char nome_paragem[], paragem_t* paragens) {
    paragem_t* paragem = paragens;

    while(paragem != NULL) {
        if (!strcmp(paragem->nome, nome_paragem)) {
            printf("%16.12f %16.12f\n", paragem->latitude, paragem->longitude);
            break;
        }
        paragem = paragem->seguinte;
    }
}

/* Adiciona uma paragem à lista paragens. */
int cria_paragem(char* nome_paragem, double latitude, double longitude, paragem_t* paragens) {
    paragem_t* nova_paragem = malloc(sizeof(paragem_t));
    paragem_t* paragem = paragens;
    
    if(nova_paragem == NULL) return 0;
    /* A memória foi excedida e o programa deve terminar.
    Ver chamada da função. */

    while(paragem->seguinte != NULL)
        paragem = paragem->seguinte;

    nova_paragem->nome = nome_paragem;
    nova_paragem->num_carreiras = 0;
    nova_paragem->latitude = latitude;
    nova_paragem->longitude = longitude;

    paragem->seguinte = nova_paragem;
    nova_paragem->anterior = paragem;
    nova_paragem->seguinte = NULL;

    nova_paragem->carreiras = NULL;
    return 1;
}

/* Analisa comandos relacionados com paragens. "saida_controlada" é invocada 
quando a memória for excedida, em qualquer ponto de qualquer processo. */
void gestao_paragens(data* database) {
    paragem_t* paragens = database->paragens;
    char c;
    char *nome_paragem = '\0';
    double latitude, longitude;
    int num_argumentos = 0, encontrou_aspas = 0, nao_acabado = 1;
    int tamanho = 0;

    while(nao_acabado) {
        switch(c = getchar()) {
            case '\n':
                nao_acabado = 0;
                if(num_argumentos == 1) 
                    nome_paragem[tamanho++] = '\0';
                break;
            case ' ':
                if(!encontrou_aspas) {
                    if(num_argumentos == 1) {
                        nome_paragem[tamanho++] = '\0';
                        nao_acabado = 0;
                        num_argumentos += 2;
                    }
                    else if(num_argumentos == 0) {
                        nome_paragem = malloc(sizeof(char));
                        if(nome_paragem == NULL) saida_controlada(database);
                        num_argumentos += 1;
                    }
                }
                else {
                    nome_paragem[tamanho++] = c; 
                    nome_paragem = realloc(nome_paragem, tamanho+1);
                    if(nome_paragem == NULL) saida_controlada(database);
                }
                break;
            case '\"':
                if(encontrou_aspas)
                    encontrou_aspas = 0;
                else
                    encontrou_aspas = 1;
                break;
            default:
                if (num_argumentos == 1) {
                    nome_paragem[tamanho++] = c;
                    nome_paragem = realloc(nome_paragem, tamanho+1);
                    if(nome_paragem == NULL) saida_controlada(database);
                }
                break;
        }
    }

    if (num_argumentos == 0) {
        print_paragens(paragens);
        free(nome_paragem);
    }
    else if (num_argumentos == 1) {
        if (existe_paragem(nome_paragem, paragens))
            print_coordenadas(nome_paragem, paragens);
        else
            printf("%s: no such stop.\n", nome_paragem);
        free(nome_paragem);
    }
    else if (num_argumentos == 3) { 
        if (scanf("%lf %lf", &latitude, &longitude) && !existe_paragem(nome_paragem, paragens))
            if (paragens->nome != NULL) /* já não é a primeira paragem*/
                if(cria_paragem(nome_paragem, latitude, longitude, paragens))
                    return;
                else {
                    free(nome_paragem);
                    saida_controlada(database);
                }
            else {
                paragens->nome = nome_paragem;
                paragens->latitude = latitude;
                paragens->longitude = longitude;
            }
        
        else {
            printf("%s: stop already exists.\n", nome_paragem);
            free(nome_paragem);
        }
    }
}