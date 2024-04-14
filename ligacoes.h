/* iaed-23 - ist1106326 - project2 */
/* Declaração de funções relacionadas com o comando l.*/

#ifndef _LIGACOES_
#define _LIGACOES_

#include "dados.h"
#include "paragens.h"
#include "carreiras.h"

int cria_ligacao(carreira_t* carreira, paragem_t* origem, paragem_t* destino, double custo, double duracao);

int adiciona_carreira_passante(paragem_t* paragem, carreira_t* carreira);

void gestao_ligacoes(data* database);


#endif