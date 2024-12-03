#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    const char *restrict format = "%d";
    int safe_report_count = 0;
    char *token, *report;
    int prev_level, curr_level, level_delta, unsafe;
    while (fgets(line, sizeof(line), stream) != NULL) {
        report = strdup(line);
        if (report == NULL) {
            perror("Insufficient memory\n");
            return 2;
        }
        
        prev_level = 0;
        curr_level = 0;
        level_delta = 0;
        unsafe = 0;
        while ((token = strsep(&report, " ")) != NULL) {
            if (sscanf(token, format, &curr_level) != 1) break;
            if (prev_level == 0) {
                prev_level = curr_level;
                continue;
            }
            if (abs(prev_level - curr_level) < 1 || abs(prev_level - curr_level) > 3) {
                unsafe = 1;
                break;
            }
            if (prev_level - curr_level > 0 && level_delta < 0) {
                unsafe = 1;
                break;
            }
            if (prev_level - curr_level < 0 && level_delta > 0) {
                unsafe = 1;
                break;
            }
            level_delta = prev_level - curr_level;
            prev_level = curr_level;
        }
        if (unsafe == 0) safe_report_count++;
    }
    printf("Number of safe reports: %d\n", safe_report_count);

    return 0;
}