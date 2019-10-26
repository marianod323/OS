#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>

sem_t ficha[5], porta, bols;
int qnt, lock_table[5];

void *bolsista(){
	while (true){
		sem_wait(&bols);
		printf("Bolsista: Cheguei, vou trabalhar um pouquinho!\n\n");
		int i;
		for(i = 0; i < 5; i++){
			printf("Bolsista: Checando os documentos da ficha %d\n", lock_table[i]);
			printf("Bolsista: Tudo certo com a ficha %d, entregando carteirinha...\n\n", lock_table[i]);
			sem_post(&ficha[i]);
		}
		qnt = 0;
		printf("Bolsista: Ja trabalhei bastante, fechei a porta e vou dar um rolezinho...\n\n");
		sem_post(&porta);
	}
}

void aluno(int num_ficha){
	while (true){
		sem_wait(&porta);
		int num_fila = qnt++;
		lock_table[num_fila] = num_ficha;
		printf("Ficha %d: Entrei, trouxe meus documentos!\n", num_ficha);
		
		if(qnt == 5){
			sem_post(&bols);
		}
		else{
			sem_post(&porta);
		}
		printf("Ficha %d: Vou esperar pela minha carteirinha!\n", num_ficha);
		sem_wait(&ficha[num_fila]);
		printf("Ficha %d: Recebi minha carteirinha!\n\n", num_ficha);
		sleep(rand()%10);
	}
}

void* aluno_thread(void* arg){
	int num_ficha = *(int *) arg;
	aluno(num_ficha);
}

int main(int argc, char *argv[]){
    srand(time(NULL));
	int n_fichas = 5, i;
	sem_init(&porta, 0, 1);
	sem_init(&bols, 0, 0);
	for (i = 0; i < 5; i++){
		sem_init(&ficha[i], 0, 0);
	}
    
    // Verifica se foi passado o número de fichas por parâmetro e ele é maior que 5
	if (argc > 1 && atoi(argv[1]) > 5)
		n_fichas = atoi(argv[1]);
	pthread_t fichas[n_fichas], dce;
	int num_ficha[n_fichas];

	// Inicializa as threads
	pthread_create(&dce, NULL, bolsista, NULL);
	for(i = 0; i < n_fichas; i++){
		num_ficha[i] = i;
		pthread_create(&fichas[i], NULL, aluno_thread, &num_ficha[i]);
	}

	pthread_join(dce, NULL);
	for(i = 0; i < n_fichas; i++){
		pthread_join(fichas[i], NULL);
	}
}