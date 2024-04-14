#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Definicao Constantes */

#define VERDADE 1
#define FALSO 0
#define NAO_ENCONTRADO -1

#define ERRO_PARSING 1
#define MAX_TAMANHO_LINHA BUFSIZ

#define MAX_NOME_PARAGEM 51
#define MAX_NOME_CARREIRA 21

#define MAX_PARAGENS 10000
#define MAX_LIGACOES 30000
#define MAX_CARREIRAS 200

/* Definicao de Tipos de Dados */

typedef struct {
    char nome[MAX_NOME_PARAGEM];
    double latitude, longitude;
    int idCarreiras[MAX_CARREIRAS];
    int numCarreiras;
} Paragem;

typedef struct {
    int idCarreira;
    int idOrigem, idDestino;
    double custo, duracao;
} Ligacao;

typedef struct {
    char nome[MAX_NOME_CARREIRA];
    double custoTotal;
    double duracaoTotal;
    int idLigacoes[MAX_LIGACOES];
    int numLigacoes;
} Carreira;

/* Variaveis Globais */

char _buffer[MAX_TAMANHO_LINHA]; /* Buffer auxiliar. */

/* Variáveis para guardar paragens, ligações e carreiras. */

Paragem _paragens[MAX_PARAGENS];
Ligacao _ligacoes[MAX_LIGACOES];
Carreira _carreiras[MAX_CARREIRAS];

/* Contadores do número de paragens, ligações e carreiras. */

int _numParagens = 0;
int _numLigacoes = 0;
int _numCarreiras = 0;


/* Funções para parsing do input */

/* Le espaços. Devolve 0 se chegou ao final de linha ou 1 caso contrário. */

int leEspacos() {
    int c;
    while ((c = getchar()) == ' ' || c == '\t');
    if (c == '\n') 
        return 0;
    ungetc(c, stdin);
    return 1;
}

/* Le um nome para a string que recebe como parametro. */

void leNome(char s[]) {
    int i = 0, c;
    s[0] = getchar();
    if (s[0] != '"') {
        i = 1;
        while ((c = getchar()) != ' ' && c != '\t' && c != '\n')
            s[i++] = c;
        ungetc(c, stdin);
    }
    else {
        while((c = getchar()) != '"')
            s[i++] = c;
    }
    s[i++] = '\0';
}

/* Le todo o texto até ao final de linha. */

void leAteFinalLinha(char s[]) {
    char c;
    int i = 0;
    while ((c = getchar()) != '\n') {
        s[i++] = c;
    }
    s[i] = '\0';
}


/* Fnções para tratar comandos */

/* Funções para tratar as carreiras */

/* Mostra no output a carreira com indice i */

void mostraCarreira(int i) {
    int numLigacoes = _carreiras[i].numLigacoes;

    printf("%s ", _carreiras[i].nome);
    if (numLigacoes > 0) {
        printf("%s %s ", _paragens[_ligacoes[_carreiras[i].idLigacoes[0]].idOrigem].nome,
                         _paragens[_ligacoes[_carreiras[i].idLigacoes[numLigacoes-1]].idDestino].nome);
        printf("%d ", numLigacoes+1);
        printf("%.2f ", _carreiras[i].custoTotal);
        printf("%.2f\n", _carreiras[i].duracaoTotal);
    }
    else 
        printf("%d %.2f %.2f\n", 0, 0.0, 0.0);
}

/* Mostra as ligações da carreira com indice i. */

void mostraLigacoesCarreira(int i, int inverso) {
    int l, numLigacoes = _carreiras[i].numLigacoes;
    int idLigacao;

    if (numLigacoes == 0) 
        return;
    if (inverso == FALSO) {
        for (l = 0; l < numLigacoes; l++) {
            idLigacao = _carreiras[i].idLigacoes[l];
            printf("%s, ", _paragens[_ligacoes[idLigacao].idOrigem].nome);
        }
        idLigacao = _carreiras[i].idLigacoes[numLigacoes-1];
        printf("%s\n", _paragens[_ligacoes[idLigacao].idDestino].nome);
    }
    else {
        for (l = numLigacoes-1; l >= 0; l--) {
            idLigacao = _carreiras[i].idLigacoes[l];
            printf("%s, ", _paragens[_ligacoes[idLigacao].idDestino].nome);
        }
        idLigacao = _carreiras[i].idLigacoes[0];
        printf("%s\n", _paragens[_ligacoes[idLigacao].idOrigem].nome);
    }
}

/* Mostra todas as carreiras. */

void listaCarreiras() {
    int i;

    for (i = 0; i < _numCarreiras; i++)
        mostraCarreira(i);
}

/* Procura uma carreira por nome.
   Devolve o indice da carreira ou NAO_ENCONTRADO se não existe. */

int encontraCarreira(char nomeCarreira[]) {
    int i;

    for (i = 0; i < _numCarreiras; i++)
        if (strcmp(_carreiras[i].nome, nomeCarreira) == 0)
            return i;
    return NAO_ENCONTRADO;
}

/* Procura se uma paragem existe numa carreira.
   Se existir, devolve o indice da primeira ligação que usa
   a paragem. Caso contrário, devolve NAO_ENCONTRADO. */

int encontraParagemCarreira(int idCarreira, int idParagem) {
    int numLigacoes = _carreiras[idCarreira].numLigacoes;
    int i, idLigacao;

    for (i = 0; i < numLigacoes; i++) {
        idLigacao = _carreiras[idCarreira].idLigacoes[i];
        if (_ligacoes[idLigacao].idOrigem == idParagem)
            return i;
    }
    idLigacao = _carreiras[idCarreira].idLigacoes[numLigacoes-1];
    if (_ligacoes[idLigacao].idDestino == idParagem)
        return numLigacoes;
    return NAO_ENCONTRADO;
}

/* Cria uma carreira nova. */

void criaCarreira(char nomeCarreira[]) {
    _carreiras[_numCarreiras].custoTotal = 0.0;
    _carreiras[_numCarreiras].duracaoTotal = 0.0;
    _carreiras[_numCarreiras].numLigacoes = 0;
    strcpy(_carreiras[_numCarreiras].nome, nomeCarreira);
    _numCarreiras++;
}

/* Verifica se a string é um prefixo de tamanho pelo menos 3 da
   palavra inverso. */

int verificaInversoOk(char s[]) {
    char inv[] = "inverso";
    int size = strlen(s), i;

    if (size < 3 || size > 7)
        return FALSO;
    for (i = 0; i < size; i++)
        if (inv[i] != s[i])
            return FALSO;
    return VERDADE;
}

/* Função para tratar do comando 'c'. */

void carreiras() {
    char s[MAX_TAMANHO_LINHA];
    int index, fimLinha = leEspacos();
    
    if (!fimLinha) {
        listaCarreiras();
        return;
    }

    leNome(s);
    index = encontraCarreira(s);
    fimLinha = leEspacos();
    if (!fimLinha) {
        if (index == NAO_ENCONTRADO)
            criaCarreira(s);
        else
            mostraLigacoesCarreira(index, FALSO);
    }
    else {
        leNome(_buffer);
        if (verificaInversoOk(_buffer))
            mostraLigacoesCarreira(index, VERDADE);
        else
            printf("incorrect sort option.\n");
        leAteFinalLinha(_buffer);
    }
}

/* Funções para tratar as paragens */

/* Mostra uma paragem. */

void mostraParagem(int i) {
    printf("%s: %16.12f %16.12f %d\n", _paragens[i].nome, _paragens[i].latitude, 
            _paragens[i].longitude, _paragens[i].numCarreiras);
}

/* Mostra todas as paragens. */

void listaParagens() {
    int i;
    for (i = 0; i < _numParagens; i++)
        mostraParagem(i);
}

/* Verifica se existe uma paragem com um determinado nome.
   Se existir devolve o indice. Caso contrário, devolve NAO_ENCONTRADO. */

int encontraParagem(char nomeParagem[]) {
    int i;

    for (i = 0; i < _numParagens; i++)
        if (strcmp(nomeParagem, _paragens[i].nome) == 0)
            return i;
    return NAO_ENCONTRADO;
}

/* Cria uma nova paragem. */

void criaParagem(char nomeParagem[], double latitude, double longitude) {
    _paragens[_numParagens].latitude = latitude;
    _paragens[_numParagens].longitude = longitude;
    _paragens[_numParagens].numCarreiras = 0;
    strcpy(_paragens[_numParagens].nome, nomeParagem);
    _numParagens++;
}

/* Função para tratar o comando 'p'. */

void paragens() {
    char s[MAX_NOME_PARAGEM];
    int i, fimLinha = leEspacos();
    
    if (!fimLinha) {
        listaParagens();
        return;
    }
    leNome(s);
    fimLinha = leEspacos();
    if (!fimLinha) {
        if ((i = encontraParagem(s)) == NAO_ENCONTRADO)
            printf("%s: no such stop.\n", s);
        else   
            printf("%16.12f %16.12f\n", _paragens[i].latitude, _paragens[i].longitude);
    }
    else {
        double latitude, longitude;
        scanf("%lf%lf", &latitude, &longitude);
        if (encontraParagem(s) == NAO_ENCONTRADO)
            criaParagem(s, latitude, longitude);
        else
            printf("%s: stop already exists.\n", s);
        leAteFinalLinha(_buffer);
    }
}

/* Funções para tratar as Ligações */

/* Adiciona uma nova ligação. */

void criaLigacao(int idCarreira, int idParagemOrigem, int idParagemDestino,
                 double custo, double duracao) {
    _ligacoes[_numLigacoes].idCarreira = idCarreira;
    _ligacoes[_numLigacoes].idOrigem = idParagemOrigem;
    _ligacoes[_numLigacoes].idDestino = idParagemDestino;
    _ligacoes[_numLigacoes].custo = custo;
    _ligacoes[_numLigacoes].duracao = duracao;
    _numLigacoes++;
}

/* Acrescenta uma nova ligação no fim de uma carreira. */

void acrescentaLigacaoFim(int idCarreira, int idLigacao) {
    int numLigacoesCarreira = _carreiras[idCarreira].numLigacoes;

    _carreiras[idCarreira].idLigacoes[numLigacoesCarreira] = idLigacao;
    _carreiras[idCarreira].custoTotal += _ligacoes[idLigacao].custo;
    _carreiras[idCarreira].duracaoTotal += _ligacoes[idLigacao].duracao;
    _carreiras[idCarreira].numLigacoes++;
}

/* Acrescenta uma nova ligação no início de uma carreira. */

void acrescentaLigacaoInicio(int idCarreira, int idLigacao) {
    int numLigacoesCarreira = _carreiras[idCarreira].numLigacoes;
    int i;

    for (i = numLigacoesCarreira-1; i >= 0; i--)
        _carreiras[idCarreira].idLigacoes[i+1] = _carreiras[idCarreira].idLigacoes[i];

    _carreiras[idCarreira].idLigacoes[0] = idLigacao;
    _carreiras[idCarreira].custoTotal += _ligacoes[idLigacao].custo;
    _carreiras[idCarreira].duracaoTotal += _ligacoes[idLigacao].duracao;
    _carreiras[idCarreira].numLigacoes++;
}

/* Adiciona que existe uma nova carreira associada a uma paragem. */

void adicionaCarreiraParagem(int idParagem, int idCarreira) {
    _paragens[idParagem].idCarreiras[_paragens[idParagem].numCarreiras] = idCarreira;
    _paragens[idParagem].numCarreiras++;
}


/* Adiciona primeira ligacao da carreira. */

void adicionaPrimeiraLigacao(int idCarreira, int idParagemOrigem, int idParagemDestino,
                     double custo, double duracao) {
    adicionaCarreiraParagem(idParagemOrigem, idCarreira);
    if (idParagemOrigem != idParagemDestino)
        adicionaCarreiraParagem(idParagemDestino, idCarreira);
    criaLigacao(idCarreira, idParagemOrigem, idParagemDestino, custo, duracao);
    acrescentaLigacaoFim(idCarreira, _numLigacoes-1);  
}


/* Adiciona uma nova ligação a uma carreira. */

void adicionaLigacao(int idCarreira, int idParagemOrigem, int idParagemDestino,
                     double custo, double duracao) {
    int numLigacoesCarreira = _carreiras[idCarreira].numLigacoes;
    
    if (numLigacoesCarreira == 0) 
        adicionaPrimeiraLigacao(idCarreira, idParagemOrigem, idParagemDestino,
				custo, duracao);
    else {
        int idPrimeiraLigacao = _carreiras[idCarreira].idLigacoes[0];
        int idUltimaLigacao = _carreiras[idCarreira].idLigacoes[numLigacoesCarreira-1];
        if (idParagemOrigem == _ligacoes[idUltimaLigacao].idDestino) {
            if (encontraParagemCarreira(idCarreira, idParagemDestino) == NAO_ENCONTRADO)
                adicionaCarreiraParagem(idParagemDestino, idCarreira);
            criaLigacao(idCarreira, idParagemOrigem, idParagemDestino, custo, duracao);
            acrescentaLigacaoFim(idCarreira, _numLigacoes-1);
        }
        else if (idParagemDestino == _ligacoes[idPrimeiraLigacao].idOrigem) {
            if (encontraParagemCarreira(idCarreira, idParagemOrigem) == NAO_ENCONTRADO)
                adicionaCarreiraParagem(idParagemOrigem, idCarreira);
            criaLigacao(idCarreira, idParagemOrigem, idParagemDestino, custo, duracao);
            acrescentaLigacaoInicio(idCarreira, _numLigacoes-1);
        }
        else 
            printf("link cannot be associated with bus line.\n");
    }
}

/* Le nome de carreira e paragens de uma ligacao. */

void leNomesComando(char nomeCarreira[], char nomeOrigem[], char nomeDestino[]) {
    leEspacos();
    leNome(nomeCarreira);
    leEspacos();
    leNome(nomeOrigem);
    leEspacos();
    leNome(nomeDestino);
}

/* Função para tratar o comando 'l'. */

void ligacoes() {
    char nomeCarreira[MAX_NOME_CARREIRA], nomeOrigem[MAX_NOME_PARAGEM], nomeDestino[MAX_NOME_PARAGEM];
    double custo, duracao;
    int indexCarreira, indexOrigem, indexDestino;

    leNomesComando(nomeCarreira, nomeOrigem, nomeDestino);
    scanf("%lf%lf", &custo, &duracao);
    leAteFinalLinha(_buffer);

    indexCarreira = encontraCarreira(nomeCarreira);
    if (indexCarreira == NAO_ENCONTRADO)
        printf("%s: no such line.\n", nomeCarreira);
    else {
        indexOrigem = encontraParagem(nomeOrigem);
        if (indexOrigem == NAO_ENCONTRADO) 
	    printf("%s: no such stop.\n", nomeOrigem);
	else {
	    indexDestino = encontraParagem(nomeDestino);
	    if (indexDestino == NAO_ENCONTRADO)
	        printf("%s: no such stop.\n", nomeDestino);
	    else if (custo < 0.0 || duracao < 0.0)
	            printf("negative cost or duration.\n");
		else 
		    adicionaLigacao(indexCarreira, indexOrigem, indexDestino, custo, duracao);
	}
    }
}

/* Funções para tratar as interseções */

/* Aplica Bubble Sort a um vector de identificadores de carreiras. */

void ordenaCarreiras(int idCarreiras[], int numCarreiras) {
    int i, j, houveTroca = VERDADE;
    for (i = 0; houveTroca && i < numCarreiras-1; i++) {
        houveTroca = FALSO;
	for (j = 0; j < numCarreiras-1-i; j++) {
	    if (strcmp(_carreiras[idCarreiras[j]].nome, _carreiras[idCarreiras[j+1]].nome) > 0) {
	        int aux = idCarreiras[j];
	        idCarreiras[j] = idCarreiras[j+1];
		idCarreiras[j+1] = aux;
		houveTroca = VERDADE;
	    }
	}
    }
}

/* Função para tratar o comando 'i'. */

void intersecoes() {
    int i, c, idCarreira, numCarreiras;

    leAteFinalLinha(_buffer);

    for (i = 0; i < _numParagens; i++) {
        numCarreiras = _paragens[i].numCarreiras;
        if (numCarreiras > 1) {
            printf("%s %d:", _paragens[i].nome, numCarreiras);
            ordenaCarreiras(_paragens[i].idCarreiras, numCarreiras);
            for (c = 0; c < numCarreiras; c++) {
                idCarreira = _paragens[i].idCarreiras[c];
                printf(" %s", _carreiras[idCarreira].nome);
            }
            printf("\n");
        }
    }
}


/* Função MAIN */

int main() {
    int c;

    do {
        c = getchar();
        switch(c) {
            case 'c':
                carreiras();
                break;
            case 'p':
                paragens();
                break;
            case 'l':
                ligacoes();
                break;
            case 'i':
                intersecoes();
                break;
	    case 'q':
	        break;
            default:
	        /* Ignorar linhas em branco */
	        if (c == ' ' || c == '\t' || c == '\n') break;
        }
    } while (c != 'q');
    return 0;
}
