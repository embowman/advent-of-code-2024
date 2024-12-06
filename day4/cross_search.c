#include <stdio.h>
#include <string.h>
#include <regex.h>

#define SIZE 140

void searchGrid(char grid[SIZE+2][SIZE+2]) {
    int total_count = 0;
    for (int r=1; r<SIZE-1; r++) {
        for (int c=1; c<SIZE-1; c++) {
            if (strncmp(&grid[r][c], "A", 1) != 0) continue;
            if (
                ((strncmp(&grid[r-1][c-1], "M", 1) == 0) && (strncmp(&grid[r+1][c+1], "S", 1) == 0)
                || (strncmp(&grid[r-1][c-1], "S", 1) == 0) && (strncmp(&grid[r+1][c+1], "M", 1) == 0))
                &&
                ((strncmp(&grid[r-1][c+1], "M", 1) == 0) && (strncmp(&grid[r+1][c-1], "S", 1) == 0)
                || (strncmp(&grid[r-1][c+1], "S", 1) == 0) && (strncmp(&grid[r+1][c-1], "M", 1) == 0))
            ) {
                total_count++;
            }
        }
    }
    printf("%d\n", total_count);
}

int main() {
    // open word search file
    FILE *restrict stream = fopen("word_search.txt", "r");
    if (stream == NULL) {
        perror("Cannot open file\n");
        return 1;
    }

    // load grid
    char grid[SIZE+2][SIZE+2];
    int i = 0;
    while (fgets(grid[i], sizeof(grid[i]), stream) != NULL) {
        grid[i][SIZE] = 0;
        i++;
    }
    fclose(stream);

    searchGrid(grid);

    return 0;
}
