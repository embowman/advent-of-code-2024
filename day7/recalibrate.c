#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FILE_LENGTH 850
#define NUM_OPERATORS 3
// 12 causes bus error
#define MAX_NUMS 16
// 3^(12-1) = 177147
#define BUFFER_SIZE 177147

long test_values[FILE_LENGTH];
long nums[FILE_LENGTH][MAX_NUMS] = {0};
long values[BUFFER_SIZE];

int loadEquations(char filename[16]) {
    // open file
    FILE *restrict stream = fopen(filename, "r");
    if (stream == NULL) {
        perror("Cannot open file\n");
        return 1;
    }

    char line[64];
    char *token, *stirred_line;
    int line_counter = 0;
    int num_counter, skip_test_value;
    while (fgets(line, sizeof(line), stream) != NULL) {
        stirred_line = strdup(line);
        if (stirred_line == NULL) {
            perror("Insufficient memory\n");
            return 2;
        }

        if ((token = strsep(&stirred_line, ":")) == NULL) return 3;
        if (sscanf(token, "%ld", &test_values[line_counter]) != 1) return 4;

        num_counter = 0;
        skip_test_value = 0;
        while ((token = strsep(&stirred_line, " ")) != NULL) {
            if (skip_test_value == 0) {
                skip_test_value = 1;
                continue;
            }
            if (sscanf(token, "%ld", &nums[line_counter][num_counter]) != 1) break;
            num_counter += 1;
        }
        nums[line_counter][15] = num_counter;
        line_counter++;
    }
    fclose(stream);

    return 0;
}

long concat(long a, long b) {
    char str_a[32], str_b[32];
    sprintf(str_a, "%ld", a);
    sprintf(str_b, "%ld", b);
    strcat(str_a, str_b);
    return strtol(str_a, NULL, 10);
}

long calibrate(long test_value, long nums[MAX_NUMS]) {
    values[0] = nums[0];
    int d = -1;
    int rp = 0, rpe = 0, wp = BUFFER_SIZE-1;
    for (int n=1; n<nums[15]; n++) {
        for (int i=abs(rp-rpe); i>=0; i--) {
            values[wp] = values[rp] + nums[n];
            values[wp+d] = values[rp] * nums[n];
            values[wp+d+d] = concat(values[rp], nums[n]);

            rp += d;
            wp += d*NUM_OPERATORS;
        }
        // change direction
        d *= -1;
        // shift back one
        rp += d;
        wp += d;
        // start reading where stopped writing
        rp = wp;
        // start writing where stopped reading
        wp = rpe;
        // stop reading opposite of writing
        rpe = BUFFER_SIZE-1 - wp;
    }

    for (int i=abs(rp-rpe); i>=0; i--) {
        if (values[rp] == test_value) return test_value;
        rp += d;
    }

    return 0;
}

int main() {
    if (loadEquations("equations.txt") != 0) return 1;
    long count = 0;
    for (int i=0; i<FILE_LENGTH; i++) {
        count += calibrate(test_values[i], nums[i]);
    }
    printf("%ld\n", count);
    
    return 0;
}
