#include <stdio.h>
#include <stdlib.h>
#include <regex.h>

char* loadMemoryFile() {
    FILE *restrict stream = fopen("corrupted_memory.txt", "r");
    if (stream == NULL) {
        perror("Cannot open file\n");
        return NULL;
    }
    if (fseek(stream, 0, SEEK_END) != 0) {
        perror("Cannot seek end of file\n");
        return NULL;
    };
    long file_size = ftell(stream);
    rewind(stream);
    char *file_buffer = malloc(file_size + 1);
    if (file_buffer == NULL) {
        perror("Cannot allocate memory for file buffer\n");
        return NULL;
    }
    fread(file_buffer, 1, file_size, stream);
    fclose(stream);
    file_buffer[file_size] = 0;

    return file_buffer;
}

int main() {
    char *file_cursor;
    if ((file_cursor = loadMemoryFile()) == NULL) return 1;

    // compile regular expressions
    regex_t mul_regex, do_regex, dont_regex;
    if (regcomp(&mul_regex, "mul\\(([0-9]+),([0-9]+)\\)", REG_EXTENDED) != 0) {
        perror("Cannot compile regular expression\n");
        return 1;
    }
    if (regcomp(&do_regex, "do\\(\\)", REG_EXTENDED) != 0) {
        perror("Cannot compile regular expression\n");
        return 1;
    }
    if (regcomp(&dont_regex, "don't\\(\\)", REG_EXTENDED) != 0) {
        perror("Cannot compile regular expression\n");
        return 1;
    }

    // search after "do()" and before "dont()"
    regmatch_t mul_match[3], do_match[1], dont_match[1];
    unsigned int mul_sum = 0;
    regexec(&dont_regex, file_cursor, 1, dont_match, 0);
    char *dont_flag = file_cursor + dont_match[0].rm_eo;
    while (regexec(&mul_regex, file_cursor, 3, mul_match, 0) == 0) {
        char num1_str[4], num2_str[4];
        snprintf(num1_str, mul_match[1].rm_eo - mul_match[1].rm_so + 1, "%s", file_cursor + mul_match[1].rm_so);
        snprintf(num2_str, mul_match[2].rm_eo - mul_match[2].rm_so + 1, "%s", file_cursor + mul_match[2].rm_so);
        file_cursor += mul_match[0].rm_eo;

        if (file_cursor < dont_flag) mul_sum += atoi(num1_str) * atoi(num2_str);
        else {
            if (regexec(&do_regex, file_cursor, 1, do_match, 0) != 0) break;
            file_cursor += do_match[0].rm_eo;
        }
        if (regexec(&dont_regex, file_cursor, 1, dont_match, 0) == 0) dont_flag = file_cursor + dont_match[0].rm_eo;
    }
    printf("Sum of multiplications after do() and before dont(): %d\n", mul_sum);

    return 0;
}
