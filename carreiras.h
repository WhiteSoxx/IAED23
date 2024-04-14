/* iaed-23 - ist1106326 - project2 */
/* Declaração de funções relacionadas com o comando c.*/

#ifndef _CARREIRAS_
#define _CARREIRAS_

#include "dados.h"

carreira_t* inicializa_carreiras(void);

int existe_carreira(char nome_carreira[], carreira_t* carreiras);

void print_paragens_carreira(char nome_carreira[], carreira_t* carreiras, char ordem[]);

void print_carreiras(carreira_t* carreiras);

void cria_carreira(char nome_carreira[], carreira_t* carreiras);

void gestao_carreiras(data* database);



#endif