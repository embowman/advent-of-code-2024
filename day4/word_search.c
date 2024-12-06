#include <stdio.h>
#include <string.h>
#include <regex.h>

#define SIZE 140

int searchPath(char grid[SIZE+2][SIZE+2], int r, int c, int dr, int dc) {
    char buffer[SIZE+1];
    char *cursor = buffer;
    int k = 0;
    while (r >= 0 && r < SIZE && c >= 0 && c < SIZE) {
        buffer[k++] = grid[r][c];
        r += dr;
        c += dc;
    }
    buffer[k] = 0;

    regex_t regex;
    regmatch_t match[1];
    int count = 0;
    regcomp(&regex, "XMAS", REG_EXTENDED);
    while (regexec(&regex, cursor, 1, match, 0) == 0) {
        count++;
        cursor += match[0].rm_eo;
    }
    regfree(&regex);

    return count;
}

void searchGrid(char grid[SIZE+2][SIZE+2]) {
    int total_count = 0;

    // horizontal right
    for (int row=0; row<SIZE; row++) total_count += searchPath(grid, row, 0, 0, 1);

    // horizontal left
    for (int row=0; row<SIZE; row++) total_count += searchPath(grid, row, SIZE-1, 0, -1);

    // vertical down
    for (int col=0; col<SIZE; col++) total_count += searchPath(grid, 0, col, 1, 0);

    // vertical up
    for (int col=0; col<SIZE; col++) total_count += searchPath(grid, SIZE-1, col, -1, 0);

    // diag tl-br
    for (int col=0; col<SIZE; col++) total_count += searchPath(grid, 0, col, 1, 1);
    for (int row=SIZE-1; row>0; row--) total_count += searchPath(grid, row, 0, 1, 1);

    // diag br-tl
    for (int row=SIZE-1; row>=0; row--) total_count += searchPath(grid, row, SIZE-1, -1, -1);
    for (int col=0; col<SIZE-1; col++) total_count += searchPath(grid, SIZE-1, col, -1, -1);

    // diag tr-bl
    for (int row=0; row<SIZE; row++) total_count += searchPath(grid, row, SIZE-1, 1, -1);
    for (int col=0; col<SIZE-1; col++) total_count += searchPath(grid, 0, col, 1, -1);

    // diag bl-tr
    for (int col=0; col<SIZE; col++) total_count += searchPath(grid, SIZE-1, col, -1, 1);
    for (int row=0; row<SIZE-1; row++) total_count += searchPath(grid, row, 0, -1, 1);

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
