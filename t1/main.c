#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "archive.h"

int main() {
    srand((unsigned)time(NULL));
    create_tip("dica_do_dia.txt", read_tip("banco_de_dicas.txt"));
    return 0;
}
