#include <stdio.h>
#include <stdlib.h>
#include "archive.h"

char* read_tip (char* arc_name){ // Funcao para ler a dica do dia
	FILE *tips_db = fopen(arc_name, "r");
	if (tips_db == NULL){
		printf("File not found\n");
		return NULL;
	}
	char *tip = malloc ((sizeof (char))*512), ch;
	int index = 0, line = rand()%count_lines(arc_name);

	while (line != 0){
		ch = fgetc(tips_db);
		if (ch == '\n'){
			line = line - 1;
		}
	}

	do{
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
	int arc_lines = 0, eof;
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

void create_tip (char* arc_name, char* tip){
	FILE* archive = fopen(arc_name, "w");
	if (archive == NULL){
		printf("Error in opening file.\n");
		return;
	}

	fprintf(archive, "%s", tip);

	fclose(archive);
	return;
}
