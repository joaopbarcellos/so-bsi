#pragma once
#define _CRT_SECURE_NO_WARNINGS 1
#define _WINSOCK_DEPRECATED_NO_WARNINGS 1

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define LENGTH 1000
#define COLS 3
#define ROWS 3

#define SEED 92


int main() {
	
	return 0;
}


int eh_primo(int n) {
	if (n <= 1) return 0;

	for (int i = 2; i <= sqrt(n); i++) {
		if (n % i == 0) return 0;
	}
	return 1;
}
