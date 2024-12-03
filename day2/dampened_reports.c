#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int validate_safety(int *rep, int size) {
    int level_delta = 0;
    int prev_level = rep[0];
    int curr_level;
    for (int level=1; level<size; level++) {
        curr_level = rep[level];
        if (abs(prev_level - curr_level) < 1 || abs(prev_level - curr_level) > 3) return 1;
        if (prev_level - curr_level > 0 && level_delta < 0) return 1;
        if (prev_level - curr_level < 0 && level_delta > 0) return 1;
        level_delta = prev_level - curr_level;
        prev_level = curr_level;
    }
    return 0;
}

int main() {
    // open reports file
    const char *restrict path = "reports.txt";
    const char *restrict mode = "r";
    FILE *restrict stream = fopen(path, mode);
    if (stream == NULL) {
        perror("Cannot open file\n");
        return 1;
    }

    char line[32];
    int report[8];
    int dampened_report[8];
    unsigned int safe_report_count = 0;
    char *token, *mixed_line;
    unsigned int ctr_a, ctr_b;
    while (fgets(line, sizeof(line), stream) != NULL) {
        // a line that's been stirred up is mixed
        mixed_line = strdup(line);
        if (mixed_line == NULL) {
            perror("Insufficient memory\n");
            return 2;
        }

        ctr_a = 0;
        while ((token = strsep(&mixed_line, " ")) != NULL) {
            if (sscanf(token, "%d", &report[ctr_a]) != 1) break;
            ctr_a += 1;
        }

        for (int prob_dampnr=0; prob_dampnr<ctr_a; prob_dampnr++) {
            ctr_b = 0;
            for (int level=0; level<ctr_a; level++) {
                if (level == prob_dampnr) continue;
                dampened_report[ctr_b] = report[level];
                ctr_b += 1;
            }
            if (validate_safety(dampened_report, ctr_b) == 0) {
                safe_report_count++;
                break;
            }
        }
    }
    printf("Number of safe reports: %d\n", safe_report_count);

    return 0;
}