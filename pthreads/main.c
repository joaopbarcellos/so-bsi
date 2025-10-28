#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1
#pragma comment(lib,"pthreadVC2.lib") 
#define HAVE_STRUCT_TIMESPEC 
#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define NUM_MAX 31999
#define ALTURA 10000
#define LARGURA 9000

#define ALTURA_MACRO_BLOCO 2500
#define LARGURA_MACRO_BLOCO 2250

#define SEED 67

#define NUM_THREADS 8

int** matriz;

int cont = 0;

int quantidade_blocos;

int linhas_bloco;
int colunas_bloco;

int proximo_bloco = 1;

pthread_mutex_t cont_mutex;
pthread_mutex_t bloco_mutex;

int** aloca_matriz(int m, int n) {
	int** matriz;
	int i;

	if (m < 1 || n < 1) {
		printf("** Erro: Parametro invalido **\n");
		return NULL;
	}

	matriz = calloc(m, sizeof(int*));
	if (matriz == NULL) {
		printf("** Erro: Memoria Insuficiente **");
		return NULL;
	}

	for (i = 0; i < m; i++) {
		matriz[i] = calloc(n, sizeof(int));
		if (matriz[i] == NULL) {
			printf("** Erro: Memoria Insuficiente **");
			return NULL;
		}
	}

	return matriz;
}

int **liberar_matriz(int m, int n, int** matriz) {
	int i;
	if (matriz == NULL) return NULL;

	if (m < 1 || n < 1) return NULL;

	for (i = 0; i < m; i++) {
		free(matriz[i]);
	}
	free(matriz);
	return NULL;
}

void preenche_matriz(int m, int n, int** matriz) {
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < n; j++) {
			matriz[i][j] = rand() % NUM_MAX;
		}
	}
}
	
int eh_primo(int n) {
	if (n <= 1) return 0;

	for (int i = 2; i <= sqrt(n); i++) {
		if (n % i == 0) return 0;
	}
	return 1;
}

void busca_serial() {
	for (int i = 0; i < ALTURA; i++) {
		for (int j = 0; j < LARGURA; j++) {
			if (eh_primo(matriz[i][j])) {
				cont++;
			}
		}
	}
}


void *thread_function(void* nenhum) {

	int primos_local = 0;
	int bloco_atual;

	while (1){

		pthread_mutex_lock(&bloco_mutex);
		if (proximo_bloco > quantidade_blocos) {
			pthread_mutex_unlock(&bloco_mutex);
			break;
		}
		bloco_atual = proximo_bloco;
		proximo_bloco++;
		pthread_mutex_unlock(&bloco_mutex);

		int linha = (bloco_atual - 1) / colunas_bloco;
		int coluna = (bloco_atual - 1) % colunas_bloco;

		int linha_inicial = linha * ALTURA_MACRO_BLOCO;
		int linha_final = linha_inicial + (ALTURA_MACRO_BLOCO - 1);

		int coluna_inicial = coluna * LARGURA_MACRO_BLOCO;
		int coluna_final = coluna_inicial + (LARGURA_MACRO_BLOCO - 1);

		for (int i = linha_inicial; i <= linha_final; i++) {
			for (int j = coluna_inicial; j <= coluna_final; j++) {
				if (eh_primo(matriz[i][j])) {
					primos_local++;
				}
			}
		}

		pthread_mutex_lock(&cont_mutex);
		cont += primos_local;
		pthread_mutex_unlock(&cont_mutex);
		primos_local = 0;
	}

	pthread_exit(NULL);
}

void busca_paralela() {
	pthread_t threads[NUM_THREADS];
	cont = 0;

	linhas_bloco = (int)(ceil((double)ALTURA / ALTURA_MACRO_BLOCO));
	colunas_bloco = (int)(ceil((double)LARGURA / LARGURA_MACRO_BLOCO));

	pthread_mutex_init(&cont_mutex, NULL);
	pthread_mutex_init(&bloco_mutex, NULL);

	for (int i = 0; i < NUM_THREADS; i++) {
		if (pthread_create(&threads[i], NULL, thread_function, NULL) != 0) {
			perror("Pthread_create falhou");
			exit(1);
		}
	}

	for (int i = 0; i < NUM_THREADS; i++) {
		if (pthread_join(threads[i], NULL)) {
			perror("Pthread_join falhou");
			exit(1);
		}
	}

	pthread_mutex_destroy(&cont_mutex);
	pthread_mutex_destroy(&bloco_mutex);

}


int main() {
	srand(SEED);
	matriz = aloca_matriz(ALTURA, LARGURA);

	quantidade_blocos = (int)(ceil((double)ALTURA / ALTURA_MACRO_BLOCO) * ceil((double)LARGURA / LARGURA_MACRO_BLOCO));

	preenche_matriz(ALTURA, LARGURA, matriz);
	clock_t clock_inicio, clock_fim;
	clock_inicio = clock();
	busca_serial();
	clock_fim = clock();
	double tempo_serial = ((double)(clock_fim - clock_inicio)) / CLOCKS_PER_SEC;
	printf("Busca Serial: %d numeros primos encontrados em %lf segundos.\n", cont, tempo_serial);
	

	clock_inicio = clock();
	busca_paralela();
	clock_fim = clock();
	double tempo_paralelo = ((double)(clock_fim - clock_inicio)) / CLOCKS_PER_SEC;
	printf("Busca Paralela: %d numeros primos encontrados em %lf segundos.\n", cont, tempo_paralelo);
	printf("Realizada com %d threads e %d Macro Blocos.\n", NUM_THREADS, quantidade_blocos);
	matriz = liberar_matriz(ALTURA, LARGURA, matriz);
	double speedup = tempo_serial / tempo_paralelo;
	printf("Speedup: %lf\n", speedup);


	return 0;
}
