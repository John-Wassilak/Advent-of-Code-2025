
#include "util.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char test_data[] = "11-22,95-115,998-1012,1188511880-1188511890,222220-222224,"
                   "1698522-1698528,446443-446449,38593856-38593862,565653-"
                   "565659,824824821-824824827,2121212118-2121212124";

typedef struct LongNode {
    long long data;
    struct LongNode *next;
} ll_Long;

ll_Long *append_ll_Long(ll_Long *tail, long long data) {
    ll_Long *new_tail = (ll_Long *)malloc(sizeof(ll_Long));
    new_tail->data = data;
    new_tail->next = NULL;
    if (tail) {
        tail->next = new_tail;
    }
    return new_tail;
}

// lazy, just creating one list of long values
// planning to bravely iterate two at a time
ll_Long *parse_input(char *raw_input) {
    ll_Long *head = NULL;
    ll_Long *tail = NULL;
    long long start, end;
    char *line;

    // line by line
    for (line = strtok(raw_input, ","); line != NULL;
         line = strtok(NULL, ",")) {

        // blind parse
        sscanf(line, "%lld-%lld", &start, &end);

        // append start
        if (!head) {
            head = append_ll_Long(NULL, start);
            tail = head;
        } else {
            tail = append_ll_Long(tail, start);
        }

        // append end
        tail = append_ll_Long(tail, end);
    }

    return head;
}

void free_ll_Long(ll_Long *input) {
    ll_Long *current;

    while (input) {
        current = input;
        input = current->next;
        free(current);
    }
}

long long solve_part_1(ll_Long *parsed_input) {
    char str[21]; // Buffer for up to 64-bit integer
    long long i, start, end;
    long long result = 0;

    while (parsed_input) {
        start = parsed_input->data;
        parsed_input = parsed_input->next;
        end = parsed_input->data;

        for (i = start; i <= end; i++) {
            sprintf(str, "%lld", i);
            int len = strlen(str);

            // if even and first half matches second half, save to result
            if (len % 2 == 0) {
                int half = len / 2;
                if (strncmp(str, str + half, half) == 0) {
                    result += i;
                }
            }
        }
        parsed_input = parsed_input->next;
    }

    return result;
}

long long solve_part_2(ll_Long *parsed_input) {
    char str[21]; // Buffer for up to 64-bit integer
    char tmp[21]; // Buffer for string comparison
    long long i, start, end;
    int seq_width, reps, j, len, half;
    long long result = 0;

    while (parsed_input) {
        start = parsed_input->data;
        parsed_input = parsed_input->next;
        end = parsed_input->data;

        for (i = start; i <= end; i++) {
            sprintf(str, "%lld", i);
            len = strlen(str);
            half = len / 2;

            // come up with a sequence of digits
            for (seq_width = 1; seq_width <= half; seq_width++) {

                // check if the string is the right length
                // to just be made of a repeating sequence
                if (len % seq_width == 0) {

                    // construct string by repeating sequence
                    reps = len / seq_width;
                    tmp[0] = '\0';
                    for (j = 0; j < reps; j++) {
                        strncat(tmp, str, seq_width);
                    }

                    // compare to input
                    if (strcmp(tmp, str) == 0) {
                        result += i;
                        break; // avoid multi counting things like 222222
                    }
                }
            }
        }
        parsed_input = parsed_input->next;
    }

    return result;
}

int main() {
    char *input = pull_input("https://adventofcode.com/2025/day/2/input");
    ll_Long *parsed = parse_input(input);
    long long part_1 = solve_part_1(parsed);
    long long part_2 = solve_part_2(parsed);

    printf("(part 1) invalid IDs added up : %lld\n", part_1);
    printf("(part 2) invalid IDs added up : %lld\n", part_2);

    free(input);
    free_ll_Long(parsed);
}
