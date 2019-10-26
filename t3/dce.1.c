#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>

sem_t card[5], mutex, bols;
int lot = 0, lock_table[5], door = 1;

void *bolsista(){
	while (true){
		sem_wait(&bols);
		printf("\nBolsista: Cheguei, vou trabalhar um pouquinho!\n");
		int i;
		for(i = 0; i < 5; i++, lot--){
			printf("Bolsista: Checando os documentos da ficha %d\n", lock_table[i]);
			printf("Bolsista: Tudo certo com a ficha %d, entregando carteirinha...\n", lock_table[i]);
			sem_post(&card[i]);
		}
		printf("Bolsista: Ja trabalhei bastante, vou dar um rolezinho...\n\n");
	}
}

void aluno(int num_ficha){
	while (true){
		sem_wait(&mutex);
		int num_fila = lot;
		lock_table[lot] = num_ficha;
		printf("Ficha %d: Entrei, trouxe meus documentos!\n", num_ficha);
		lot += 1;
		if(lot == 5) sem_post(&bols);
		sem_post(&mutex);

		printf("Ficha %d: Vou esperar pela minha carteirinha!\n", num_ficha);
		printf("Ficha %d: Recebi minha carteirinha!\n\n", num_ficha);
		sleep(rand()%10);
		/*if(lot == 0) sem_wait(&bols);
		int valor;
		sem_getvalue(&bols, &valor);
		printf("\nValor do vc save: %d\n", valor);
		*/
	}
}

void* aluno_thread(void* arg){
	int num_ficha = *(int *) arg;
	aluno(num_ficha);
}

int main(int argc, char *argv[]){
	//gcc -o dce dce.c -lpthread
	srand(time(NULL));
	int n_fichas = 5, i;
	sem_init(&mutex, 0, 1);
	sem_init(&bols, 0, 0);
	for (i = 0; i < 5; i++){
		sem_init(&card[i], 0, 0);
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