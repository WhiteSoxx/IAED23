/* iaed-23 - ist1106326 - project2 */
/* Declaração de funções relacionadas com os comandos e & r.*/

#ifndef _REMOCOES_
#define _REMOCOES_

#include "dados.h"
#include "paragens.h"
#include "carreiras.h"
#include "ligacoes.h"

void remove_carreira_passante(carreira_t* carreira, paragem_t* paragem);

void remove_ligacoes_carreira(carreira_t* carreira);

void remove_paragem_percurso(carreira_t* carreira, paragem_t* paragem);

void remove_carreiras_da_paragem(paragem_t* paragem);

carreira_t* remove_carreira(carreira_t* carreiras);

paragem_t* remove_paragem(paragem_t* paragem);

#endif