#include <stdio.h>
#include <stdlib.h>
#include "archive.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

char* make_path (char* str1, char* str2){   // Funcao para concatenar duas strings com uma barra no meio
    char* str3 = malloc(strlen(str1) + strlen(str2) + 10);
    strcat(str3, str1);
    strcat(str3, "/");
    strcat(str3, str2);
    return str3;
}

void spread_arc (char* dir_path, int mainPid){ // Funcao que espalha os arquivos e escreve na saida
    DIR *dp;
    struct dirent *ep;
    dp = opendir(dir_path);
    int pid;

    if (dp != NULL){
        while ((ep = readdir (dp)) != NULL){ // Varre todos os arquivos do diretorio
            if (ep->d_type == DT_DIR && strcmp(ep->d_name, ".") != 0 &&
                strcmp(ep->d_name, "..") != 0){ // Verifica se sao pastas e nao sao a . ou ..
                pid = fork(); // Fork para a pasta

                if (pid == -1){
                    printf("Fork failed\n");
                    return;
                }
                else if (pid == 0){
                    char* next_path = make_path(dir_path, ep->d_name);
                    spread_arc(next_path, mainPid);  // Recursao para vasculhar a prox pasta
                    free(next_path);
                    return;
                }
                else{
                    wait(NULL); // Faz o pai esperar pelo filho que acabou de criar
                }
            }
        }
        // Apos chegar no ultimo diretorio, cria a dica e escreve na saida
        char* tip_path = make_path(dir_path, "dica_do_dia.txt");
        create_line(tip_path, read_tip("./banco_de_dicas.txt"));

        FILE* archive = fopen("./saida.gv", "a");
        if (getpid() != mainPid){
            fprintf(archive, "\t <%d> -> <%d>;\n", getppid(), getpid());
        }
        fprintf(archive, "\t <%d> [label=\"%s, <%d>\"];\n", getpid(), dir_path, getpid());
        fclose(archive);
        
        closedir(dp);
        return;
      }
    else{
        perror ("Couldn't open the directory");
        printf("%s\n", dir_path);
    }

}

int main() {
    FILE* archive = fopen("./saida.gv", "w");
    fprintf(archive, "%s", "digraph G {\n");
    fclose(archive);

    int mainPid = getpid();
    spread_arc("./home/estagiario", mainPid);

    if (getpid() == mainPid){
        archive = fopen("./saida.gv", "a");
        fprintf(archive, "%s", "}");
        fclose(archive);
    }
    return 0;
}
