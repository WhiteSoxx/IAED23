/* iaed-23 - ist1106326 - project2 */
/* Funções relacionadas com o comando c.*/

#include "dados.h"
#include "memoria.h"

/* Cria uma head para a lista de carreiras. Usada no início do programa
ou quando todas as carreiras são apagadas. */
carreira_t* inicializa_carreiras(void) {
    carreira_t* carreiras = malloc(sizeof(carreira_t));

    carreiras->nome = NULL;
    carreiras->anterior = NULL;
    carreiras->seguinte = NULL;
    carreiras->origem = NULL;
    carreiras->destino = NULL;
    carreiras->custo_total = 0;
    carreiras->duracao_total = 0;
    carreiras->numero_paragens = 0;

    return carreiras;
}

/* Verifica a existência de uma carreira com o nome indicado.*/
int existe_carreira(char nome_carreira[], carreira_t* carreiras) {
    carreira_t* carreira = carreiras;
    if(carreira->nome != NULL) { /* Porque, quando não há carreiras, isto se verifica */
        while(carreira != NULL) {
            if (!strcmp(carreira->nome, nome_carreira))
                return 1;
            carreira = carreira->seguinte;
        }
    }

    return 0;
}

/* Devolve no stdout os nomes das paragens da carreira, pela ordem instruída por "ordem". */
void print_paragens_carreira(char nome_carreira[], carreira_t* carreiras, char ordem[]) { /* define de ordem?? */
    carreira_t* carreira = carreiras;
    ligacao_t node;

    while(carreira != NULL) { /* Primeiro, procuramos a carreira. */
        if (!strcmp(carreira->nome, nome_carreira))
            break;
        carreira = carreira->seguinte;
    }

    if (!strcmp(ordem, "normal") && carreira->origem != NULL) {
        node = *carreira->origem;
        printf("%s", node.paragem->nome);
        while (node.seguinte != NULL) {
                printf(", %s", node.seguinte->paragem->nome);
                node = *node.seguinte;
        }
        printf("\n");
    }
    else if(carreira->destino != NULL) { /* Caso inverso */
        node = *carreira->destino;
        printf("%s", node.paragem->nome);
        while (node.anterior != NULL) {
                printf(", %s", node.anterior->paragem->nome);
                node = *node.anterior;
        }
        printf("\n");
    } 

    free(nome_carreira); /* já não é necessário o nome recolhido
    no comando */
}

/* Devolve no stdout informações sobre cada carreira, por ordem de criação. */
void print_carreiras(carreira_t* carreiras) {
    carreira_t* carreira = carreiras;

    while(carreira != NULL) {
        if (carreira->nome != NULL) {
            printf("%s ", carreira->nome);
            if(carreira->origem != NULL)
                printf("%s %s ", carreira->origem->paragem->nome, carreira->destino->paragem->nome);
            printf("%d %.2f %.2f\n", carreira->numero_paragens, carreira->custo_total, carreira->duracao_total);
        }
        carreira = carreira->seguinte;
    }
}

/* Adiciona uma carreira no fim da lista carreiras. */
int cria_carreira(char* nome_carreira, carreira_t* carreiras) {
    carreira_t* nova_carreira = malloc(sizeof(carreira_t));
    carreira_t* carreira = carreiras;

    if(nova_carreira == NULL) return 0;
    /* A memória foi excedida e o programa deve terminar.
    Ver chamada da função. */

    while(carreira->seguinte != NULL)
        carreira = carreira->seguinte;

    nova_carreira->nome = nome_carreira;
    nova_carreira->custo_total = 0;
    nova_carreira->duracao_total = 0;
    nova_carreira->numero_paragens = 0;
    nova_carreira->origem = NULL;
    nova_carreira->destino = NULL;

    nova_carreira->anterior = carreira;
    carreira->seguinte = nova_carreira;
    nova_carreira->seguinte = NULL;
    return 1;
}

/* Analisa comandos relacionados com carreiras. "saida_controlada" é invocada 
quando a memória for excedida, em qualquer ponto de qualquer processo. */
void gestao_carreiras(data* database) {
    carreira_t* carreiras = database->carreiras;
    char c;
    char *nome_carreira = '\0';
    char ordem[11] = {'\0'};
    int i = 0, num_argumentos = 0, encontrou_aspas = 0, nao_acabado = 1;
    int tamanho = 0;
    
    while(nao_acabado) {
        switch(c = getchar()) {
            case '\n':
                nao_acabado = 0;
                if(num_argumentos == 1)
                    nome_carreira[tamanho++] = '\0';
                else if(num_argumentos == 2) {
                        ordem[i] = '\0'; 
                    }
                break;
            case ' ':
                if(!encontrou_aspas) {
                    if(num_argumentos == 1)
                        nome_carreira[tamanho++] = '\0';
                    else if(num_argumentos == 0) {
                        nome_carreira = malloc(sizeof(char));
                        if(nome_carreira == NULL) saida_controlada(database);
                    }
                    num_argumentos += 1;
                }   
                    
                else {
                    nome_carreira[tamanho++] = c; 
                    nome_carreira = realloc(nome_carreira, tamanho+1);
                    if(nome_carreira == NULL) saida_controlada(database);
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
                    nome_carreira[tamanho++] = c; 
                    nome_carreira = realloc(nome_carreira, tamanho+1);
                    if(nome_carreira == NULL) saida_controlada(database);
                }
                if (num_argumentos == 2) {
                    ordem[i] = c;
                    i += 1;
                }
                break;
        }
    }
    
    if (num_argumentos == 0) print_carreiras(carreiras);

    else if (num_argumentos == 1) {
        if (existe_carreira(nome_carreira, carreiras))
            print_paragens_carreira(nome_carreira, carreiras, "normal");
        else
            if (carreiras->nome != NULL) { /* já não é a primeira carreira */
                if(!cria_carreira(nome_carreira, carreiras)) {
                    free(nome_carreira);
                    saida_controlada(database);
                }
            }
            else
                carreiras->nome = nome_carreira;
    }
    else if (num_argumentos == 2) {
        if (!strcmp(ordem, "inv") || !strcmp(ordem, "inve") || !strcmp(ordem, "inver")
        || !strcmp(ordem, "invers") || !strcmp(ordem, "inverso"))
            print_paragens_carreira(nome_carreira, carreiras, "inverso");
        else {
            printf("incorrect sort option.\n");
            free(nome_carreira); /* não se fará nada, o nome
            recolhido é portanto inútil*/
        }
    }
}