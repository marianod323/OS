#include <stdio.h>
#include <stdlib.h>
#include "archive.h"

char* read_tip (char* arc_name){ // Funcao para ler a dica do dia
	FILE *tips_db = fopen(arc_name, "r");
	if (tips_db == NULL){
		printf("Archive not found\n");
		return NULL;
	}
	char *tip = malloc ((sizeof (char))*512), ch;
	int index = 0, line = rand()%count_lines(arc_name);

	/*while (){

	}*/

	while (ch != '\n'){
		ch = fgetc(tips_db);
		tip[index] = ch;
		index++;
	}
	tip[index] = '\0';

	fclose(tips_db);
	return tip;
}

int count_lines (char* arc_name){ // Funcao para contar as linhas do arquivo
	FILE* archive = fopen(arc_name, "r");
	if (archive == NULL){
		printf("Archive not found\n");
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

void main(){
	printf("%s\n", read_tip("banco_de_dicas.txt"));
}
