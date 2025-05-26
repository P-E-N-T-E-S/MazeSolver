#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX 2048


float solve_maze(const char* maze_str, const char* output_filename);

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Uso: %s \"maze_string\"\n", argv[0]);
        return 1;
    }

    float tempo = solve_maze(argv[1], "output_algcf_egcf_spscl_phss.txt");

    if (tempo >= 0) {
        printf("%.2f", tempo);
    }

    return 0;
}

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

float solve_maze(const char* maze_str, const char* output_filename) {
    char* maze[MAX];
    int rows = 0;
    char* str_copy = strdup(maze_str);

    char *src = str_copy, *dst = str_copy;
    clock_t start_time = clock();
    while (*src) {
        if (src[0] == '\\' && src[1] == 'n') {
            *dst++ = '\n';
            src += 2;
        } else {
            *dst++ = *src++;
        }
    }
    *dst = '\0';

    char* line = strtok(str_copy, "\n");
    while (line && rows < MAX) {
        maze[rows++] = strdup(line);
        line = strtok(NULL, "\n");
    }

    int cols = strlen(maze[0]);

    Point start, end;
    int start_found = 0, end_found = 0;

    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) {
            if (maze[i][j] == 'S') {
                start = (Point){i, j};
                start_found = 1;
            }
            if (maze[i][j] == 'E') {
                end = (Point){i, j};
                end_found = 1;
            }
        }

    if (!start_found || !end_found) {
        fprintf(stderr, "Labirinto deve conter exatamente um 'S' e um 'E'\n");
        free(str_copy);
        for (int i = 0; i < rows; i++) free(maze[i]);
        return -1;
    }

    int** visited = calloc(rows, sizeof(int*));
    for (int i = 0; i < rows; i++) {
        visited[i] = calloc(cols, sizeof(int));
    }

    Node** queue = malloc(sizeof(Node*) * rows * cols);
    int front = 0, rear = 0;

    Node* start_node = malloc(sizeof(Node));
    start_node->pt = start;
    start_node->parent = NULL;

    queue[rear++] = start_node;
    visited[start.row][start.col] = 1;

    Node* end_node = NULL;
    int dr[] = {-1, 1, 0, 0}, dc[] = {0, 0, -1, 1};

    while (front < rear) {
        Node* curr = queue[front++];
        int r = curr->pt.row, c = curr->pt.col;

        if (r == end.row && c == end.col) {
            end_node = curr;
            break;
        }

        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d], nc = c + dc[d];
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

    for (int i = 0; i < rear; i++) free(queue[i]);
    free(queue);

    for (int i = 0; i < rows; i++) free(visited[i]);
    free(visited);

    clock_t end_time = clock();
    float duration_ms = ((float)(end_time - start_time) / CLOCKS_PER_SEC) * 1000;

    FILE* out = fopen(output_filename, "w");
    if (!out) {
        perror("Erro ao escrever o arquivo de saída");
        for (int i = 0; i < rows; i++) free(maze[i]);
        free(str_copy);
        return -1;
    }

    for (int i = 0; i < rows; i++) {
        fprintf(out, "%s\n", maze[i]);
        free(maze[i]);
    }

    fclose(out);
    free(str_copy);
    return duration_ms;
}