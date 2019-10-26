#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "archive.h"

char* read_tip (char* arc_name){ // Funcao para ler a dica do dia
	FILE *tips_db = fopen(arc_name, "r");
	if (tips_db == NULL){
		printf("File not found\n");
		return NULL;
	}
	char *tip = malloc ((sizeof (char))*512), ch;
	srand(time(NULL)+getpid()); // Utiliza o pid para gerar uma seed
	int index = 0, line = rand()%count_lines(arc_name);	// Sorteia a dica do dia
	while (line != 0){	// Chega à linha sorteada
		ch = fgetc(tips_db);
		if (ch == '\n'){
			line = line - 1;
		}
	}

	do{	// Copia a linha sorteada ao array auxiliar
		ch = fgetc(tips_db);
		tip[index] = ch;
		index++;
	}while (ch != '\n');

	tip[index] = '\0';

	fclose(tips_db);
	return tip;
}

int count_lines (char* arc_name){ // Funcao para contar as linhas do arquivo
	FILE* archive = fopen(arc_name, "r");
	if (archive == NULL){
		printf("File not found\n");
		return -1;
	}
	int arc_lines = 0;
	char ch;

	while(!feof(archive)){
	  ch = fgetc(archive);
	  if(ch == '\n'){
	    arc_lines++;
	  }
	}

	fclose(archive);
	return arc_lines;
}

int create_line (char* arc_name, char* tip){ // Funcao para criar o arquivo da dica do dia
	FILE* archive = fopen(arc_name, "w");
	if (archive == NULL){
		printf("Error in creating file.\n");
		return -1;
	}

	fprintf(archive, "%s", tip);

	fclose(archive);
	return 1;
}
