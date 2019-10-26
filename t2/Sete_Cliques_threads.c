#include "curl_code.h"
#include <unistd.h>
#include <pthread.h>
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"
#define RESET "\033[0m"

// Verifica se a palavra encontra-se na pagina
bool isWordFound(char *word, char *page_content){
	return strstr(page_content, word) == NULL ? false : true;
}

// Funcao para verificar se o link sorteado eh valido
bool isValidLink(char* chosenLink, char* search_page){
	return !(isWordFound("https://pt.wikipedia.org/wiki/Wikipedia:", chosenLink) ||
		   isWordFound("https://pt.wikipedia.org/wiki/Wikipédia:", chosenLink) ||
		   isWordFound("https://pt.wikipedia.org/wiki/Ficheiro:", chosenLink) ||
		   isWordFound("https://pt.wikipedia.org/wiki/Categoria:", chosenLink) ||
		   isWordFound("https://pt.wikipedia.org/wiki/Especial:", chosenLink) ||
		   !isWordFound("https://pt.wikipedia.org/wiki/", chosenLink) ||
		   isWordFound(search_page, chosenLink));
}

// Funcao que joga o jogo dos sete cliques
void seven_clicks_t(char *word, char *search_page, int click, int t_id){

	// Extrai o conteudo da página
	CURL *curl_handle;
	char *page_content = download_page(curl_handle, search_page);

	// Verifica se a palavra procurada se encontra na página extraida
	if (isWordFound(word, page_content)){
		printf("\nThread %s%d%s:\n", KYEL, t_id, RESET);
		printf("%sClique %d encontrou a palavra %s no link: %s%s\n\n", KRED, click,
		word, search_page, RESET);
		return;
	}

	// Procura os links da pagina extraida
	int links_readed;
	char **links = find_links(curl_handle, page_content, 50, &links_readed);

	// Sorteia um link valido
	int random;
	do{
		random = rand() % links_readed;
	}while(!isValidLink(links[random], search_page));

	// Faz a verificação se é o último, primeiro ou outro clique qualquer
 	if (click == 7){
		// Se for o último, informa que a palavra não foi encontrada
		printf("\nThread %s%d%s:\n", KYEL, t_id, RESET);
		printf("Clique %s%d%s nao encontrou a palavra %s%s%s no link: %s%s%s\n", KCYN, click, RESET,
		KCYN, word, RESET, KCYN, search_page, RESET);
		printf("%sA palavra nao foi encontrada%s\n\n", KRED, RESET);
		return;
	}
	else if (click != 0){
		printf("\nThread %s%d%s:\n", KYEL, t_id, RESET);
		printf("Clique %s%d%s nao encontrou a palavra %s%s%s no link: %s%s%s\n"
			   "Proximo link sorteado: %s%s%s\n", KCYN,
			   click, RESET, KCYN, word, RESET, KCYN, search_page, RESET,
			   KBLU, links[random], RESET);
	}
	else{
		printf("\nThread %s%d%s:\n", KYEL, t_id, RESET);
		printf("Procurando a palavra %s%s%s, a partir de %s%s%s.\nPrimeiro link sorteado: %s%s%s\n",
		KCYN, word, RESET, KCYN, search_page, RESET, KBLU, links[random], RESET);
	}
	curl_global_cleanup();

	// Faz a chamada da recursão para o próximo clique do jogo
	seven_clicks_t(word, links[random], click + 1, t_id);
}

// Funcao que inicializa as threads
void *thread_func(void *arg){

	// Pega o id da thread, que veio pelo argumento
	int id_t = *(int *) arg;

	// Inicializa uma jogada
	seven_clicks_t("Presidente", "https://pt.wikipedia.org/wiki/Futebol", 0, id_t);
	return 0;
}

int main(int argc, char *argv[]){
	srand(time(NULL));
	int n_threads = 1, i;

	// Verifica se foi passado o número de threads por parâmetro
	if (argc > 1)
		n_threads = atoi(argv[1]);
	pthread_t thread[n_threads];
	int t_id[n_threads];

	// Inicializa as threads
	for(i = 0; i < n_threads; i++){
		t_id[i] = i;
		pthread_create(&thread[i], NULL, thread_func, &t_id[i]);
	}

	// Espera todas as threads terminarem
	for(i = 0; i < n_threads; i++){
		pthread_join(thread[i], NULL);
	}

	return 0;
}
