#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct Node {
    long val;
    struct Node *next;
} Node;

Node *Head, *Tail;

Node *createNode(long val) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->val = val;
    newNode->next = NULL;
    return newNode;
}

int loadFile(char filename[16]) {
    FILE *restrict stream = fopen(filename, "r");
    if (stream == NULL) {
        perror("Cannot open file\n");
        return 1;
    }

    char line[64];
    if (fgets(line, sizeof(line), stream) == NULL) return 2;
    fclose(stream);

    char *token, *stirred_line;
    stirred_line = strdup(line);
    if (stirred_line == NULL) {
        perror("Insufficient memory\n");
        return 2;
    }

    Head = createNode(-1);
    Node *curr_node = Head;
    while ((token = strsep(&stirred_line, " ")) != NULL) {
        curr_node->next = createNode((long)atoi(token));
        curr_node = curr_node->next;
    }
    Tail = createNode(-1);
    curr_node->next = Tail;

    return 0;
}

void transformStones() {
    Node *curr_node = Head->next;
    while (curr_node->val != -1) {
        if (curr_node->val == 0) {
            curr_node->val = 1;
            curr_node = curr_node->next;
            continue;
        }
        long d = 10, nd = 1;
        while (curr_node->val >= d) {
            d *= 10;
            nd++;
        }
        if (nd % 2 == 0) {
            d = (long)sqrt(d);
            nd = curr_node->val;
            curr_node->val = nd / d;
            Node *tmp = curr_node->next;
            curr_node->next = createNode(nd - (d * curr_node->val));
            curr_node = curr_node->next;
            curr_node->next = tmp;
            curr_node = curr_node->next;
            continue;
        }
        curr_node->val *= 2024;
        curr_node = curr_node->next;
    }
}

int main() {
    if (loadFile("stones.txt") != 0) return 1;

    for (int i=0; i<25; i++) transformStones();

    Node *curr_node = Head->next;
    int sum = 0;
    while (curr_node->val != -1) {
        curr_node = curr_node->next;
        sum++;
    }
    printf("%d\n", sum);

    return 0;
}
