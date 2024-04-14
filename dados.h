/* iaed-23 - ist1106326 - project2 */
/* Definição de estruturas e constantes, e include de libraries. */

#ifndef _DADOS_
#define _DADOS_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct ligacao {
    struct paragem* paragem;
    double custo;
    double duracao;
    struct ligacao *anterior;
    struct ligacao *seguinte;
} ligacao_t;

typedef struct carreira{
    char *nome;
    ligacao_t *origem;
    ligacao_t *destino;
    int numero_paragens;
    double custo_total;
    double duracao_total;

    struct carreira* anterior;
    struct carreira* seguinte;
} carreira_t;

typedef struct carreira_que_passa {
    carreira_t* carreira_passante;
    struct carreira_que_passa *seguinte;
} carreira_que_passa_t;

typedef struct paragem{
    char *nome;
    double latitude;
    double longitude;
    int num_carreiras;
    carreira_que_passa_t* carreiras;

    struct paragem* anterior;
    struct paragem* seguinte;
} paragem_t;

typedef struct data{
    carreira_t* carreiras;
    paragem_t* paragens;
} data;

#endif