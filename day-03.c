#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// should probably practice something
// other than a linked list.
// using dynamic array:
typedef struct {
    char **strings;
    size_t size;
    size_t capacity;
} string_array;

string_array *init_string_array() {
    string_array *a = (string_array *)malloc(sizeof(string_array));
    a->size = 0;
    a->capacity = 32;
    a->strings = malloc(a->capacity * sizeof(char *));

    return a;
}

void append_string_array(string_array *a, char *str) {
    if (a->size == a->capacity) {
        size_t newCapacity = a->capacity * 2;
        char **temp = realloc(a->strings, newCapacity * sizeof(char *));
        a->strings = temp;
        a->capacity = newCapacity;
    }

    a->strings[a->size++] = strdup(str);
}

void free_string_array(string_array *a) {
    for (size_t i = 0; i < a->size; i++) {
        free(a->strings[i]);
    }
    free(a->strings);
    free(a);
}

// I get that I can probably just readline my way
// thru the input, but I want to play with data structures...
string_array *parse_input(char *input) {
    string_array *a = init_string_array();
    char *line;

    for (line = strtok(input, "\n"); line != NULL; line = strtok(NULL, "\n")) {
        append_string_array(a, line);
    }

    return a;
}

int solve_part_1(string_array *input) {
    int result = 0;
    int i, j, joltage;
    char joltage_str[3] = {0};

    for (i = 0; i < input->size; i++) {
        char *line = input->strings[i];
        int len = strlen(line);

        // first digit is basically the greatest
        // digit that isn't the last digit
        int first_digit_i = 0;
        for (j = 1; j < len - 1; j++) {
            if (line[j] > line[first_digit_i]) {
                first_digit_i = j;
            }
        }

        // second digit is the greatest digit
        // appearing after the first digit
        int second_digit_i = first_digit_i + 1;
        for (j = first_digit_i + 1; j < len; j++) {
            if (line[j] > line[second_digit_i]) {
                second_digit_i = j;
            }
        }

        joltage_str[0] = line[first_digit_i];
        joltage_str[1] = line[second_digit_i];
        joltage = atoi(joltage_str);

        result += joltage;
    }

    return result;
}

// could probably do with less iterators
// but it works...
long long solve_part_2(string_array *input) {
    long long result = 0;
    int digit, range_start, range_end, i, j;
    int indicies[12];
    char joltage_str[13] = {0};
    long long joltage;

    for (i = 0; i < input->size; i++) { // for each line
        char *line = input->strings[i];
        int len = strlen(line);

        for (digit = 0; digit < 12; digit++) { // search for each digit
            if (digit == 0) {
                range_start = 0;
            } else { // lookup last digit and start to the right
                range_start = indicies[digit - 1] + 1;
            }
            range_end = len - (12 - digit); // ensure room for all digits
            indicies[digit] = range_start;

            for (j = range_start; j <= range_end; j++) { // find max
                if (line[j] > line[indicies[digit]]) {
                    indicies[digit] = j;
                }
            }
        }

        for (j = 0; j < 12; j++) { // build resulting number
            joltage_str[j] = line[indicies[j]];
        }
        joltage = atoll(joltage_str);
        result += joltage;
    }
    return result;
}

int main() {
    char *input = pull_input("https://adventofcode.com/2025/day/3/input");
    string_array *parsed = parse_input(input);
    
    int part_1 = solve_part_1(parsed);
    long long part_2 = solve_part_2(parsed);

    printf("(part 1) total joltage : %d\n", part_1);
    printf("(part 2) total joltage : %lld\n", part_2);

    free(input);
    free_string_array(parsed);

    return 0;
}
