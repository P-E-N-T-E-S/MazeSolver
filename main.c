#include <stdio.h>
#include "function.h"

int main() {
    const char* input_folder = "../Maze";
    const char* output_folder = "../Output";
    process_all_mazes(input_folder, output_folder);

    return 0;
}