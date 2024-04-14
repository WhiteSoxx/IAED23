/* iaed-23 - ist1106326 - project2 */

#include "dados.h"
#include "paragens.h"
#include "carreiras.h"
#include "ligacoes.h"
#include "remocoes.h"
#include "memoria.h"

int main()
{
    char c;
    data* database = malloc(sizeof(data));

    database->carreiras = inicializa_carreiras();
    database->paragens = inicializa_paragens();

    while((c = getchar()) != EOF) {
        if (c == 'q') {
            liberta_memoria(database);
            free(database);
            return 0;
        }
        if (c == 'c')
            gestao_carreiras(database);
        if (c == 'p')
            gestao_paragens(database);
        if (c == 'l')
            gestao_ligacoes(database);
        if (c == 'i')
            print_intersecoes(database->paragens);
        if (c == 'r')
            database->carreiras = remove_carreira(database->carreiras);
        if (c == 'e')
            database->paragens = remove_paragem(database->paragens);
        if (c == 'a') {
            liberta_memoria(database);
            database->carreiras = inicializa_carreiras();
            database->paragens = inicializa_paragens();
        }
    }
    return -1;
}