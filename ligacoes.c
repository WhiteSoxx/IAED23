/* iaed-23 - ist1106326 - project2 */
/* Funções relacionadas com o comando l.*/

#include "dados.h"
#include "carreiras.h"
#include "paragens.h"
#include "memoria.h"

/* Função adiciona à lista de carreiras que passam na paragem fornecida
a carreira também fornecida.*/
int adiciona_carreira_passante(paragem_t* paragem, carreira_t* carreira) {
    carreira_que_passa_t* nova_carreira_que_passa = malloc(sizeof(carreira_que_passa_t));
    carreira_que_passa_t* carreira_que_passa = paragem->carreiras;

    if(nova_carreira_que_passa == NULL) return 0; 
    /* A memória foi excedida e o programa deve terminar.
    Ver chamada da função. */

    nova_carreira_que_passa->carreira_passante = carreira;

    if(paragem->num_carreiras == 0) { 
        /* Primeira carreira a ser adicionada */
        paragem->carreiras = nova_carreira_que_passa;
        paragem->carreiras->seguinte = NULL;
    }
    else if(!strcmp(carreira->nome, carreira_que_passa->carreira_passante->nome)) {
        /* A carreira já está na lista */
        carreira->numero_paragens += 1;
        free(nova_carreira_que_passa);
        return 1;
    }
    else if(strcmp(carreira->nome, carreira_que_passa->carreira_passante->nome) < 0) { 
        /* Adicionar ao início*/
        paragem->carreiras = nova_carreira_que_passa;
        paragem->carreiras->seguinte = carreira_que_passa;

    }
    else {
        /* É procurada uma posição para a carreira, mantendo a ordem alfabética. */
        while(carreira_que_passa->seguinte != NULL)
            if(strcmp(carreira->nome, carreira_que_passa->seguinte->carreira_passante->nome) > 0)
                carreira_que_passa = carreira_que_passa->seguinte;
            else if (!strcmp(carreira->nome, carreira_que_passa->seguinte->carreira_passante->nome)) {
                /* Posição encontrada. */
                carreira->numero_paragens += 1;
                free(nova_carreira_que_passa);
                return 1;
            }
            else
                break;
        nova_carreira_que_passa->seguinte = carreira_que_passa->seguinte;
        carreira_que_passa->seguinte = nova_carreira_que_passa;
    }
    
    paragem->num_carreiras += 1;
    carreira->numero_paragens += 1;
    return 1;
}

/* Cria uma ligação na carreira fornecida. Como esta função é algo 
complexa, estão divididas certas tarefas para melhorar a legibilidade.*/
int cria_ligacao(carreira_t* carreira, paragem_t* origem, paragem_t* destino, double custo, double duracao) {
    ligacao_t* ligacao_nova = malloc(sizeof(ligacao_t));
    ligacao_t* ligacao_nova2 = malloc(sizeof(ligacao_t));

    if(ligacao_nova == NULL || ligacao_nova2 == NULL) return 0; 
    /* A memória foi excedida e o programa deve terminar.
    Ver chamada da função. */
        
    ligacao_nova->custo = 0;
    ligacao_nova->duracao = 0;
    ligacao_nova2->custo = 0;
    ligacao_nova2->duracao = 0;

    if(carreira->numero_paragens == 0) {
        /* Caso em que a carreira ainda não tinha uma única ligação. */
        ligacao_nova->anterior = NULL;
        ligacao_nova2->seguinte = NULL;

        ligacao_nova->paragem = origem;
        ligacao_nova->seguinte = ligacao_nova2;
        carreira->origem = ligacao_nova;
        
        ligacao_nova2->paragem = destino;
        ligacao_nova2->anterior = ligacao_nova;
        carreira->destino = ligacao_nova2;

        ligacao_nova->custo = custo;
        ligacao_nova->duracao = duracao;
        
        if(!adiciona_carreira_passante(origem, carreira))
            return 0;
        
        if(!adiciona_carreira_passante(destino, carreira))
            return 0;
    }

    else if (carreira->destino->paragem == origem 
    && carreira->origem->paragem == destino) {
        /* Caso em que a carreira se torna num círculo. */
        free(ligacao_nova); /* Só vamos precisar de um node novo, este pode ser libertado. */
        ligacao_nova = carreira->destino;
        ligacao_nova2->paragem = destino;

        ligacao_nova2->anterior = ligacao_nova;
        ligacao_nova2->seguinte = NULL;

        ligacao_nova->seguinte = ligacao_nova2;

        carreira->destino = ligacao_nova2;

        ligacao_nova->custo = custo;
        ligacao_nova->duracao = duracao;

        carreira->numero_paragens += 1; /* Exceção, já que
        a carreira já está na lista e adiciona_carreira_passante
        não é invocada. */
    }

    else if (carreira->destino->paragem == origem) {
        /* Caso em que se adiciona a nova paragem no final da carreira. */
        free(ligacao_nova); /* Só vamos precisar de um node novo, este pode ser libertado. */
        
        ligacao_nova = carreira->destino;
        ligacao_nova2->paragem = destino;

        ligacao_nova2->anterior = ligacao_nova;
        ligacao_nova2->seguinte = NULL;

        ligacao_nova->seguinte = ligacao_nova2;

        carreira->destino = ligacao_nova2;
        
        ligacao_nova->custo = custo;
        ligacao_nova->duracao = duracao;

        if(!adiciona_carreira_passante(destino, carreira))
            return 0;
    }

    else if (carreira->origem->paragem == destino) {
        /* Caso em que se adiciona a nova paragem no início da carreira. */
        free(ligacao_nova2); /* Só vamos precisar de um node novo, este pode ser libertado. */
        ligacao_nova2 = carreira->origem;
        ligacao_nova->paragem = origem;

        ligacao_nova2->anterior = ligacao_nova;

        ligacao_nova->anterior = NULL;
        ligacao_nova->seguinte = ligacao_nova2;

        carreira->origem = ligacao_nova;

        ligacao_nova->custo = custo;
        ligacao_nova->duracao = duracao;
        
        if(!adiciona_carreira_passante(origem, carreira))
            return 0;
    }

    carreira->custo_total += custo;
    carreira->duracao_total += duracao;

    return 1;
}

/* Analisa comandos relacionados com ligações. "saida_controlada" é invocada 
quando a memória for excedida, em qualquer ponto de qualquer processo. */
void gestao_ligacoes(data* database) {
    char c;
    char* nome_carreira = malloc(sizeof(char));
    char* nome_origem = malloc(sizeof(char));
    char* nome_destino = malloc(sizeof(char));
    double custo, duracao;
    int num_argumentos = 0, encontrou_aspas = 0, nao_acabado = 1, valido = 0;
    int tamanho = 0;

    carreira_t* carreira;
    paragem_t* origem;
    paragem_t* destino;
    
    while(nao_acabado) {
        switch(c = getchar()) {
            case ' ':
                if(!encontrou_aspas) {
                    if(num_argumentos == 1) 
                        nome_carreira[tamanho++] = '\0';
                    if(num_argumentos == 2) 
                        nome_origem[tamanho++] = '\0';
                    if(num_argumentos == 3) {
                        nome_destino[tamanho++] = '\0';
                        nao_acabado = 0;
                    }
                    num_argumentos += 1;
                    tamanho = 0; 
                }
                else {
                    if(num_argumentos == 1) {
                        nome_carreira[tamanho++] = c; 
                        nome_carreira = realloc(nome_carreira, tamanho+1);
                        if(nome_carreira == NULL) saida_controlada(database);
                    }
                    if(num_argumentos == 2) {
                        nome_origem[tamanho++] = c; 
                        nome_origem = realloc(nome_origem, tamanho+1);
                        if(nome_origem == NULL) saida_controlada(database);
                    }
                    if(num_argumentos == 3) {
                        nome_destino[tamanho++] = c; 
                        nome_destino = realloc(nome_destino, tamanho+1);
                        if(nome_destino == NULL) saida_controlada(database);
                    }
                }
                break;
            case '\"':
                if(encontrou_aspas)
                    encontrou_aspas = 0;
                else
                    encontrou_aspas = 1;
                break;
            default:
                if(num_argumentos == 1) {
                    nome_carreira[tamanho++] = c; 
                    nome_carreira = realloc(nome_carreira, tamanho+1);
                    if(nome_carreira == NULL) saida_controlada(database);
                }
                if(num_argumentos == 2) {
                    nome_origem[tamanho++] = c; 
                    nome_origem = realloc(nome_origem, tamanho+1);
                    if(nome_origem == NULL) saida_controlada(database);
                }
                if(num_argumentos == 3) {
                    nome_destino[tamanho++] = c; 
                    nome_destino = realloc(nome_destino, tamanho+1);
                    if(nome_destino == NULL) saida_controlada(database);
                }
                break;
        }
    }

    if (existe_carreira(nome_carreira, database->carreiras)) {
        if (existe_paragem(nome_origem, database->paragens)) {
            if (existe_paragem(nome_destino, database->paragens))
                valido = 1;
            else
            printf("%s: no such stop.\n", nome_destino);
        }
        else
            printf("%s: no such stop.\n", nome_origem);
    }
    else
        printf("%s: no such line.\n", nome_carreira);

    if(scanf("%lf %lf", &custo, &duracao) && (custo < 0 || duracao < 0)) {
        printf("negative cost or duration.\n");
        valido = 0;
    }

    if(valido) { /* Começa-se por extrair as carreiras e paragens em questão.*/
        carreira = database->carreiras;
        while(carreira != NULL) {
            if (!strcmp(carreira->nome, nome_carreira))
                break;
            carreira = carreira->seguinte;
        }
        free(nome_carreira);

        origem = database->paragens;
        while(origem != NULL) {
            if (!strcmp(origem->nome, nome_origem))
                break;
            origem = origem->seguinte;
        }
        free(nome_origem);

        destino = database->paragens;
        while(destino != NULL) {
            if (!strcmp(destino->nome, nome_destino))
                break;
            destino = destino->seguinte;
        }
        free(nome_destino);

        /* Testa-se, por esta ordem: Se um dos extremos da ligação corresponde a um dos extremos da carreira,se o 
        destino da carreira é o destino da ligação, e se a origem da carreira é a origem da ligação. Como, no caso,de não 
        ter ainda paragens, estas duas últimas condições serem verdade, avalia-se também o número de paragens da carreira.*/
        if(carreira->numero_paragens > 0) /* refazer com comparação de pointers */
            if ((strcmp(carreira->origem->paragem->nome, destino->nome)
            && (strcmp(carreira->destino->paragem->nome, origem->nome)))
            || (((!strcmp(carreira->origem->paragem->nome, origem->nome) 
            && strcmp(carreira->origem->paragem->nome, carreira->destino->paragem->nome))
            || (!strcmp(carreira->destino->paragem->nome, destino->nome) 
            && strcmp(carreira->origem->paragem->nome, carreira->destino->paragem->nome)))
            && (strcmp(origem->nome, destino->nome)))) {
                printf("link cannot be associated with bus line.\n");
                valido = 0;
            }
        
        if(valido) {
            if(!cria_ligacao(carreira, origem, destino, custo, duracao))
                saida_controlada(database);
        }
    }
}