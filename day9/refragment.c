#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int id, len;
    struct Node *prev, *next;
} Node;

Node *Head, *Tail;

Node *createNode(int id, int len, Node *prev, Node *next) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->id = id;
    newNode->len = len;
    newNode->prev = prev;
    newNode->next = next;
    return newNode;
}

int loadChain(char filename[16]) {
    FILE *restrict stream = fopen(filename, "r");
    if (stream == NULL) {
        perror("Cannot open file\n");
        return 1;
    }

    char buffer[2];
    size_t bytes_read;
    Head = createNode(-1, -1, NULL, NULL);
    Node *curr_node = Head;
    int counter = 0;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), stream)) > 0) {
        int s = atoi(&buffer[1]);
        buffer[1] = 0;
        int f = atoi(&buffer[0]);
        
        curr_node->next = createNode(counter, f, curr_node, NULL);
        curr_node = curr_node->next;

        if (bytes_read == 1) break;
        counter++;
        if (s == 0) continue;
        curr_node->next = createNode(-1, s, curr_node, NULL);
        curr_node = curr_node->next;
    }
    fclose(stream);

    curr_node->next = createNode(-1, -1, curr_node, NULL);
    Tail = curr_node->next;

    return 0;
}

Node *findEmptyNode(int len) {
    Node *curr_node = Head;
    while (curr_node->id != -1 || curr_node->len < len) {
        curr_node = curr_node->next;
        if (curr_node->len == -1 || curr_node->id == Tail->id) return NULL;
    }
    return curr_node;
}

void removeNode(Node *node) {
    Node *newNode = createNode(-1, node->len, node->prev, node->next);
    node->prev->next = newNode;
    node->next->prev = newNode;
}

void insertNode(Node *empty_node, Node *curr_node) {
    curr_node->prev = empty_node->prev;
    curr_node->prev->next = curr_node;

    int diff = empty_node->len - curr_node->len;
    if (diff == 0) {
        curr_node->next = empty_node->next;
        curr_node->next->prev = curr_node;
    } else {
        curr_node->next = createNode(-1, diff, curr_node, empty_node->next);
        empty_node->next->prev = curr_node->next;
    }
    free(empty_node);
    empty_node = NULL;
}

void consolidateFreeSpace() {
    Node *curr_node = Head->next;
    Node *tf;
    while (curr_node->next->len != -1) {
        if (curr_node->id == -1 && curr_node->next->id == -1) {
            tf = curr_node->next;
            curr_node->len += curr_node->next->len;
            curr_node->next = curr_node->next->next;
            curr_node->next->prev = curr_node;
            free(tf);
            continue;
        }
        curr_node = curr_node->next;
    }
}

void compactDisk() {
    Node *empty_node;
    Node *curr_node = Tail->prev;
    while (curr_node->len != -1) {
        if (curr_node->id == -1) {
            curr_node = curr_node->prev;
            continue;
        }

        Tail->id = curr_node->id;
        empty_node = findEmptyNode(curr_node->len);
        if (empty_node == NULL) {
            curr_node = curr_node->prev;
            continue;
        }

        removeNode(curr_node);
        insertNode(empty_node, curr_node);
        consolidateFreeSpace();
        
        curr_node = Tail->prev;
        while (curr_node->id == -1 || curr_node->id >= Tail->id) {
            curr_node = curr_node->prev;
        }
    }
}

long getCheckSum() {
    Node *curr_node = Head->next;
    int counter = 0;
    long sum = 0;
    while (curr_node->len != -1) {
        if (curr_node->id == -1) {
            counter += curr_node->len;
            curr_node = curr_node->next;
            continue;
        }
        for (int i=0; i<curr_node->len; i++) {
            sum += curr_node->id * counter;
            counter++;
        }
        curr_node = curr_node->next;
    }
    return sum;
}

int main() {
    if (loadChain("disk_map.txt") != 0) return 1;
    compactDisk();
    printf("%ld\n", getCheckSum());

    return 0;
}
