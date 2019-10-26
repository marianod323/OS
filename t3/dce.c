#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>
#include <stdbool.h>
#include <semaphore.h>

//
// Trabalho 3: DCE - Sistemas Operacionais
// Mariano Dorneles de Freitas
//

sem_t ficha[5], porta, bols;
int qnt, tabela_guia[5];

void *bolsista(){
	while (true){
		sem_wait(&bols);
		printf("Bolsista: Cheguei, vou trabalhar um pouquinho!\n\n");
		int i;
		// Percorre o vetor ficha e atende cada ficha por ordem de chegada
		for(i = 0; i < 5; i++){
			printf("Bolsista: Checando os documentos da ficha %d\n", tabela_guia[i]);
			printf("Bolsista: Tudo certo com a ficha %d, entregando carteirinha...\n\n", tabela_guia[i]);
			sem_post(&ficha[i]);
		}
		qnt = 0; // Reinicia o contador de alunos na sala
		printf("Bolsista: Ja trabalhei bastante, sai, abri a porta e vou dar um rolezinho...\n\n");
		sem_post(&porta);
	}
}

// Thread do aluno
void aluno(int id_ficha){
	while (true){
		sem_wait(&porta); // Fecha a porta
		int num_fila = qnt++; // Pega um "numero de atendimento"
		tabela_guia[num_fila] = id_ficha; // Registra o ID no numero de atendimento correspondente
		printf("Ficha %d: Entrei, trouxe meus documentos!\n", id_ficha);
		
		if(qnt == 5){
			sem_post(&bols); // Chama o bolsista
		}
		else{
			sem_post(&porta); // Abre a porta
		}
		printf("Ficha %d: Vou esperar pela minha carteirinha!\n", id_ficha);
		// Aguarda pela sua carteira
		sem_wait(&ficha[num_fila]);
		printf("Ficha %d: Recebi minha carteirinha!\n\n", id_ficha);
		sleep(rand()%10);
	}
}

// Função que inicializa uma ficha (thread de aluno)
void* aluno_thread(void* arg){
	int id_ficha = *(int *) arg;
	aluno(id_ficha);
}

int main(int argc, char *argv[]){
	// gcc -o dce dce.reborn.c -lpthread
    srand(time(NULL));
	int n_fichas = 5, i;
	sem_init(&porta, 0, 1); // Porta para mutex
	sem_init(&bols, 0, 0);  // Semáforo do bolsista (inicia bloqueado)
	for (i = 0; i < 5; i++){
		sem_init(&ficha[i], 0, 0);
	}
    
    // Verifica se foi passado o número de fichas por parâmetro e ele é maior que 5
	if (argc > 1 && atoi(argv[1]) > 5)
		n_fichas = atoi(argv[1]);
	pthread_t fichas[n_fichas], dce;
	int id_ficha[n_fichas];

	// Inicializa as threads
	pthread_create(&dce, NULL, bolsista, NULL);
	for(i = 0; i < n_fichas; i++){
		id_ficha[i] = i;
		pthread_create(&fichas[i], NULL, aluno_thread, &id_ficha[i]);
	}

	// Join das threads
	pthread_join(dce, NULL);
	for(i = 0; i < n_fichas; i++){
		pthread_join(fichas[i], NULL);
	}
}