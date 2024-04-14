/* iaed-23 - ist1106326 - project2 */
/* Funções relacionadas com a gestão de memória do programa.*/

#include "dados.h"

/* Liberta toda a memória usada pelo programa até à chamada da função. */
void liberta_memoria(data* database) {
    carreira_t* carreira = database->carreiras;
    ligacao_t* ligacao = carreira->origem;
    paragem_t* paragem = database->paragens;
    carreira_que_passa_t* tmp;
    carreira_que_passa_t* carreira_que_passa = paragem->carreiras;

    while(carreira != NULL) {
        ligacao = carreira->origem;
        while(ligacao != NULL) {
            if(ligacao->seguinte != NULL) {
                ligacao = ligacao->seguinte;
                free(ligacao->anterior);
            }
            else {
                free(ligacao);
                break;
            }
        }
        if(carreira->nome != NULL) free(carreira->nome);
        if(carreira->seguinte != NULL) {
            carreira = carreira->seguinte;
            free(carreira->anterior);
        }
        else {
            free(carreira);
        break;
        }
    }

    while(paragem != NULL) {
        carreira_que_passa = paragem->carreiras;
        while (carreira_que_passa != NULL) {
           tmp = carreira_que_passa;
           carreira_que_passa = carreira_que_passa->seguinte;
           free(tmp);
        }
        if(paragem->nome != NULL) free(paragem->nome);
        if(paragem->seguinte != NULL) {
            paragem = paragem->seguinte;
            free(paragem->anterior);
        }
        else {
            free(paragem);
        break;
        }
    }

    /* Aqui não são inicializadas as listas porque a função
    pode ser chamada no fim do programa, ocasião em que voltar
    a inicializá-las só para as libertar depois é completamente 
    redundante.*/
    database->carreiras = NULL;
    database->paragens = NULL;
}

/* Quando a memória é excedida, esta função apresenta uma mensagem de erro,
apaga toda a memória usada até à chamada da função, e termina o programa. */
void saida_controlada(data* database) {
    printf("No memory.");
    liberta_memoria(database);
    free(database);
    exit(EXIT_FAILURE);
}