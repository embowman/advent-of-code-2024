#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SECTION_BREAK 1176
#define TABLE_SIZE 100

typedef struct Page {
    int value;
    struct Page *next;
} Page;

Page *Index[TABLE_SIZE];

Page *createPage(int value) {
    Page *newPage = (Page *)malloc(sizeof(Page));
    newPage->value = value;
    newPage->next = NULL;
    return newPage;
}

void indexPage(int a, int b) {
    Page *page = Index[b];
    if (page == NULL) {
        Index[b] = createPage(a);
        return;
    }
    while (page->next != NULL) page = page->next;
    page->next = createPage(a);
}

void blockPages(int *block, int key) {
    Page *page = Index[key];
    if (page == NULL) return;
    while (page->next != NULL) {
        block[page->value] = 1;
        page = page->next;
    }
    block[page->value] = 1;
}

int main() {
    FILE *restrict stream = fopen("manual_changes.txt", "r");
    if (stream == NULL) {
        perror("Cannot open file\n");
        return 1;
    }

    char line[70];
    int lc = 0;
    int sum = 0;
    while (fgets(line, sizeof(line), stream) != NULL) {
        if (lc < SECTION_BREAK) {
            char num1_str[4], num2_str[4];
            snprintf(num1_str, 3, "%s", &line[0]);
            snprintf(num2_str, 3, "%s", &line[3]);
            indexPage(atoi(num1_str), atoi(num2_str));
        } else if (lc > SECTION_BREAK) {
            int block[TABLE_SIZE] = {0};
            int update[100];
            int u = 0;
            char *token = strtok(line, ",");
            while (token != NULL) {
                update[u++] = atoi(token);
                token = strtok(NULL, ",");
            }

            int flag = 0;
            for (int i=0; i<u; i++) {
                blockPages(block, update[i]);
                if (block[update[i]] != 0) {
                    flag = 1;
                    break;
                }
            }
            if (!flag) sum += update[u/2];
        }
        lc++;
    }
    fclose(stream);

    printf("Sum of middle pages from good updates: %d\n", sum);

    return 0;
}
