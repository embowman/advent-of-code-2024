#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b) {
    int *da = (int *)a;
    int *db = (int *)b;
    return *da - *db;
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
    int right_locations[1024];
    size_t nel = 0;
    while (fscanf(stream, format, &left_loc, &right_loc) != EOF) {
        left_locations[nel] = left_loc;
        right_locations[nel] = right_loc;
        nel++;
    }
    fclose(stream);

    // sort locations
    qsort(left_locations, nel, sizeof(int), compare);
    qsort(right_locations, nel, sizeof(int), compare);

    // calculate total distance
    int total_distance = 0;
    for (int i=0; i<nel; i++) {
        total_distance += abs(left_locations[i] - right_locations[i]);
    }
    printf("Total distance: %d\n", total_distance);
    
    return 0;
}
