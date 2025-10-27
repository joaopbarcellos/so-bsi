#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include <pthread.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

#define NUM_MAX 31999
#define COLUNAS 10000
#define LINHAS 10000

#define SEED 67


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
	

int main() {
	srand(SEED);
	int** matriz = aloca_matriz(LINHAS, COLUNAS);

	preenche_matriz(LINHAS, COLUNAS, matriz);
	
	pthread_t tid;

	matriz = liberar_matriz(LINHAS, COLUNAS, matriz);
	return 0;
}


int eh_primo(int n) {
	if (n <= 1) return 0;

	for (int i = 2; i <= sqrt(n); i++) {
		if (n % i == 0) return 0;
	}
	return 1;
}

int busca_serial(int** matriz) {
	int count = 0;
	for (int i = 0; i < LINHAS; i++) {
		for (int j = 0; j < COLUNAS; j++) {
			if (eh_primo(matriz[i][j])) {
				count++;
			}
		}
	}
	return count;
}
