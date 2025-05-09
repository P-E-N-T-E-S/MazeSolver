#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <dirent.h>

#define MAX 1024

typedef struct {
    int row, col;
} Point;

typedef struct Node {
    Point pt;
    struct Node* parent;
} Node;

int is_valid(char** maze, int rows, int cols, int r, int c) {
    return (r >= 0 && r < rows && c >= 0 && c < cols &&
            (maze[r][c] == ' ' || maze[r][c] == 'E'));
}

float solve_maze(const char* input_filename, const char* output_filename) {

    clock_t start_time = clock();

    FILE* f = fopen(input_filename, "r");
    if (!f) {
        perror("Erro ao abrir o arquivo de entrada");
        return -1;
    }

    char* maze[MAX];
    int rows = 0;

    char buffer[2048];
    while (fgets(buffer, sizeof(buffer), f)) {
        maze[rows] = strdup(buffer);
        rows++;
    }
    fclose(f);


    int cols = strlen(maze[0]) - 1;

    Point start, end;
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            if (maze[i][j] == 'S') start = (Point){i, j};
            if (maze[i][j] == 'E') end = (Point){i, j};
        }

    Node** queue = malloc(sizeof(Node*) * MAX * MAX);
    int front = 0, rear = 0;
    int** visited = malloc(sizeof(int*) * rows);
    for (int i = 0; i < rows; i++) {
        visited[i] = calloc(cols, sizeof(int));
    }

    Node* start_node = malloc(sizeof(Node));
    start_node->pt = start;
    start_node->parent = NULL;

    queue[rear++] = start_node;
    visited[start.row][start.col] = 1;

    Node* end_node = NULL;
    int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};

    while (front < rear) {
        Node* curr = queue[front++];
        int r = curr->pt.row;
        int c = curr->pt.col;

        if (r == end.row && c == end.col) {
            end_node = curr;
            break;
        }

        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (is_valid(maze, rows, cols, nr, nc) && !visited[nr][nc]) {
                visited[nr][nc] = 1;
                Node* next = malloc(sizeof(Node));
                next->pt = (Point){nr, nc};
                next->parent = curr;
                queue[rear++] = next;
            }
        }
    }

    Node* p = end_node;
    while (p && !(p->pt.row == start.row && p->pt.col == start.col)) {
        if (maze[p->pt.row][p->pt.col] == ' ')
            maze[p->pt.row][p->pt.col] = '.';
        p = p->parent;
    }

    FILE* out = fopen(output_filename, "w");
    if (!out) {
        perror("Erro ao escrever o arquivo de saída");
        return -1;
    }

    for (int i = 0; i < rows; i++) {
        fputs(maze[i], out);
        free(maze[i]);
        free(visited[i]);
    }

    fclose(out);
    free(visited);
    free(queue);

    clock_t end_time = clock();
    float duration_ms = ((float)(end_time - start_time) / CLOCKS_PER_SEC) * 1000;

    return duration_ms;
}

void process_all_mazes(const char* input_folder, const char* output_folder) {
    DIR* dir = opendir(input_folder);
    if (!dir) {
        perror("Erro ao abrir a pasta de entrada");
        return;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strstr(entry->d_name, ".txt")) {
            char input_path[512], output_path[512];
            snprintf(input_path, sizeof(input_path), "%s/%s", input_folder, entry->d_name);
            snprintf(output_path, sizeof(output_path), "%s/solved_%s", output_folder, entry->d_name);

            printf("Resolvendo %s...\n", entry->d_name);
            float time = solve_maze(input_path, output_path);
            if (time >= 0) {
                printf("Resolvido em %.2f ms ", time);
            }
        }
    }

    closedir(dir);
}
