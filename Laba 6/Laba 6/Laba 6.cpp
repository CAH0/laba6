
#define _CRT_SECURE_NO_WARNINGS
#include <time.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <windows.h>
#include "string.h"
#include <cmath>

int** createG(int size) {
	int** G;
	G = (int**)malloc(size * sizeof(int*));
	for (int i = 0; i < size; i++) {
		G[i] = ((int*)malloc(size * sizeof(int)));
	}

	for (int i = 0; i < size; i++) {
		for (int j = i; j < size; j++) {
			G[i][j] = rand() % 2;
			if (i == j) G[i][j] = 0;
			G[j][i] = G[i][j];
		}
	}
	return G;
}

void** printG(int** G, int size) {
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			printf("%d ", G[i][j]);
		}
		printf("\n");
	}
	return 0;
}

// удаление вершины  из графа
int** delG(int** G, int size, int v) {
	int** Gtemp = createG(size - 1);
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i < v && j < v) Gtemp[i][j] = G[i][j];
			if (i > v && j > v) Gtemp[i - 1][j - 1] = G[i][j];
			if (i > v && j < v) Gtemp[i - 1][j] = G[i][j];
			if (i < v && j > v) Gtemp[i][j - 1] = G[i][j];
		}
	}
	for (int i = 0; i < size; i++)  free(G[i]);
	free(G);
	return Gtemp;
}

//отождествление вершин(объединение двух вершин в графе)
int** unionV(int** G, int size, int v1, int v2) {
	for (int i = 0; i < size; i++) {
		if (G[v2][i] == 1) {
			G[v1][i] = G[v2][i];
			G[i][v1] = G[i][v2];
		}
		// создаём петлю
		if (G[v1][v2] == 1) {
			G[v1][v1] = 1;
		}
		G = delG(G, size, v2);
		return G;
	}
	
}

//удалени ребра между двумя вершинами
int** contrE(int** G, int size, int v1, int v2) {
	if (G[v1][v2] != 1) {
		printf("Ребра нет\n");
		return G;
	}
	else {
		G[v1][v2] = 0;
		G[v2][v1] = 0;

		for (int i = 0; i < size; i++) {
			if (G[v2][i] == 1) {
				G[v1][i] = G[v2][i];
				G[i][v1] = G[i][v2];
			}
			G = delG(G, size, v2);
			return G;
		}
	}
	

}
//Разделение указанное врешины v в графе
int** SplitV(int** G, int size, int v) {
	int** Gtemp = createG(size + 1);

	for (int i = 0; i < size; i++)
	{
		Gtemp[i][size] = G[i][v];
		Gtemp[size][i] = G[v][i];
	}

	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			Gtemp[i][j] = G[i][j];
		}
	}
	Gtemp[v][v] = 0;
	Gtemp[size][size] = 0;

	for (int i = 0; i < size; i++)
	{
		free(G[i]);
	}

	free(G);
	G = NULL;

	return Gtemp;
}
//объединение двух графов
int** unicoG(int** G1, int** G2, int size1, int size2) {

	int sizemax = (size1 > size2) ? size1 : size2;
	int sizemin = (size1 < size2) ? size1 : size2;
	int** Gmax = (size1 > size2) ? G1 : G2;
	int** Gmin = (size1 < size2) ? G1 : G2;
	int** Gtemp = createG(sizemax);

	for (int i = 0; i < sizemin; i++) {
		for (int j = 0; j < sizemin; j++) {
			Gtemp[i][j] = Gmin[i][j] | Gmax[i][j];
		}
	}

	for (int i = 0; i < sizemin; i++) {
		for (int j = 0; j < sizemin; j++) {
			Gtemp[i][j] = Gmax[i][j];
			Gtemp[j][i] = Gtemp[i][j];
		}
	}
	return Gtemp;
}
// Матрица смежночти имеющая свзяи более крупного грава иск те, который присудств в меньшем
int** xorG(int** G1, int** G2, int size1, int size2) {
	int sizemax = (size1 > size2) ? size1 : size2;
	int sizemin = (size1 < size2) ? size1 : size2;
	int** Gmax = (size1 > size2) ? G1 : G2;
	int** Gmin = (size1 < size2) ? G1 : G2;
	int** Gtemp = createG(sizemax - sizemin);

	for (int i = sizemin; i < sizemax; i++) {
		for (int j = sizemin; j < sizemax; j++) {
			Gtemp[i - sizemin][j - sizemin] = Gmax[i][j];

		}

	}
	return Gtemp;
}

// Кольцевая сумма двух графов
int** ringSum(int** G1, int size1, int** G2, int size2) {
	// Создаем новый граф, который будет содержать кольцевую сумму
	int newSize = size1 + size2 - 2; // Уменьшаем на 2, так как удаляем по одной вершине из каждого графа
	int** Gnew = createG(newSize);

	// Копируем данные из первого графа, пропуская одну вершину
	for (int i = 0; i < size1; i++) {
		for (int j = 0; j < size1; j++) {
			if (i != 0 && j != 0) { // Пропускаем первую вершину
				Gnew[i - 1][j - 1] = G1[i][j];
			}
		}
	}

	// Копируем данные из второго графа, пропуская одну вершину
	for (int i = 0; i < size2; i++) {
		for (int j = 0; j < size2; j++) {
			if (i != 0 && j != 0) { // Пропускаем первую вершину
				Gnew[i + size1 - 2][j + size1 - 2] = G2[i][j];
			}
		}
	}

	// Соединяем графы в кольцевую структуру
	for (int i = 0; i < size1 - 1; i++) {
		Gnew[i][i + 1] = 1;
		Gnew[i + 1][i] = 1;
	}
	Gnew[0][size1 - 2] = 1; // Соединяем последнюю вершину с первой в новом графе
	Gnew[size1 - 2][0] = 1; // Обратное соединение

	for (int i = 0; i < size2 - 1; i++) {
		Gnew[size1 - 2 + i][size1 - 1 + i] = 1;
		Gnew[size1 - 1 + i][size1 - 2 + i] = 1;
	}
	Gnew[size1 - 2 + (size2 - 2)][size1 - 2] = 1; // Соединяем последнюю вершину с первой в новом графе
	Gnew[size1 - 2][size1 - 2 + (size2 - 2)] = 1; // Обратное соединение

	return Gnew;
}


int main()
{
	srand(time(NULL));
	setlocale(LC_ALL, "Rus");
	int** G = NULL, count = 0, * deg = NULL, * loop = NULL;
	int size1 = 0, size2 = 0;

	printf("Введите размер матрицы: ");
	scanf("%d", &size1);
	printf("Введите размер матрицы: ");
	scanf("%d", &size2);

	int** G1 = createG(size1);
	int** G2 = createG(size2);

	printf("\nПервая\n");
	printG(G1, size1);

	printf("\nВторая\n");
	printG(G2, size2);

	printf("\nG1 = delG(G1, size1, 2);\n");
	G1 = delG(G1, size1, 2);
	printG(G1, --size1);

	printf("\nG2 = delG(G2, size2, 4);\n");
	G2 = delG(G2, size2, 4);
	printG(G2, --size2);

	printf("\nG1 = unionV(G1, size1, 2, 3);\n");
	G1 = unionV(G1, size1, 2, 3);
	printG(G1,--size1);

	printf("\nG2 = unionV(G2, size2, 2, 3);\n");
	G2 = unionV(G2, size2, 2, 3);
	printG(G2, --size2);

	printf("\nG1 = contrE(G1, size1, 2, 3)\n");
	G1 = contrE(G1, size1, 2, 3);
	printG(G1,--size1);

	printf("\nG2 = contrE(G2, size2, 2, 3)\n");
	G2 = contrE(G2, size2, 2, 3);
	printG(G2, --size2);


	printf("\nG1 = SplitV(G1, size1, 2);\n");
	G1 = SplitV(G1, size1, 2);
	printG(G1,++size1);

	printf("\nG2 = SplitV(G2, size2, 2);\n");
	G2 = SplitV(G2, size2, 2);
	printG(G2,++size2);


	printf("\nG1 = unicoG(G1, G2, size1, size2);\n");
	G1 = unicoG(G1, G2, size1, size2);
	printG(G1,(size1<size2)?size1:size2);


	printf("\nG1 = ringSum(G1, size1, G2, size2);\n");
	int** G_ring = ringSum(G1, size1, G2, size2);
	printG(G_ring, abs(size1 + size2) - 2);
	return 0;
}
