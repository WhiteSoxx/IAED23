/* iaed-23 - ist1106326 - project2 */
/* Funções relacionadas com os comandos e & r.*/

#include "dados.h"
#include "paragens.h"
#include "carreiras.h"
#include "ligacoes.h"

/* Função remove da lista de carreiras que passam na paragem fornecida
a carreira também fornecida.*/
void remove_carreira_passante(carreira_t* carreira, paragem_t* paragem) {
    carreira_que_passa_t* carreira_anterior = NULL;
    carreira_que_passa_t* carreira_que_passa = paragem->carreiras;

    while(carreira_que_passa != NULL) {
        if(!strcmp(carreira->nome, paragem->carreiras->carreira_passante->nome)) {
            /* Quando a carreira é a primeira da lista */
            paragem->carreiras = carreira_que_passa->seguinte;
            free(carreira_que_passa);
            paragem->num_carreiras -= 1;
            break;
        }
        
        else if(!strcmp(carreira->nome, carreira_que_passa->carreira_passante->nome) &&
        carreira_que_passa->seguinte == NULL) {
            /* Quando a carreira é a última da lista */
            carreira_anterior->seguinte = NULL;
            free(carreira_que_passa);
            paragem->num_carreiras -= 1;
            break;
        }
        
        else if(!strcmp(carreira->nome, carreira_que_passa->carreira_passante->nome) &&
        carreira_que_passa != paragem->carreiras && carreira_que_passa->seguinte != NULL) {
            /* Quando a carreira está no meio da lista */
            carreira_anterior->seguinte = carreira_que_passa->seguinte;
            free(carreira_que_passa);
            paragem->num_carreiras -= 1;
            break;
        }

        carreira_anterior = carreira_que_passa;
        carreira_que_passa = carreira_que_passa->seguinte;
    }
}

/* Função remove todas as ligações associadas à carreira, para que
ela possa ser apagada.*/
void remove_ligacoes_carreira(carreira_t* carreira) {
    ligacao_t* ligacao = carreira->origem;

    while(ligacao != NULL) {
        remove_carreira_passante(carreira, ligacao->paragem);
        if (ligacao->anterior != NULL) 
            free(ligacao->anterior);
        if (ligacao->seguinte == NULL) {
            free(ligacao);
            break;
        }
        ligacao = ligacao->seguinte;
    }
}

/* Função remove a paragem fornecida do percurso da carreira também fornecida.
Condisera os valores custo/duração nesta remoção. */
void remove_paragem_percurso(carreira_t* carreira, paragem_t* paragem) {
    ligacao_t* ligacao_anterior = NULL;
    ligacao_t* ligacao = carreira->origem;
    int num_ocorrencias = 0;

    while(ligacao != NULL) {
        if(ligacao->paragem == paragem)
            num_ocorrencias += 1;
        ligacao = ligacao->seguinte;
    }

    ligacao = carreira->origem;

    if(carreira->numero_paragens - num_ocorrencias < 2) {
        /* Apagar tudo */
        while(ligacao != NULL) {
            if(ligacao->paragem != paragem)
                remove_carreira_passante(carreira, ligacao->paragem);
            
            if(ligacao->seguinte == NULL) {
                free(ligacao);
                break;
            }
            else {
                ligacao = ligacao->seguinte;
                free(ligacao->anterior);
            }
        }
        carreira->origem = NULL;
        carreira->destino = NULL;
        carreira->numero_paragens = 0;
        carreira->custo_total = 0;
        carreira->duracao_total = 0;
        return;
    }
    else {
        carreira->numero_paragens -= num_ocorrencias;
        while(num_ocorrencias != 0) { /* ver espaços nos for, if e whiles */
            if(ligacao->paragem == paragem && ligacao->anterior == NULL) {
                /* Primeira da lista */
                carreira->origem = ligacao->seguinte;
                carreira->custo_total -= ligacao->custo;
                carreira->duracao_total -= ligacao->duracao;
                num_ocorrencias -= 1;
                ligacao = ligacao->seguinte;
                free(ligacao->anterior);
                ligacao->anterior = NULL;
            }

            else if(ligacao->paragem == paragem && ligacao->anterior != NULL &&
            ligacao->seguinte != NULL) {
                /* No meio da lista */
                ligacao->anterior->seguinte = ligacao->seguinte;
                ligacao->seguinte->anterior = ligacao->anterior;
                ligacao->anterior->custo += ligacao->custo;
                ligacao->anterior->duracao += ligacao->duracao;
                ligacao_anterior = ligacao;
                ligacao = ligacao->seguinte;
                num_ocorrencias -= 1;
                free(ligacao_anterior);
            }

            else if(ligacao->paragem == paragem && ligacao->anterior != NULL &&
            ligacao->seguinte == NULL) {
                /* No fim da lista */
                carreira->destino = ligacao->anterior;
                ligacao->anterior->seguinte = NULL;
                carreira->custo_total -= ligacao->anterior->custo;
                carreira->duracao_total -= ligacao->anterior->duracao;
                ligacao->anterior->custo = 0;
                ligacao->anterior->duracao = 0;
                num_ocorrencias -= 1;
                free(ligacao);
                break;
            }

            else ligacao = ligacao->seguinte;
        }
    }
}

/* Função remove todas as carreiras associadas à paragem, para que
ela possa ser apagada.*/
void remove_carreiras_da_paragem(paragem_t* paragem) {
    carreira_que_passa_t* carreira_anterior = NULL;
    carreira_que_passa_t* carreira_que_passa = paragem->carreiras;

    while(carreira_que_passa != NULL) {
        if(carreira_que_passa->seguinte == NULL) {
            /* Se for a última. */
            remove_paragem_percurso(carreira_que_passa->carreira_passante, paragem);
            free(carreira_que_passa);
            break;
        }
        else {
            remove_paragem_percurso(carreira_que_passa->carreira_passante, paragem);
            carreira_anterior = carreira_que_passa;
            carreira_que_passa = carreira_que_passa->seguinte;
            free(carreira_anterior);
        }
    }

    return;
}

/* Função apaga da lista de carreiras a carreira fornecida.*/
carreira_t* remove_carreira(carreira_t* carreiras) {
    char c;
    char *nome_carreira = '\0';
    int encontrou_aspas = 0, nao_acabado = 1;
    int tamanho = 0;

    carreira_t* carreira = carreiras;

    while(nao_acabado) {
        switch(c = getchar()) {
            case '\n':
                nao_acabado = 0;
                nome_carreira[tamanho++] = '\0';
                break;
            case ' ':
                nome_carreira = malloc(sizeof(char));
                break;
            case '\"':
                if(!encontrou_aspas) 
                    encontrou_aspas = 1;
                else 
                    encontrou_aspas = 0;
                break;
            default:
                nome_carreira[tamanho++] = c; 
                nome_carreira = realloc(nome_carreira, tamanho+1);
                break;
        }
    }

    if(!existe_carreira(nome_carreira, carreiras)) {
        printf("%s: no such line.\n", nome_carreira);
        free(nome_carreira);
        return carreiras;
    }

    if(!strcmp(nome_carreira, carreiras->nome) &&
    carreiras->anterior == NULL && carreiras->seguinte == NULL) { 
        /* A carreira a eliminar é a única que existe. */
        if(carreiras->origem != NULL) 
            remove_ligacoes_carreira(carreiras);
        free(carreiras->nome);
        free(carreiras);
        free(nome_carreira);
        carreiras = inicializa_carreiras();
        return carreiras;
    }

    while(carreira != NULL) {
        if(strcmp(nome_carreira, carreira->nome))
            carreira = carreira->seguinte;
        else {
            if(carreira->anterior == NULL) {
                /* A carreira é a primeira da lista. */
                carreira->seguinte->anterior = NULL;
                carreiras = carreira->seguinte;
            }
            else if (carreira->seguinte == NULL) {
                /* A carreira é a última da lista. */
                carreira->anterior->seguinte = NULL;
            }
            else {
                /* A carreira está no meio da lista. */
                carreira->anterior->seguinte = carreira->seguinte;
                carreira->seguinte->anterior = carreira->anterior;
            }

            if(carreira->origem != NULL) 
                remove_ligacoes_carreira(carreira);

            free(carreira->nome);
            free(carreira);
            break;
        }
    }
    free(nome_carreira);
    return carreiras;
}

/* Função apaga da lista de paragens a paragem fornecida.*/
paragem_t* remove_paragem(paragem_t* paragens) {
    char c;
    char *nome_paragem = '\0';
    int encontrou_aspas = 0, nao_acabado = 1;
    int tamanho = 0;

    paragem_t* paragem = paragens;

    while(nao_acabado) {
        switch(c = getchar()) {
            case '\n':
                nao_acabado = 0;
                nome_paragem[tamanho++] = '\0';
                break;
            case ' ':
                nome_paragem = malloc(sizeof(char));
                break;
            case '\"':
                if(!encontrou_aspas) 
                    encontrou_aspas = 1;
                else 
                    encontrou_aspas = 0;
                break;
            default:
                nome_paragem[tamanho++] = c; 
                nome_paragem = realloc(nome_paragem, tamanho+1);
                break;
        }
    }

    if(!existe_paragem(nome_paragem, paragens)) {
        printf("%s: no such stop.\n", nome_paragem);
        free(nome_paragem);
        return paragens;
    }

    else if(!strcmp(nome_paragem, paragens->nome) &&
    paragens->anterior == NULL && paragens->seguinte == NULL) { 
        /* A paragem a eliminar é a única que existe. */
        if(paragens->carreiras != NULL) 
            remove_carreiras_da_paragem(paragens);
        free(paragens->nome);
        free(paragens);
        free(nome_paragem);
        paragens = inicializa_paragens();
        return paragens;
    }

    while(paragem != NULL) {
        if(strcmp(nome_paragem, paragem->nome))
            paragem = paragem->seguinte;
        else {
            if(paragem->anterior == NULL) {
                /* A paragem é a primeira da lista. */
                paragem->seguinte->anterior = NULL;
                paragens = paragem->seguinte;
            }
            else if (paragem->seguinte == NULL) {
                /* A paragem é a última da lista. */
                paragem->anterior->seguinte = NULL;
            }
            else {
                /* A paragem está no meio da lista.*/
                paragem->anterior->seguinte = paragem->seguinte;
                paragem->seguinte->anterior = paragem->anterior;
            }
            if(paragem->carreiras != NULL) 
                remove_carreiras_da_paragem(paragem);
            free(paragem->nome);
            free(paragem);
            break;
        }
    }

    free(nome_paragem);
    return paragens;
}