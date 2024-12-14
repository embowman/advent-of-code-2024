#include <stdio.h>
#include <stdlib.h>

#define MAP_SIZE 48

typedef struct Node {
    int val, id;
    struct Node *up, *down, *left, *right;
} Node;

Node *map[MAP_SIZE][MAP_SIZE];
Node *trail[2][MAP_SIZE*MAP_SIZE];

Node *createNode(int val, int id) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->val = val;
    newNode->id = id;
    newNode->up = NULL;
    newNode->down = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

int loadFile(char filename[16]) {
    FILE *restrict stream = fopen(filename, "r");
    if (stream == NULL) {
        perror("Cannot open file\n");
        return 1;
    }

    char line[MAP_SIZE+2];
    int up, down, left, right;
    int i = 0, id = 1;
    while (fgets(line, sizeof(line), stream) != NULL) {
        line[MAP_SIZE] = 0;
        for (int j=0; j<MAP_SIZE; j++) {
            map[i][j] = createNode(line[j]-'0', id);
            id++;
        }
        i++;
    }
    fclose(stream);

    return 0;
}

void buildLowerRelationships() {
    for (int i=0; i<MAP_SIZE; i++) {
        for (int j=0; j<MAP_SIZE; j++) {
            if (map[i][j] == NULL) continue;
            if (map[i][j]->val == 0) continue;
            if (i != 0 && ((map[i][j]->val - map[i-1][j]->val) == 1)) map[i][j]->up = map[i-1][j];
            if (i != MAP_SIZE-1 && ((map[i][j]->val - map[i+1][j]->val) == 1)) map[i][j]->down = map[i+1][j];
            if (j != 0 && ((map[i][j]->val - map[i][j-1]->val) == 1)) map[i][j]->left = map[i][j-1];
            if (j != MAP_SIZE-1 && ((map[i][j]->val - map[i][j+1]->val) == 1)) map[i][j]->right = map[i][j+1];
        }
    }
}

void buildUpperRelationships() {
    for (int i=0; i<MAP_SIZE; i++) {
        for (int j=0; j<MAP_SIZE; j++) {
            if (map[i][j] == NULL) continue;
            if (map[i][j]->val == 9) continue;
            if (i != 0 && ((map[i][j]->val - map[i-1][j]->val) == -1)) map[i][j]->up = map[i-1][j];
            if (i != MAP_SIZE-1 && ((map[i][j]->val - map[i+1][j]->val) == -1)) map[i][j]->down = map[i+1][j];
            if (j != 0 && ((map[i][j]->val - map[i][j-1]->val) == -1)) map[i][j]->left = map[i][j-1];
            if (j != MAP_SIZE-1 && ((map[i][j]->val - map[i][j+1]->val) == -1)) map[i][j]->right = map[i][j+1];
        }
    }
}

int removeLowerNodes() {
    int count = 0;
    for (int i=0; i<MAP_SIZE; i++) {
        for (int j=0; j<MAP_SIZE; j++) {
            if (map[i][j] == NULL) continue;
            if (map[i][j]->val == 0) continue;

            if (map[i][j]->up != NULL && ((map[i][j]->val - map[i][j]->up->val) == 1)) continue;
            if (map[i][j]->down != NULL && ((map[i][j]->val - map[i][j]->down->val) == 1)) continue;
            if (map[i][j]->left != NULL && ((map[i][j]->val - map[i][j]->left->val) == 1)) continue;
            if (map[i][j]->right != NULL && ((map[i][j]->val - map[i][j]->right->val) == 1)) continue;

            free(map[i][j]);
            map[i][j] = NULL;

            count++;
        }
    }

    return count;
}

int removeUpperNodes() {
    int count = 0;
    for (int i=0; i<MAP_SIZE; i++) {
        for (int j=0; j<MAP_SIZE; j++) {
            if (map[i][j] == NULL) continue;
            if (map[i][j]->val == 9) continue;

            if (map[i][j]->up != NULL && ((map[i][j]->val - map[i][j]->up->val) == -1)) continue;
            if (map[i][j]->down != NULL && ((map[i][j]->val - map[i][j]->down->val) == -1)) continue;
            if (map[i][j]->left != NULL && ((map[i][j]->val - map[i][j]->left->val) == -1)) continue;
            if (map[i][j]->right != NULL && ((map[i][j]->val - map[i][j]->right->val) == -1)) continue;

            free(map[i][j]);
            map[i][j] = NULL;

            count++;
        }
    }

    return count;
}

int checkID(int id, int p, int size) {
    for (int i=0; i<size; i++) {
        if (trail[p][i]->id == id) return 1;
    }
    return 0;
}

int mapTrails(Node *node) {
    int rp = 0, wp = 1;
    int offset[2] = {0};
    int tmp;
    
    trail[rp][offset[rp]] = node;
    offset[rp]++;

    for (int i=1; i<10; i++) {
        for (int j=0; j<offset[rp]; j++) {
            node = trail[rp][j];
            if (
                node->up != NULL &&
                node->up->val == i &&
                checkID(node->up->id, wp, offset[wp]) == 0
            ) {
                trail[wp][offset[wp]] = node->up;
                offset[wp]++;
            }
            if (
                node->down != NULL &&
                node->down->val == i &&
                checkID(node->down->id, wp, offset[wp]) == 0
            ) {
                trail[wp][offset[wp]] = node->down;
                offset[wp]++;
            }
            if (
                node->left != NULL &&
                node->left->val == i &&
                checkID(node->left->id, wp, offset[wp]) == 0
            ) {
                trail[wp][offset[wp]] = node->left;
                offset[wp]++;
            }
            if (
                node->right != NULL &&
                node->right->val == i &&
                checkID(node->right->id, wp, offset[wp]) == 0
            ) {
                trail[wp][offset[wp]] = node->right;
                offset[wp]++;
            }
        }
        tmp = rp;
        rp = wp;
        wp = tmp;
        offset[wp] = 0;
    }

    return offset[rp];
}

int main() {
    if (loadFile("map.txt") != 0) return 1;

    buildLowerRelationships();
    buildUpperRelationships();
    while (removeLowerNodes() != 0);
    while (removeUpperNodes() != 0);

    int sum = 0;
    for (int r=0; r<MAP_SIZE; r++) {
        for (int c=0; c<MAP_SIZE; c++) {
            if (map[r][c] == NULL) continue;
            if (map[r][c]->val == 0) {
               sum += mapTrails(map[r][c]);
            };
        }
    }
    printf("%d\n", sum);

    return 0;
}
