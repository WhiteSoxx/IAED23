/* iaed-23 - ist1106326 - project2 */
/* Declaração de funções relacionadas com os comandos p (e i).*/

#ifndef _PARAGENS_
#define _PARAGENS_

#include "dados.h"

paragem_t* inicializa_paragens(void);

void print_intersecoes(paragem_t* paragens);

void print_paragens(paragem_t* paragens);

int existe_paragem(char nome_paragem[], paragem_t* paragens);

void print_coordenadas(char nome_paragem[], paragem_t* paragens);

int cria_paragem(char* nome_paragem, double latitude, double longitude, paragem_t* paragens);

void gestao_paragens(data* database);

#endif

