#include <stdio.h>
#include <stdlib.h>

#define MAP_SIZE 50
#define LIST_SIZE 256

int map[MAP_SIZE][MAP_SIZE] = {0};

typedef struct Node {
    int r, c;
    struct Node *next;
} Node;

Node *List[LIST_SIZE];

Node *createNode(int r, int c, Node *next) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->r = r;
    newNode->c = c;
    newNode->next = next;
    return newNode;
}

void insertNode(int key, int r, int c) {
    Node *node = List[key];
    List[key] = createNode(r, c, node);
}

int loadMap(char filename[16]) {
    // open file
    FILE *restrict stream = fopen(filename, "r");
    if (stream == NULL) {
        perror("Cannot open file\n");
        return 1;
    }

    // read in map
    char line[64];
    for (int i=0; i<MAP_SIZE; i++) {
        if (fgets(line, sizeof(line), stream) == NULL) return 2;
        for (int j=0; j<MAP_SIZE; j++) {
            if (line[j] != '.') insertNode((int)line[j], i, j);
        }
    }
    fclose(stream);

    return 0;
}

int main() {
    if (loadMap("antennas.txt") != 0) return 1;

    for (int n=0; n<LIST_SIZE; n++) {
        Node *curr_node = List[n];
        while (curr_node != NULL) {
            Node *next_node = curr_node->next;
            while (next_node != NULL) {
                int dr = next_node->r - curr_node->r;
                int dc = next_node->c - curr_node->c;

                int ar = next_node->r + dr;
                int ac = next_node->c + dc;
                if (ar >= 0 && ar < MAP_SIZE && ac >= 0 && ac < MAP_SIZE) map[ar][ac] = 1;

                int br = curr_node->r - dr;
                int bc = curr_node->c - dc;
                if (br >= 0 && br < MAP_SIZE && bc >= 0 && bc < MAP_SIZE) map[br][bc] = 1;

                next_node = next_node->next;
            }
            curr_node = curr_node->next;
        }
    }

    int count = 0;
    for (int r=0; r<MAP_SIZE; r++) {
        for (int c=0; c<MAP_SIZE; c++) {
            if (map[r][c]) count += 1;
        }
    }
    printf("%d\n", count);

    return 0;
}
