#include <stdio.h>
#include "function.h"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Uso: %s \"maze_string\"\n", argv[0]);
        return 1;
    }

    float tempo = solve_maze(argv[1], "saida.txt");

    if (tempo >= 0) {
        printf("Labirinto resolvido em %.2f ms\n", tempo);
    }

    return 0;
}
