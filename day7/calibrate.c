#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define FILE_LENGTH 850

long test_values[FILE_LENGTH];
int nums[FILE_LENGTH][16] = {0};

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
            if (sscanf(token, "%d", &nums[line_counter][num_counter]) != 1) break;
            num_counter += 1;
        }
        line_counter++;
    }
    fclose(stream);

    return 0;
}

long calibrate(long test_value, int nums[16]) {
    int num_counter = 0;
    while (nums[num_counter] != 0) {
        num_counter++;
    }
    int size = (int) pow(2, num_counter-1);
    long values[size];
    values[0] = (long) nums[0];
    for (int i=1; i<size; i++) {
        values[i] = 0;
    }
    int n = 1;
    int d = -1;
    int rp = 0, rpe = 0, wp = size-1;
    int tmp;
    while (n < num_counter) {
        int iter = abs(rp - rpe);
        while (iter >= 0) {
            values[wp] = values[rp] + (long) nums[n];
            values[wp+d] = values[rp] * (long) nums[n];

            rp += d;
            wp += d*2;
            iter--;
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
        rpe = size - 1 - wp;

        n++;
    }

    int iter = abs(rp - rpe);
    while (iter >= 0) {
        if (values[rp] == test_value) return test_value;
        rp += d;
        iter--;
    }

    return 0;
}

int main() {
    loadEquations("equations.txt");
    long count = 0;
    for (int i=0; i<FILE_LENGTH; i++) {
        count += calibrate(test_values[i], nums[i]);
    }
    printf("%ld\n", count);
    
    return 0;
}
