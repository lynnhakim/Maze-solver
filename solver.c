#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int rows = 0;
int cols = 0;
char **maze;
int **visited;

enum grid {
    empty,
    wall, 
    crumb
};

void read_file(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Invalid input\n");
        return; 
    }

    char buffer[10000];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        rows++;
        if (rows == 1) {
            cols = strlen(buffer) - 1; 
        }
    }
   

    fclose(file);
}

void populate_maze(char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Unable to open file\n");
        return; 
    }

    maze = (char **)malloc(rows * sizeof(char *));
    for (int i = 0; i < rows; i++) {
        maze[i] = (char *)malloc((cols + 1) * sizeof(char)); 
    }

    for (int i = 0; i < rows; i++) {
        fgets(maze[i], cols + 1, file); 
        fgetc(file); 
    }

    fclose(file);
}

void populate_visited() {
    visited = malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; i++) {
        visited[i] = malloc(cols * sizeof(int)); 
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++ ){
            if (maze[i][j] == '*') {
                visited[i][j] = empty; 
            } else {
                visited[i][j] = wall;
            }
        }
    }
}

void print_maze(char **maze) {
    for (int i = 0; i < rows; i++) {
        printf("%s\n", maze[i]); 
    }
}

void add_crumbs(){
    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            if (visited[i][j] == crumb){
                maze[i][j] = 'o';
            }
        }
    }
}

int dfs(int row, int col) {
    if (row == rows - 1 && col == cols - 1) {
        visited[row][col] = crumb;
        return 1;
    }
    
    if (row < 0 || col < 0 || row >= rows || col >= cols || visited[row][col] == wall || visited[row][col] == crumb) {
        return 0;
    }

    visited[row][col] = crumb;

    if (dfs(row, col + 1) || dfs(row + 1, col) || dfs(row, col - 1) || dfs(row - 1, col)) {
        return 1;
    }

    visited[row][col] = empty;
    return 0;
}


void free_memory() {
    for (int i = 0; i < rows; i++) {
        free(maze[i]);
        free(visited[i]);
    }
    free(maze);
    free(visited);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1; 
    }
    
    char *filename = argv[1];
    read_file(filename);

    if (rows == 0 || cols == 0){
        printf("Empty file\n");
        return 1;
    }

    populate_maze(filename);
    populate_visited();

    printf("Here is a %d x %d maze...\n", rows, cols);
    print_maze(maze);

    int change;
    if (maze[0][0] == 'X' || maze[rows - 1][cols - 1] == 'X') change = 0;
    else change = dfs(0, 0);

    if (change) {
        add_crumbs();
        printf("\n...and a way to solve it.\n");
        print_maze(maze);
    }
    else {
        printf("No valid solution\n");
    }
    free_memory();
    return 0;
}
