
#include <stdio.h>
#include <stdlib.h>

int longitudLinea(int *D,int cElementosV, double radio, double *longitudes) {
	for (int i = 0; i < cElementosV; i++) {
		longitudes[i] = (double)D[i] / cElementosV * radio;
	}
};

void colorLinea(int *D, int cElementosV, int color1[3], int color2[3], int colores[][3]) {
	for (int i = 0; i < cElementosV; i++) {
		for (int j = 0; j < 3; j++) {
			colores[i][j] = color1[j] + ((D[i] - 1) * (color2[j] - color1[j])) / (cElementosV - 1);
		}
	}
}