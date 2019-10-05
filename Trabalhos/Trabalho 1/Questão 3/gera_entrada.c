#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char const *argv[]){
	if(argc < 2){
		fprintf(stderr, "Número de argumentos insuficientes!\n");

		return 1;
	}

	FILE* file = fopen("xydata", "w");
	char buffer[50];
	double x, y;

	if(file == NULL){
		fprintf(stderr, "Falha ao criar arquivo!\n");

		return 2;
	}

	fputs(argv[1], file);
	fputs("\n", file);
	srand(time(0));

	for (int i = 0; i < atoi(argv[1]); ++i){
		x = (((double)rand()/(double)(RAND_MAX)) * 1000) - (((double)rand()/(double)(RAND_MAX)) * 1000); // Cria numeros aleatórios entre -1000 e 1000

		sprintf(buffer, "%g", x);
		fputs(buffer, file);
		fputs(" ", file);

		y = x + 5/*((((double)rand()/(double)(RAND_MAX)) * 1000) - (((double)rand()/(double)(RAND_MAX)) * 1000))*/; // y = m*x + b onde m = 1, b =5

		if(i > 0){
        	y = y + (((double)rand()/(double)(RAND_MAX)) * 100) - (((double)rand()/(double)(RAND_MAX)) * 100); // Introduz uma mutação na equação
        }

		sprintf(buffer, "%g", y);
		fputs(buffer, file);
		fputs("\n", file);
	}

	fclose(file);
	return 0;
}
