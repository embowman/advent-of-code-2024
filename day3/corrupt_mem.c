#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

int main() {
    // open "memory" file
    FILE *restrict stream = fopen("corrupted_memory.txt", "r");
    if (stream == NULL) {
        perror("Cannot open file\n");
        return 1;
    }

    // get file size
    if (fseek(stream, 0, SEEK_END) != 0) {
        perror("Cannot seek end of file\n");
        return 1;
    };
    long file_size = ftell(stream);
    rewind(stream);

    // allocate memory for file buffer
    char *file_buffer = malloc(file_size + 1);
    if (file_buffer == NULL) {
        perror("Cannot allocate memory for file buffer\n");
        return 1;
    }

    // read file into buffer
    fread(file_buffer, 1, file_size, stream);
    file_buffer[file_size] = 0;

    // compile regular expression
    regex_t preg;
    int reti = regcomp(&preg, "mul\\(([0-9]+),([0-9]+)\\)", REG_EXTENDED);
    if (reti != 0) {
        perror("Cannot compile regular expression\n");
        return 1;
    }

    // search
    char *cursor = file_buffer;
    regmatch_t pmatch[3];
    unsigned int mul_sum = 0;
    while (regexec(&preg, cursor, 3, pmatch, 0) == 0) {
        char num1_str[4], num2_str[4];

        snprintf(num1_str, pmatch[1].rm_eo - pmatch[1].rm_so + 1, "%s", cursor + pmatch[1].rm_so);
        snprintf(num2_str, pmatch[2].rm_eo - pmatch[2].rm_so + 1, "%s", cursor + pmatch[2].rm_so);

        mul_sum += atoi(num1_str) * atoi(num2_str);
        
        cursor += pmatch[0].rm_eo;
    }
    printf("Sum of all multiplications: %d\n", mul_sum);

    return 0;
}
