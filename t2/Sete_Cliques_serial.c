#include "curl_code.h"
#include <unistd.h>
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

// Verifica se o link sorteado eh valido
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
void seven_clicks(char *word, char *search_page, int click){

	// Extrai o conteudo da página
	CURL *curl_handle;
	char *page_content = download_page(curl_handle, search_page);

	// Verifica se a palavra procurada se encontra na página extraida
	if (isWordFound(word, page_content)){
		printf("\nClique %s%d%s encontrou a palavra %s%s%s no link: %s%s%s\n\n", KCYN, click , RESET,
		KCYN, word, RESET, KCYN, search_page, RESET);
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
		printf("\nClique %s%d%s nao encontrou a palavra %s%s%s no link: %s%s%s\n", KCYN, click, RESET,
		KCYN, word, RESET, KCYN, search_page, RESET);
		printf("%sA palavra nao foi encontrada%s\n\n", KRED, RESET);
		return;
	}
	else if (click != 0){
		printf("\nClique %s%d%s nao encontrou a palavra %s%s%s no link: %s%s%s\n"
			   "Proximo link sorteado: %s%s%s\n", KCYN,
			   click, RESET, KCYN, word, RESET, KCYN, search_page, RESET,
			   KBLU, links[random], RESET);
	}
	else{
		printf("\nProcurando a palavra %s%s%s, a partir de %s%s%s.\nPrimeiro link sorteado: %s%s%s\n",
		KCYN, word, RESET, KCYN, search_page, RESET, KBLU, links[random], RESET);
	}

	curl_global_cleanup();

	// Faz a chamada da recursão para o próximo clique do jogo
	seven_clicks(word, links[random], click + 1);
}

int main(void){
	srand(time(NULL));
	seven_clicks("Presidente", "https://pt.wikipedia.org/wiki/Futebol", 0);
	return 0;
}
