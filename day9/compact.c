#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int id, hot;
    struct Node *prev, *next;
} Node;

Node *Head, *Tail;

Node *createNode(int id, Node *prev, Node *next) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->id = id;
    newNode->hot = 0;
    newNode->prev = prev;
    newNode->next = next;
    return newNode;
}

int loadMap(char filename[16]) {
    FILE *restrict stream = fopen(filename, "r");
    if (stream == NULL) {
        perror("Cannot open file\n");
        return 1;
    }

    char buffer[2];
    size_t bytes_read;
    Head = createNode(-1, NULL, NULL);
    Node *curr_node = Head;
    int counter = 0;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), stream)) > 0) {
        int s = atoi(&buffer[1]);
        buffer[1] = 0;
        int f = atoi(&buffer[0]);
        
        for (int i=0; i<f; i++) {
            curr_node->next = createNode(counter, curr_node, NULL);
            curr_node = curr_node->next;
        }
        if (bytes_read == 1) break;
        for (int i=0; i<s; i++) {
            curr_node->next = createNode(-1, curr_node, NULL);
            curr_node = curr_node->next;
        }
        counter++;
    }
    fclose(stream);

    Head = Head->next;
    Tail = curr_node;

    return 0;
}

int main() {
    if (loadMap("disk_map.txt") != 0) return 1;

    Node *curr_node = Head;
    curr_node->hot = 1;
    Tail->hot = 1;
    while (curr_node != NULL) {
        if (curr_node->id != -1) {
            curr_node->hot = 0;
            curr_node = curr_node->next;
            if (curr_node->hot == 1) break;
            curr_node->hot = 1;
            continue;
        }
        while (Tail->id == -1) {
            Tail->hot = 0;
            Tail = Tail->prev;
            if (Tail->hot == 1) break;
            Tail->hot = 1;
            Tail->next = NULL;
        }
        curr_node->id = Tail->id;

        Tail->hot = 0;
        Tail = Tail->prev;
        if (Tail->hot == 1) break;
        Tail->hot = 1;
        Tail->next = NULL;

        curr_node->hot = 0;
        curr_node = curr_node->next;
        if (curr_node->hot == 1) break;
        curr_node->hot = 1;
    }

    curr_node = Head;
    int counter = 0;
    long sum = 0;
    while (curr_node != NULL) {
        if (curr_node->id != -1) sum += curr_node->id * counter;
        curr_node = curr_node->next;
        counter++;
    }
    printf("%ld\n", sum);

    return 0;
}
