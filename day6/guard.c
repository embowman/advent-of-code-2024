#include <stdio.h>

#define SIZE 130

char map[SIZE+2][SIZE+2];
int init_r, init_c;

int loadMap(char filename[16]) {
    // open file
    FILE *restrict stream = fopen(filename, "r");
    if (stream == NULL) {
        perror("Cannot open file\n");
        return 1;
    }

    // read in map
    int i = 0;
    while (fgets(map[i], sizeof(map[i]), stream) != NULL) {
        map[i][SIZE] = 0;
        i++;
    }
    fclose(stream);

    return 0;
}

void findInitialPosition() {
    for (int r=0; r<SIZE; r++) {
        for (int c=0; c<SIZE; c++) {
            if (map[r][c] == '^') {
                init_r = r;
                init_c = c;
                return;
            }
        }
    }
}

int walkPath() {
    int visited[SIZE][SIZE] = {0};
    int r = init_r, c = init_c;
    int dr = -1, dc = 0;
    int count = 0;
    int tmp;
    while (r >= 0 && r < SIZE && c >= 0 && c < SIZE) {
        // mark space
        visited[r][c] = 1;
        // step forward
        r += dr;
        c += dc;
        if (map[r][c] == '#') {
            // step back
            r -= dr;
            c -= dc;
            // turn right
            tmp = dr;
            dr = dc;
            dc = tmp * -1;
        }
        if (visited[r][c] == 1) continue;
        count++;
    }
    return count;
}

int main() {
    if (loadMap("map.txt") != 0) return 1;
    findInitialPosition();
    printf("Number of distinct positions: %d\n", walkPath());

    return 0;
}
