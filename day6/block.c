#include <stdio.h>

#define SIZE 130

char map[SIZE+2][SIZE+2];
int init_r, init_c;
int path[SIZE][SIZE];

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

void markPath() {
    int r = init_r, c = init_c;
    int dr = -1, dc = 0;
    int tmp;
    while (r >= 0 && r < SIZE && c >= 0 && c < SIZE) {
        // mark space
        path[r][c] = 1;
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
    }
    // remove guard's initial position from potential spaces
    path[init_r][init_c] = 0;
}

int getDirection(int dr, int dc) {
    if (dr == -1 && dc == 0) return 2;
    else if (dr == 0 && dc == 1) return 3;
    else if (dr == 1 && dc == 0) return 5;
    else if (dr == 0 && dc == -1) return 7;
    else {
        perror("Error getting direction");
        return 0;
    }
}

int checkForLoop(int r, int c, int dr, int dc) {
    int obstacle_map[SIZE][SIZE];
    for (int i=0; i<SIZE; i++) {
        for (int j=0; j<SIZE; j++) {
            obstacle_map[i][j] = 1;
        }
    }
    int tmp;
    while (r >= 0 && r < SIZE && c >= 0 && c < SIZE) {
        if (map[r][c] == '#') {
            if (obstacle_map[r][c] % getDirection(dr, dc) == 0) return 1;
            obstacle_map[r][c] *= getDirection(dr, dc);
            // step back
            r -= dr;
            c -= dc;
            // turn right
            tmp = dr;
            dr = dc;
            dc = tmp * -1;
        }
        // step forward
        r += dr;
        c += dc;
    }
    return 0;
}

int walkPath() {
    int r = init_r, c = init_c;
    int dr = -1, dc = 0;
    int count = 0;
    int blocked[SIZE][SIZE] = {0};
    for (int i=0; i<SIZE; i++) {
        for (int j=0; j<SIZE; j++) {
            if (path[i][j] == 0) continue;
            map[i][j] = '#';
            if (checkForLoop(r, c, dr, dc) == 1) {
                blocked[r][c] = 1;
                count++;
            }
            map[i][j] = '.';
        }
    }
    return count;
}

int main() {
    if (loadMap("map.txt") != 0) return 1;
    findInitialPosition();
    markPath();
    printf("Number of obstructable positions: %d\n", walkPath());

    return 0;
}
