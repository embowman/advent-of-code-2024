#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 1024

typedef struct Location {
    int key, value;
    struct Location *next;
} Location;

Location *LocCountsTable[TABLE_SIZE];

Location *createLocation(int key) {
    Location *newLocation = (Location *)malloc(sizeof(Location));
    newLocation->key = key;
    newLocation->value = 1;
    newLocation->next = NULL;
    return newLocation;
}

void insertLocation(int key) {
    unsigned int slot = key % TABLE_SIZE;
    Location *location = LocCountsTable[slot];

    // if slot empty, create new
    if (location == NULL) {
        LocCountsTable[slot] = createLocation(key);
        return;
    }

    // if slot not empty, find key
    Location *prev = NULL;
    while (location != NULL) {
        if (location->key == key) {
            location->value++;
            return;
        }
        prev = location;
        location = location->next;
    }

    // if key not found, append new location
    prev->next = createLocation(key);
}

int getLocation(int key) {
    unsigned int slot = key % TABLE_SIZE;
    Location *location = LocCountsTable[slot];

    while (location != NULL) {
        if (location->key == key) {
            return location->value;
        }
        location = location->next;
    }
    return 0;
}

int main() {
    // open locations file
    const char *restrict path = "locations.txt";
    const char *restrict mode = "r";
    FILE *restrict stream = fopen(path, mode);
    if (stream == NULL) {
        perror("Cannot open file\n");
        return 1;
    }

    // load locations from file
    const char *restrict format = "%d %d";
    int left_loc, right_loc;
    int left_locations[1024];
    size_t nel = 0;
    while (fscanf(stream, format, &left_loc, &right_loc) != EOF) {
        left_locations[nel] = left_loc;
        insertLocation(right_loc);
        nel++;
    }
    fclose(stream);

    // calculate similarity score
    int similarity_score = 0;
    for (int i=0; i<nel; i++) {
        similarity_score += left_locations[i] * getLocation(left_locations[i]);
    }
    printf("Similarity score: %d\n", similarity_score);
    
    return 0;
}
