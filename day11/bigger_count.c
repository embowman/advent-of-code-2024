#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define INPUT_FILE_PATH "stones.txt"
#define MAX_STACK_SIZE 8
#define NUM_BLINKS 75
#define HASH_MAP_SIZE 262144

typedef unsigned long long stone_engraving_t;
typedef unsigned long long stone_count_t;

typedef struct {
    stone_engraving_t arr[MAX_STACK_SIZE];
    int top;
} Stack;

typedef struct {
    stone_engraving_t engraving;
    int blinks;
    bool occupied;
} HashMapKey;

typedef struct {
    HashMapKey key;
    stone_count_t count;
} HashMapEntry;

#pragma region STACK
//////////////////////////////////////////////////////////////////////////////

void stack_init(Stack *stack) {
    stack->top = -1;
}

void stack_push(Stack *stack, stone_engraving_t value) {
    if (stack->top >= MAX_STACK_SIZE) {
        perror("Stack overflow\n");
        exit(EXIT_FAILURE);
    }
    stack->arr[++stack->top] = value;
}

bool stack_pop(Stack *stack, stone_engraving_t *value) {
    if (stack->top < 0) return false;
    *value = stack->arr[stack->top--];
    return true;
}

#pragma endregion

#pragma region INPUT FILE
//////////////////////////////////////////////////////////////////////////////

FILE *file = NULL;

void close_file() {
    if (file != NULL) {
        fclose(file);
        file = NULL;
    }
}

void load_file(Stack *stack, const char *filename) {
    atexit(close_file);
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file\n");
        exit(EXIT_FAILURE);
    }

    stone_engraving_t curr_engraving;
    while (fscanf(file, "%lld", &curr_engraving) == 1) {
        stack_push(stack, curr_engraving);
    }

    fclose(file);
    file = NULL;
}

#pragma endregion

#pragma region MEMO
//////////////////////////////////////////////////////////////////////////////

HashMapEntry hash_map[HASH_MAP_SIZE];

unsigned int hash(stone_engraving_t engraving, int blinks) {
    unsigned long long combined = engraving ^ (unsigned long long) blinks;
    return combined % HASH_MAP_SIZE;
}

void hash_map_store(
    stone_engraving_t engraving,
    int blinks,
    stone_count_t count
) {
    unsigned int index = hash(engraving, blinks);
    unsigned int original_index = index;
    int i = 0;

    while (hash_map[index].key.occupied) {
        if (++i > HASH_MAP_SIZE) {
            perror("Hash map full\n");
            exit(EXIT_FAILURE);
        }
        index = (original_index + i * i) % HASH_MAP_SIZE;
    }

    hash_map[index].count = count;
    hash_map[index].key.engraving = engraving;
    hash_map[index].key.blinks = blinks;
    hash_map[index].key.occupied = true;
}

bool hash_map_lookup(
    stone_engraving_t engraving,
    int blinks,
    stone_count_t *count
) {
    unsigned int index = hash(engraving, blinks);
    unsigned int original_index = index;
    int i = 0;

    while (hash_map[index].key.occupied) {
        if (
            hash_map[index].key.engraving == engraving &&
            hash_map[index].key.blinks == blinks
        ) {
            *count = hash_map[index].count;
            return true;
        }

        if (++i > HASH_MAP_SIZE) {
            return false;
        }
        index = (original_index + i * i) % HASH_MAP_SIZE;
    }

    return false;
}

#pragma endregion

#pragma region RECURSIVE COUNT
//////////////////////////////////////////////////////////////////////////////

stone_count_t count_stones(stone_engraving_t engraving, int blinks) {
    if (blinks == 0) return 1;

    stone_count_t memoized_count;
    if (hash_map_lookup(engraving, blinks, &memoized_count)) {
        return memoized_count;
    }

    stone_count_t count = 0;
    if (engraving == 0) {
        count += count_stones(1ULL, blinks - 1);
    } else {
        u_int64_t d = 10ULL, engraving_length = 1ULL;
        while (engraving >= d) {
            d *= 10ULL;
            engraving_length++;
        }
        if (engraving_length % 2 == 0) {
            d = (u_int64_t) sqrt(d);
            stone_engraving_t left = engraving / d;
            stone_engraving_t right = engraving - (left * d);
            count += count_stones(left, blinks - 1);
            count += count_stones(right, blinks - 1);
        } else {
            count += count_stones(engraving * 2024ULL, blinks - 1);
        }
    }

    hash_map_store(engraving, blinks, count);
    return count;
}

#pragma endregion

#pragma region MAIN
//////////////////////////////////////////////////////////////////////////////

int main() {
    Stack init_stone_engravings;
    stack_init(&init_stone_engravings);
    load_file(&init_stone_engravings, INPUT_FILE_PATH);

    stone_engraving_t curr_engraving;
    stone_count_t total_stones = 0;
    while (stack_pop(&init_stone_engravings, &curr_engraving)) {
        total_stones += count_stones(curr_engraving, NUM_BLINKS);
    }
    printf("%llu\n", total_stones);
}

#pragma endregion
